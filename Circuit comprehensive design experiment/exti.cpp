#include <exti.h>
//编写中断服务函数。这个函数不需要程序员在主函数调用，满足条件CPU自行调用的函数
void EXTI0_IRQHandler(void) //在startup_stm32f407xx.c 里找到固定的函数名;不需要声明
{
	//Serial_SendString("go into exit");
	//判断中断标志是否为1
	if (EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		//Serial_SendString("exti_line0_set");
	}
	
	Delay_ms(10); //延时消抖
	LED1_Turn();
	//清空标志位
	EXTI_ClearITPendingBit(EXTI_Line0);
}

void Exti__Init(void)
{
	NVIC_InitTypeDef  NVIC_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	
	Key_Init();
	
	//使能SYSCFG时钟： 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //使能SYSCFG时钟，相当于重映射
	//设置IO口与中断线的映射关系。确定什么引脚对应哪个中断线 PA0 -- EXTI0 （可变）
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式，或者事件模式，Event
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发或者Falling,Rising_Falling
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; //NVIC通道，在stm32f4xx.h可查看通道 （可变）
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 0x00; //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02; //响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //使能
	//配置中断分组（NVIC），并使能中断。
	NVIC_Init(&NVIC_InitStruct);
}

