#include <InputCapture.h>
uint16_t c = 0;
void IC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3); 
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_6; //TIM3_CH1
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//72M/PSC 是计数器自增频率，即为标准频率fc
	TIM_TimeBaseStruct.TIM_Prescaler	= 720 - 1; //设置定时器的分频值 1MHZ，		PSC  （测频法，测周法）
	TIM_TimeBaseStruct.TIM_Period		= 65536 - 1; //设置定时器的的自动重装载的值,满量程防止溢出		ARR
	TIM_TimeBaseStruct.TIM_CounterMode	= TIM_CounterMode_Up; //设置定时器为向上计数模式
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1; //设置定时器的时钟分频因子
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStruct); //初始化定时器 2
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0xF;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	//TIM_ICInit(TIM3, &TIM_ICInitStruct);
	TIM_PWMIConfig(TIM3, &TIM_ICInitStruct);//在配置channel_1的同时相反的配置channel_2,附加测量占空比，
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
//	NVIC_InitTypeDef NVIC_InitStructure;	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //定时器3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //允许定时器3更新中断
//	
//	
	TIM_Cmd(TIM3, ENABLE);
	
//	c = 0;
	
}
void SetPrescaler(uint32_t Prescaler)
{
	TIM_PrescalerConfig(TIM3, Prescaler, TIM_PSCReloadMode_Update);
}
float IC_GetFreq(uint32_t* prescaler)
{
	*prescaler = 720-1;
	SetPrescaler(*prescaler);
	uint16_t temp;
	uint16_t res;
	uint16_t tc = c;
	temp = TIM_GetCapture1(TIM3);
	//Delay_ms(10);
	while (true)
	{
//		if (temp > 8000)
//		{
//			TIM_SetCounter(TIM3, 0);
//			*prescaler = (*prescaler + 1) * 100;
//			SetPrescaler(*prescaler);
//			temp = TIM_GetCapture1(TIM3);
//		}
//		else if (temp < 99)
//		{
//			TIM_SetCounter(TIM3, 0);
//			*prescaler = (*prescaler + 1) /100;
//			SetPrescaler(*prescaler);
//			temp = TIM_GetCapture1(TIM3);
//			if (*prescaler == 0 && temp == 0)
//			{
//				return 0.0;
//			}
//		}
//		else
//		{
//			res = temp;
//			break;
//		}
		temp = TIM_GetCapture1(TIM3);
		res = temp;
		break;
	}
	
	return 72000000.0/(float)(*prescaler+1)/(res+1);//fc/N	N=CCR	CCR通过capture_value读取
}

float IC_GetDuty(void)
{
	uint16_t rise = TIM_GetCapture1(TIM3) + 1;
	uint16_t fall = TIM_GetCapture2(TIM3) + 1;
	//当一个周期内计数点数较小时，应当多计几个周期。
	return (float)fall / (float)rise;
}

//void TIM3_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
//	{
//		//Serial_sendNumber(c);
//		c++;
//		LED2_Turn();
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位	
//	}
//}