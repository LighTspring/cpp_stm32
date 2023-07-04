#pragma once
#ifndef __EXTI_H
#define __EXTI_H
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_dbgmcu.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx_flash_ramfunc.h>
#include <stm32f4xx_it.h>
#include <misc.h>

#include <Delay.h>
#include <LED.h>
#include <Key.h>
#include <usart.h>
void Exti__Init(void);
extern "C" void EXTI0_IRQHandler(void);
#endif
