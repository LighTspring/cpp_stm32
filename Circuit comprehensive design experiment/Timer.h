#pragma once
#ifndef _TIMER_H
#define _TIMER_H
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
uint16_t get_num();
void Timer_Init(u16 Period_arr = 10000 - 1, u16 Prescaler = 7200 - 1);
void Timer_Init_ETR();
//extern "C" void TIM3_IRQHandler(void);
#endif
