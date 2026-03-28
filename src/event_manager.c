//
// Created by Kok on 1/12/26.
//

#include "event_manager.h"

#include "error.h"
#include "system_config.h"
#include "stm32f4xx_hal.h"

HAL_StatusTypeDef EM_HandleNewEncValue(uint16_t value) {
    if (HAL_DAC_SetValue(systemConfig.pDACHandle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value & 0xFFF) != HAL_OK) {
        ERR_Trigger("Failed to update DAC value!");
        HAL_Delay(2000);
        NVIC_SystemReset();
    };

    return HAL_OK;
}
