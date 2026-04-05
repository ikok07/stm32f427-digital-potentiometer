//
// Created by Kok on 7/17/25.
//

#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define ERROR_LED_PORT              GPIOC
#define ERROR_LED_PIN               GPIO_PIN_0

void ERR_Setup();

void ERR_Trigger(char *msgFormat, ...);
void ERR_Clear();
uint8_t CheckForError();

#endif //ERROR_H
