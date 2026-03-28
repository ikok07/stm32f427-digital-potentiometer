//
// Created by Kok on 7/17/25.
//

#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx_hal.h"

#define ENCODER_TIM_ARR_VALUE               65536
#define ENCODER_MIN_VALUE                   0
#define ENCODER_MAX_VALUE                   4096

extern uint32_t EncoderValue;

HAL_StatusTypeDef ENC_Setup();

#endif //ENCODER_H
