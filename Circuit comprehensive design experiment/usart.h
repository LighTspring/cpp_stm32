#pragma once
#ifndef _USART_H
#define _USART_H
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_it.h>
#include <misc.h>
#include <stdio.h>
#include <string>
#include <iostream>

#include <Delay.h>
#include <LED.h>
#define USART_REC_LEN  			200  	//最大接收字节数
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
void Usart_Init(u32 bound,u8 foot = 9);//串口2有问题?
void Serial_SendByte(uint8_t Byte, u8 foot = 9);
void Serial_SendArray(uint8_t *Array);
void Serial_SendString(char *String);
void Serial_sendOneNumber(uint8_t OneNumber);
void Serial_sendNumber(uint32_t Number);
void Serial_sendPacket(uint8_t *array, uint8_t start, uint8_t end);
void Serial_sendFloat(float number, uint8_t precision);
int fputc(int ch, FILE *f);
extern "C" void USART1_IRQHandler(void);
extern "C" void USART2_IRQHandler(void);
u16* usart_receive();
#endif
