#pragma once
#ifndef _LCD_H
#define _LCD_H
#include <stm32f4xx.h>
#include <usart.h>

void vgus_SendData(uint8_t address1 = 0x00, uint8_t address2 = 0x20, uint8_t num1 = 0x00, uint8_t num2 = 0x10);
void vgus_SendDataPic(uint8_t num1 = 0x00, uint8_t num2 = 0x00);
void vgus_SendDataWord(u8 address1, u8 address2, u8 num1);

void vgus_SendArr(uint8_t* arr, uint8_t length);
#endif // !_LCD_H


