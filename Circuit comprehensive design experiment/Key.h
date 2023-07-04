#pragma once
#ifndef _KEY_H
#define _KEY_H
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <Delay.h>
void Key_Init();
uint8_t Key_GetNum(void);

#endif // !_KEY_H

