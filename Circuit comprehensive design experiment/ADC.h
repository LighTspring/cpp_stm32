#pragma once
#ifndef _ADC_H
#define _ADC_H
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_adc.h>
#include <stm32f4xx_it.h>
#include <misc.h>
#include <stdio.h>

#include <Delay.h>

void  Adc_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch, u8 times);

#endif
