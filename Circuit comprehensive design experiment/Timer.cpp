#include <Timer.h>

void Timer_Init(u16 Period_arr,u16 Prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	TIM_InternalClockConfig(TIM3);
	
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter(); //与PWM有关，note This parameter is valid only for TIM1 and TIM8
	//1s定时，定时频率=72M/(per+1)/(presc+1)
	TIM_TimeBaseInitStructure.TIM_Period = Period_arr; 	//自动重装载值，0~65355
	TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler;  //定时器分频，0~65355
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //时钟分频，与采样频率有关，和时基单元关系不大
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void Timer_Init_ETR()  //外部时钟，还需要配置GPIO
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //外部信号输入引脚
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;//推荐使用浮空输入，但是害怕引脚电平跳动，当输入信号较小时用浮空
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed;
	
	//TIM_TimeBaseInitStructure.TIM_RepetitionCounter(); //与PWM有关，note This parameter is valid only for TIM1 and TIM8
	//1s定时，定时频率=72M/(per+1)/(presc+1)
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; //自动重装载值，0~65355
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; //定时器分频，0~65355
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //时钟分频，与采样频率有关，和时基单元关系不大
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t num = 0;
uint16_t get_num()
{
	return num;
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
	{
		num++;
		Serial_sendNumber(num);
		LED2_Turn();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位	
	}
}