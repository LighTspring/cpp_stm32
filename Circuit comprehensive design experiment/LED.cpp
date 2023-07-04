#include <LED.h>
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//???不能影响引脚电平
	/*如果是output，那个一般选择no pull,这样，引脚才能根据你的output数据，进行正确输出。
	如果是input，那么需要看具体应用的默认输入值是0还是1.  如果默认是输入0，则最好配置为pull down，反之则配置为pull up.*/
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);//置为高电平
	
}

void LED1_ON(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_9);
}

void LED1_OFF(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_9);	
}

void LED1_Turn(void)
{
	uint16_t flag = GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_9);
	if (flag == 0)
	{
		GPIO_SetBits(GPIOF, GPIO_Pin_9);
	}
	else 
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_9);
	}
}

void LED2_ON(void)
{
	GPIO_ResetBits(GPIOF, GPIO_Pin_10);	
}

void LED2_OFF(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_10);	
}

void LED2_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_10) == 0)
	{
		GPIO_SetBits(GPIOF, GPIO_Pin_10);
	}
	else 
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_10);
	}
}

