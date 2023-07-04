#include <stm32f4xx.h>
#include <Delay.h>
#include <Buzzer.h>
#include <LightSensor.h>

int main(void)
{
	Buzzer_Init();
	LightSensor_Init();
	while (true)
	{
		if (LightSensor_Get() == 1)
		{
			Buzzer_ON();
		}
		else
		{
			Buzzer_OFF();
		}
	}
	
}