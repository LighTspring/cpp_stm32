#include <experiment.h>
void Program_Controlled_DC_Regulator_Source_play();
void Simple_Signal_Analyzer_play();
void Flame_Detection_play();
int main(void)
{
	//NVIC分组（一个工程当中只能配置一次分组）抢占优先级2位,值范围：0~3；响应优先级2位,值范围：0~3；
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Delay_Init(168); 
	Usart_Init(115200);
	LED_Init();
	while (true)
	{
		LED1_Turn();
		Delay_ms(500);
	}
	//Simple_Signal_Analyzer_play();
	//Program_Controlled_DC_Regulator_Source_play();
	//Flame_Detection_play();
	return 0;
}
void Simple_Signal_Analyzer_play()
{
	Adc_Init();
	Dac1_Init();
	IC_Init();
	u16 adcx;
	u16 tadc;
	u16 max, max1;
	u16 i = 0;
	u16 j = 0;
	//u16 ithred = 2 << 10;
	u16 fre;
	u32 prescaler = 720 - 1;
	while (true)
	{
		assert_param(IS_ADC_ALL_PERIPH(ADC1));
		ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;
		assert_param(IS_ADC_ALL_PERIPH(ADC1));
		adcx = (uint16_t) ADC1->DR;
		DAC_SetChannel1Data(DAC_Align_8b_R, adcx);
		//adcx = Get_Adc_Average(ADC_Channel_5, 5);
		//vgus_SendData84(0x01, adcx >> 8, (u8)adcx);
		tadc = ((float)adcx * 3.3 / (2 << 7) - 1.65) * 1000;
		if (tadc > max) max = tadc;
		if ((i + 1) % 10000 == 0)
		{
			vgus_SendData(0x00, 0x40, max >> 8, (u8)max);
			max = 0;
			i = 0;
			//vgus_SendData(0x00, 0x40, tadc>> 8, (u8)tadc);
			vgus_SendData(0x00, 0x30, 0x00, (u8)(IC_GetDuty() * 100.0 + 0.5));
			fre = (u16)(IC_GetFreq(&prescaler) * 1.2);
			vgus_SendData(0x00, 0x20, fre >> 8, (u8)fre);
			vgus_SendData(0x00, 0x10, 0x00, (u8)(fre / 100));
		}
		i++;
	}
}
void Program_Controlled_DC_Regulator_Source_play()
{
	Program_Controlled_DC_Regulator_Source e3;
	e3.Source_Init();
	e3.update();
}
void Flame_Detection_play()
{
	Flame_Detection f4;
	f4.Flame_Init();
	f4.Flame_update();
}


