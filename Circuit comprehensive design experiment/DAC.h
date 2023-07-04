#pragma once
#ifndef _DAC_H
#define _DAC_H
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <misc.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_dac.h>

#include <Delay.h>
#include <LED.h>
void Dac1_Init(void); //DAC通道1初始化
void Dac1_Set_Vol(float vol); //设置通道1输出电压
#endif
