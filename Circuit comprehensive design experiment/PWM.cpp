#include <PWM.h>
void PWM_Init(u16 TIM_Prescaler, u32 TIM_Period)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct; 	//定时一个该结构体
	
	//开启 TIM2 和 和 GPIO时钟，配置 PA0选择复用功能 AF0 （TIM2）输出。
	//库函数使能 TIM2 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//还需要使用到GPIOA0，所以也要使能该引脚 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	//由于使用到PA0的复用功能，利用库函数使其引脚与TIM2定时器
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2); 
	
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_0; 		//GPIOA0
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF; 	//配置引脚为复用功能
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_100MHz; 
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP; 	//推挽复用输出
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP; 	//上拉??
	GPIO_Init(GPIOA,&GPIO_InitStruct); 	//初始化 PA0，用库函数寄存器
	
	//频率1000Hz，占空比50%
	TIM_TimeBaseStruct.TIM_Prescaler	= TIM_Prescaler ; 	//设置定时器的分频值 1MHZ，		PSC
	TIM_TimeBaseStruct.TIM_Period		= TIM_Period; 	//设置定时器的的自动重装载的值 周期为500us,		ARR
	TIM_TimeBaseStruct.TIM_CounterMode	= TIM_CounterMode_Up;//设置定时器为向上计数模式
	TIM_TimeBaseStruct.TIM_ClockDivision= TIM_CKD_DIV1;//设置定时器的时钟分频因子
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStruct);//初始化定时器 2
	
	TIM_OCStructInit(&TIM_OCInitStruct);  //给结构体成员赋初始值
	TIM_OCInitStruct.TIM_OCMode			= TIM_OCMode_PWM1; 		//选择PWM的模式，选择PWM模式1
	TIM_OCInitStruct.TIM_OCPolarity 	= TIM_OCPolarity_Low; 	//输出的极性，输出是高电平还是低电平，这里选择低电平
	TIM_OCInitStruct.TIM_OutputState 	= TIM_OutputState_Enable; //用于设置输出的使能，使能PWM输出到端口
	TIM_OCInitStruct.TIM_Pulse = 50;		//CCR，50/100=50%占空比
	//根据设定信息配置TIM2 OC1 -- 通道1
	TIM_OC1Init(TIM2, &TIM_OCInitStruct); 
	
	//使能预装载寄存器：
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
	//使能自动重装载的预装载寄存器允许位	
	TIM_ARRPreloadConfig(TIM2,ENABLE);
 
	//使能定时器。
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}
void PWM_SetPrescaler(uint32_t Prescaler)
{
	TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Update);
}