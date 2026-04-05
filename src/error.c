//
// Created by Kok on 7/17/25.
//

#include "error.h"

#include <stdio.h>
#include <stdarg.h>

void ERR_Setup() {
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Setup error LED GPIO
    GPIO_InitTypeDef GPIO_Config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Pin = ERROR_LED_PIN
    };

    HAL_GPIO_Init(ERROR_LED_PORT, &GPIO_Config);
}

/**
 * @brief Triggers an error which enables the error LED and sends message via USART
 * @param msgFormat Message to send via USART
 */
void ERR_Trigger(char *msgFormat, ...) {
    HAL_GPIO_WritePin(ERROR_LED_PORT, ERROR_LED_PIN, GPIO_PIN_SET);

    if (msgFormat == NULL) return;

    // Send message
    va_list args;
    va_start(args, msgFormat);

    vprintf(msgFormat, args);

    va_end(args);
}

/**
 * @brief Clears the currently active error if one exists
 */
void ERR_Clear() {
    HAL_GPIO_WritePin(ERROR_LED_PORT, ERROR_LED_PIN, GPIO_PIN_RESET);
    printf("Active error cleared\n");
}
