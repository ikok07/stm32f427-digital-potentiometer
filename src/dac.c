//
// Created by Kok on 7/18/25.
//

#include "dac.h"

#include "system_config.h"

/**
 * @brief Configures the DAC1 peripheral
 */
HAL_StatusTypeDef DAC_Setup() {
    HAL_StatusTypeDef status = HAL_OK;

    // Initialize DAC
    if ((status = HAL_DAC_Init(systemConfig.pDACHandle)) != HAL_OK) return status;

    DAC_ChannelConfTypeDef Channel_Config = {
        .DAC_Trigger = DAC_TRIGGER_SOFTWARE,
        .DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE
    };

    // Configure DAC channel 1
    if ((status = HAL_DAC_ConfigChannel(systemConfig.pDACHandle, &Channel_Config, DAC_CHANNEL_1)) != HAL_OK) return status;

    // Start DAC1
    if ((status = HAL_DAC_Start(systemConfig.pDACHandle, DAC_CHANNEL_1)) != HAL_OK) return status;

    return status;
}
