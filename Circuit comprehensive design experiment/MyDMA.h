#pragma once
#ifndef _MYDMA_H
#define _MYDMA_H
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_adc.h>
#include <stm32f4xx_dac.h>
#include <stm32f4xx_dma.h>
#include <stm32f4xx_it.h>
#include <misc.h>
#include <stdio.h>

//#include <stm32f4xx_dma2d.h>
#include <Delay.h>
#include <Timer.h>
#include <DAC.h>
#define ADC1_DR_ADDRESS     ((uint32_t)0x4001204C)
#define DAC_DHR12R1_ADDRESS  ((uint32_t)0x40007408)

void ADC_Config(void);
void DAC_Config(void);
void DMA_Config(void);
void TIM6_Config(void);

void ADC_DMA_Init(void);
#endif
