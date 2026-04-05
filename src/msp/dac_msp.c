//
// Created by Kok on 7/18/25.
//

#include "stm32f4xx_hal.h"

void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_DAC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_Config = {
        .Mode = GPIO_MODE_ANALOG,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_MEDIUM,
        .Pin = GPIO_PIN_4
    };

    // Init DAC output 1
    HAL_GPIO_Init(GPIOA, &GPIO_Config);
}