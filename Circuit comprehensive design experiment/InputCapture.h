#pragma once
#ifndef _INPUTCAPTURE_H
#define _INPUTCAPTURE_H
#include <stm32f4xx.h>
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <misc.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_tim.h>

#include <Delay.h>
#include <LED.h>
#include <usart.h>
extern uint16_t c;
void IC_Init(void);
void SetPrescaler(uint16_t Prescaler);
float IC_GetFreq(uint32_t* prescaler);
float IC_GetDuty(void);
//extern "C" void TIM3_IRQHandler(void);
#endif
