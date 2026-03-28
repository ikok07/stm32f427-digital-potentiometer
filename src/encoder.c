//
// Created by Kok on 7/17/25.
//

#include <stdio.h>

#include "encoder.h"

#include "error.h"
#include "event_manager.h"
#include "system_config.h"

static volatile uint32_t encoderValue = 0;
static volatile int32_t prevCounter = -1;

/**
 * @brief Setup the encoder with interrupts
 */
HAL_StatusTypeDef ENC_Setup() {
    TIM_Encoder_InitTypeDef encoderConfig = {
        .EncoderMode = TIM_ENCODERMODE_TI1,

        .IC1Filter = 0xF,
        .IC1Polarity = TIM_ENCODERINPUTPOLARITY_RISING,
        .IC1Prescaler = TIM_ICPSC_DIV1,
        .IC1Selection = TIM_ICSELECTION_INDIRECTTI,

        .IC2Filter = 0xF,
        .IC2Polarity = TIM_ENCODERINPUTPOLARITY_RISING,
        .IC2Prescaler = TIM_ICPSC_DIV1,
        .IC2Selection = TIM_ICSELECTION_INDIRECTTI
    };

    HAL_StatusTypeDef status;

    if ((status = HAL_TIM_Encoder_Init(systemConfig.pTIMHandle, &encoderConfig)) != HAL_OK) return status;
    if ((status = HAL_TIM_Encoder_Start_IT(systemConfig.pTIMHandle, TIM_CHANNEL_ALL)) != HAL_OK) return status;

    // Restore encoder value from backup register
    encoderValue = HAL_RTCEx_BKUPRead(systemConfig.pRTCHandle, RTC_BKP_DR0);

    return HAL_OK;
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        uint16_t counter = __HAL_TIM_GET_COUNTER(htim);
        int16_t diff = counter - prevCounter;

        if (diff > (ENCODER_TIM_ARR_VALUE - 1) / 2) diff -= ENCODER_TIM_ARR_VALUE;
        else if (diff < -(ENCODER_TIM_ARR_VALUE / 2)) diff += ENCODER_TIM_ARR_VALUE;

        if (diff > 0 && encoderValue + 1 <= ENCODER_MAX_VALUE) encoderValue++;
        else if (diff < 0 && encoderValue - 1 >= ENCODER_MIN_VALUE) encoderValue--;

        printf("Encoder value: %ld\n", encoderValue);

        // Write to backup register
        HAL_RTCEx_BKUPWrite(systemConfig.pRTCHandle, RTC_BKP_DR0, encoderValue);

        // Update DAC
        EM_HandleNewEncValue(encoderValue);

        prevCounter = counter;
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    encoderValue = 0;
}