#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <Delay.h>
void LED()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//GPIOD
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	while (true)
	{
		GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_RESET);
		Delay_ms(500);
	}
}
void key()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //GPIOB
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //?
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
	
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
	Delay_ms(500);
	GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
	Delay_ms(500);
	
}
int main()
{
	//LED();
	
}
