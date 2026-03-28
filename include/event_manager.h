//
// Created by Kok on 1/12/26.
//

#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

HAL_StatusTypeDef EM_HandleNewEncValue(uint16_t value);

#endif //EVENT_MANAGER_H
