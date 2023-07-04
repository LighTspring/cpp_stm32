#include <Key.h>
void Key_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //The keys are suitable for pull-up input
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)//开关一端接地，按下后即为低电平
	{
		Delay_ms(20);//防止按下抖动
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0) ;//按键一直按下直到松手否则卡在此处
		Delay_ms(20);//防止松手抖动
		KeyNum = 1;
	}
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)//开关一端接地，按下后即为低电平
	{
		Delay_ms(20);//防止按下抖动
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0) ;//按键一直按下直到松手否则卡在此处
		Delay_ms(20);//防止松手抖动
		KeyNum = 2;
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)//开关一端接地，按下后即为低电平
	{
		Delay_ms(20);//防止按下抖动
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1) ;//按键一直按下直到松手否则卡在此处
		Delay_ms(20);//防止松手抖动
		KeyNum = 3;
	}
	return KeyNum;
	
}