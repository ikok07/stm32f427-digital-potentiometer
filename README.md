# STM32F427 Digital Potentiometer

A bare-metal firmware for the **STM32F427ZGTx** microcontroller that emulates a real analog potentiometer using a rotary encoder and a DAC output. Encoder pulses are counted in hardware via TIM2 in encoder mode, and the resulting 12-bit value is converted to an analog voltage on **PA4** (DAC1 Channel 1), making the output electrically indistinguishable from a wiper-voltage of a physical potentiometer.

![PCB 3D Model](https://github.com/ikok07/stm32f427-digital-potentiometer/blob/main/pcb/3d_model.png?raw=true)

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware](#hardware)
  - [Pin Mapping](#pin-mapping)
- [Software Architecture](#software-architecture)
  - [Module Breakdown](#module-breakdown)
- [Building](#building)
  - [Prerequisites](#prerequisites)
  - [Debug Build](#debug-build)
  - [Release Build](#release-build)
- [Flashing](#flashing)
- [USART Debug Output](#usart-debug-output)
- [Project Structure](#project-structure)

---

## Overview

```
Rotary Encoder ──► TIM2 (encoder mode) ──► Event Manager ──► DAC1 CH1 ──► Analog Voltage (PA4)
                         │
                    RTC Backup Register  (value persisted across resets)
```

Every detent of the rotary encoder increments or decrements a 12-bit counter (0 – 4096). The counter value is immediately written to DAC1 Channel 1, producing a proportional analog output voltage between **0 V** and **V_REF** (3.3 V). The current position is stored in the RTC backup register **DR0** so the last-known position survives a power cycle or soft reset.

Pressing the encoder's push-button resets the position back to **0**.

---

## Features

| Feature           | Details                                                     |
|-------------------|-------------------------------------------------------------|
| Encoder interface | TIM2 hardware encoder mode (both edges, noise filter `0xF`) |
| DAC resolution    | 12-bit right-aligned (0 – 4095 → 0 – V_REF)                 |
| Value persistence | RTC backup register DR0 (survives resets / power loss)      |
| Encoder reset     | EXTI2 interrupt on PA2 (encoder push-button)                |
| Debug logging     | USART1 TX @ 9600 8N1 (printf redirect)                      |
| Error indication  | Error LED on PC0, error messages via USART                  |
| Build system      | CMake + arm-none-eabi-gcc                                   |
| Debug probe       | ST-LINK v2 / OpenOCD                                        |

---

## Hardware

### Pin Mapping

| Pin   | Peripheral | Function                                     |
|-------|------------|----------------------------------------------|
| PA0   | TIM2 CH1   | Encoder channel A (TI1)                      |
| PA1   | TIM2 CH2   | Encoder channel B (TI2)                      |
| PA2   | EXTI2      | Encoder push-button (reset position to 0)    |
| PA4   | DAC1 CH1   | Analog output (emulated potentiometer wiper) |
| PA9   | USART1 TX  | Debug serial output                          |
| PA10  | USART1 RX  | Debug serial input                           |
| PC0   | GPIO OUT   | Error LED                                    |

> The DAC output on **PA4** can be fed directly into an audio amplifier, analog control input, or any circuit that expects a variable wiper voltage.

---

## Software Architecture

### Module Breakdown

```
src/
├── main.c              – Peripheral initialisation and superloop
├── encoder.c           – TIM2 encoder setup, IC capture ISR, EXTI reset ISR
├── dac.c               – DAC1 channel 1 initialisation
├── event_manager.c     – Translates encoder value to a DAC output value
├── error.c             – Error LED + USART error reporting
├── stm32fxxx_it.c      – SysTick, TIM2 and EXTI2 interrupt vectors
└── msp/
    ├── tim_msp.c       – GPIO & NVIC low-level init for TIM2
    ├── dac_msp.c       – GPIO low-level init for DAC1 (PA4 analog)
    └── usart_msp.c     – GPIO low-level init for USART1
```

**Encoder flow:**

1. `ENC_Setup()` — initialises TIM2 in encoder mode and restores the last position from the RTC backup register.
2. `HAL_TIM_IC_CaptureCallback()` — fires on every encoder edge; computes direction-aware delta, clamps value to `[0, 4096]`, persists to backup register, then calls `EM_HandleNewEncValue()`.
3. `EM_HandleNewEncValue()` — writes the new 12-bit value to `DAC1 CH1` using `HAL_DAC_SetValue()`.
4. `HAL_GPIO_EXTI_Callback()` — resets position to 0 when the push-button is pressed (PA2 rising edge).