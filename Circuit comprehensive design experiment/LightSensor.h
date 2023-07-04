#pragma once
#ifndef _LIGHT_SENSOR_H
#define _LIGHT_SENSOR_H
#include <stm32f4xx.h>

void LightSensor_Init(void);
uint8_t LightSensor_Get(void);


#endif // !_LIGHT_SENSOR_H
