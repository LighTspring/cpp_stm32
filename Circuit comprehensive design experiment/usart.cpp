#include <usart.h>
u16 USART_RX_STA = 0; //接收状态位置
u16 USART_RX_BUF[USART_REC_LEN]; //接受缓冲
//重定义printf函数
int fputc(int ch, FILE *f)
{ 	
	while ((USART1->SR & 0X40) == 0) ;//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
void Usart_Init(u32 bound,u8 foot) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	if (foot == 9)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //PA9:U1_TX
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //PA10:U1_RX

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //流控
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART1, &USART_InitStructure);
		USART_Cmd(USART1, ENABLE);
	}
	else if (foot == 2)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //PA2:U2_TX
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //PA3:U2_RX

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //流控
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART2, &USART_InitStructure);
		USART_Cmd(USART2, ENABLE);
	}

	//USART_ClearFlag(USART1, USART_FLAG_TC);

#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器、

#endif
	
}

void Serial_SendByte(uint8_t Byte,u8 foot)
{
	if (foot == 9)
	{
		USART_SendData(USART1, Byte);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) ;
	}
	if (foot == 2)
	{
		USART_SendData(USART2, Byte);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) ;
	}

}
uint16_t get_arr_length(uint8_t *arr)//适用于以'\0'结尾的数组
{
	uint16_t len = 0;
	while (true)
	{
		if (arr[len] == '\0')break;
		len++;
	}
	return len;
}
void Serial_SendArray(uint8_t *Array)
{
	uint16_t i;
	uint16_t Length = get_arr_length(Array);
	for (i = 0; i < Length; i++)
	{
		Serial_SendByte(Array[i]);
	}
}
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] !='\0'; i++)
	{
		Serial_SendByte(String[i]);
	}
}
uint32_t Serial_Pow(uint32_t x, uint32_t y)
{
	uint32_t Result = 1;
	while (y--)
	{
		Result *= x;	
	}
	return Result;
}

uint8_t get_length(uint32_t x)
{
	int leng = 0;
	if (x == 0)return 1;
	while (x)
	{
		x /= 10;
		leng++;
	}
	return leng;
}
void Serial_sendOneNumber(uint8_t OneNumber)
{
	Serial_SendByte(OneNumber + '0');
}
void Serial_sendNumber(uint32_t Number)
{
	uint8_t i;
	uint8_t Length = get_length(Number);
	for (i = 0; i < Length; i++)Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
}
void Serial_sendPacket(uint8_t *array, uint8_t start, uint8_t end)
{
	Serial_SendByte(start);
	Serial_SendArray(array);
	Serial_SendByte(end);
}
void Serial_sendFloat(float number,uint8_t precision=5)
{
	//precision是保留的小数位数
	uint8_t i;
	uint32_t num;
	uint32_t integ = number;//整数部分
	num = number*Serial_Pow(10, precision);//浮点数给整型赋值，略去小数部分
	uint8_t Length = get_length(num);
	uint8_t list[20] = {};
	for (i = 0; i < Length; i++) list[i] = num / Serial_Pow(10, Length - i - 1) % 10;
	Serial_sendNumber(integ);//发送整数部分
	Serial_SendByte('.');//发送小数点
	for (i = 0; i < precision - Length; i++) Serial_SendByte('0');//发送小数点后可能存在的0
	for (i = 0; i < Length; i++) Serial_SendByte(list[i] + '0');//发送小数部分的非0部分
}




void USART1_IRQHandler(void)
{
//	u8 Res;
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//	{
//		Res = USART_ReceiveData(USART1);//读取接收到的数据
//		
//		if ((USART_RX_STA & 0x8000) == 0)//接收未完成
//		{
//			if (USART_RX_STA & 0x4000)//接收到了0x0d
//			{
//				if (Res != 0x0a)USART_RX_STA = 0;//接收错误,重新开始
//				else USART_RX_STA |= 0x8000; //接收完成了 
//			}
//			else //还没收到0X0D
//			{	
//				if (Res == 0x0d)USART_RX_STA |= 0x4000;
//				else
//				{
//					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
//					USART_RX_STA++;
//					if (USART_RX_STA > (USART_REC_LEN - 1))USART_RX_STA = 0; //接收数据错误,重新开始接收
//				}
//			}
//		}
//	}
	u8 Res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART1);
		//接收以0x0d 0x0a结束的数据
		if ((USART_RX_STA & 0x8000) == 0)//接收未完成
		{
			if (USART_RX_STA & 0x4000)//接收到了0x0d
			{
				if (Res != 0x0a)USART_RX_STA = 0;
				else USART_RX_STA |= 0x8000; //接收完成了 
			}
			else //还没收到0X0D
			{	
				if (Res == 0x0d)USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))USART_RX_STA = 0; //接收回车符,重新开始接收
				}
			}
		}
	}
}
void USART2_IRQHandler(void)
{
	u8 Res;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res = USART_ReceiveData(USART2);
		//接收以0x0d 0x0a结束的数据
		if ((USART_RX_STA & 0x8000) == 0)//接收未完成
		{
			if (USART_RX_STA & 0x4000)//接收到了0x0d
			{
				if (Res != 0x0a)USART_RX_STA = 0;
				else USART_RX_STA |= 0x8000; //接收完成了 
			}
			else //还没收到0X0D
			{	
				if (Res == 0x0d)USART_RX_STA |= 0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
					USART_RX_STA++;
					if (USART_RX_STA > (USART_REC_LEN - 1))USART_RX_STA = 0; //接收回车符,重新开始接收
				}
			}
		}
	}
}

u16* usart_receive()
{
	u8 len;
	u16 times = 0;
	u16* arr = 0;
	static u16 BUF[USART_REC_LEN];
	if (USART_RX_STA & 0x8000)
	{					   
		len = USART_RX_STA & 0x3fff; //得到此次接收到的数据长度
		for (u8 t = 0; t < len; t++)
		{
			//USART_SendData(USART1, USART_RX_BUF[t]); //向串口1发送数据
			//while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET) ;//等待发送结束
			BUF[t] = USART_RX_BUF[t];
		}
		USART_RX_STA = 0;
		arr = BUF;
	}
	return arr;
}