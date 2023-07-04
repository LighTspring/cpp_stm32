#pragma once
#ifndef __DELAY_H
#define __DELAY_H
#include <misc.h>
#include <stm32f4xx.h>
#include <sys.h>
void Delay_Init(u8 SYSCLK);
void Delay_s(u16 ns);
void Delay_ms(u16 nms);
void Delay_us(u32 nus);

#endif
