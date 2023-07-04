#pragma once
#ifndef _PWM_H
#define _PWM_H
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
void PWM_Init(u16 TIM_Prescaler = 720-1, u32 TIM_Period=100-1);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetPrescaler(uint32_t Prescaler);
#endif
