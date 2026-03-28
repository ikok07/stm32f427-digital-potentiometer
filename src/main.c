
#include <stdio.h>

#include "dac.h"
#include "stm32f4xx_hal.h"
#include "system_config.h"
#include "encoder.h"
#include "error.h"

USART_HandleTypeDef husart1 = {
    .Instance = USART1,
    .Init = {
        .Mode = USART_MODE_TX,
        .BaudRate = 9600,
        .Parity = USART_PARITY_NONE,
        .StopBits = USART_STOPBITS_1,
        .WordLength = USART_WORDLENGTH_8B,
    }
};

TIM_HandleTypeDef htim2 = {
    .Instance = TIM2,
    .Init = {
        .AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE,
        .ClockDivision = TIM_CLOCKDIVISION_DIV1,
        .CounterMode = TIM_COUNTERMODE_UP,
        .Period = ENCODER_TIM_ARR_VALUE - 1,
        .Prescaler = 0x00
    }
};

RTC_HandleTypeDef hrtc = {
    .Instance = RTC
};

DAC_HandleTypeDef hdac1 = {
    .Instance = DAC1
};

System_Config_t systemConfig = {
    .pTIMHandle = &htim2,
    .pUSARTHandle = &husart1,
    .pRTCHandle = &hrtc,
    .pDACHandle = &hdac1
};

int __io_putchar(int ch) {
    HAL_USART_Transmit(systemConfig.pUSARTHandle, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int main(void) {
    if (HAL_Init() != HAL_OK) {
        while (1);
    }

    if (HAL_USART_Init(systemConfig.pUSARTHandle) != HAL_OK) {
        ERR_Trigger(NULL);
        HAL_Delay(2000);
        NVIC_SystemReset();
    }

    // Enable access to backup registers
    __HAL_RCC_PWR_CLK_ENABLE();

    HAL_PWR_EnableBkUpAccess();
    if (HAL_PWREx_EnableBkUpReg() != HAL_OK) {
        ERR_Trigger("Backup registers could not be accessed");
        HAL_Delay(2000);
        NVIC_SystemReset();
    };

    // Init encoder
    if (ENC_Setup() != HAL_OK) {
        ERR_Trigger("Encoder could not be initialized!");
        HAL_Delay(2000);
        NVIC_SystemReset();
    };

    // Init DAC
    if (DAC_Setup() != HAL_OK) {
        ERR_Trigger("Digital to analog converter could not be initialized!");
        HAL_Delay(2000);
        NVIC_SystemReset();
    };

    while (1);
}