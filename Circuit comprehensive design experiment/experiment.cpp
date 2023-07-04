#include <experiment.h>
Amplifier_Nonlinear_Distortion::Amplifier_Nonlinear_Distortion()
{
}

Amplifier_Nonlinear_Distortion::~Amplifier_Nonlinear_Distortion()
{
}
void Amplifier_Nonlinear_Distortion::shift_i()
{
	i++;
	if (i == 4)
	{
		i = 0;
	}
}
void Amplifier_Nonlinear_Distortion::Exp1_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	/*如果是output，那个一般选择no pull,这样，引脚才能根据你的output数据，进行正确输出。
	如果是input，那么需要看具体应用的默认输入值是0还是1.  如果默认是输入0，则最好配置为pull down，反之则配置为pull up.*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//010正常波形
	GPIO_SetBits(GPIOB, GPIO_Pin_5); 
	GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_7);
}
void Amplifier_Nonlinear_Distortion::key_exit_Init()
{
	NVIC_InitTypeDef  NVIC_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; //The keys are suitable for pull-up input
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_8 | GPIO_Pin_9);
	
	//使能SYSCFG时钟： 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //使能SYSCFG时钟，相当于重映射
	//设置IO口与中断线的映射关系。确定什么引脚对应哪个中断线
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);

	EXTI_InitStruct.EXTI_Line = EXTI_Line8 | EXTI_Line9;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //中断模式，或者事件模式，Event
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn; //NVIC通道，在stm32f4xx.h可查看通道 （可变）
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00; //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x02; //响应优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; //使能
	NVIC_Init(&NVIC_InitStruct);
}
extern "C" void Amplifier_Nonlinear_Distortion::EXTI9_5_IRQHandler(void)
{
	uint16_t num = 0;
	uint16_t numth = 500;
	uint16_t down = 130;
	if (EXTI_GetITStatus(EXTI_Line8) == SET)
	{
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
		{
			num++;
			Delay_ms(1);
			if (num >= numth)
			{
				//Serial_SendString("Turn mode");
				mode = !mode;
				break;
			}
		}
		//Serial_SendString("num: ");
		//Serial_sendNumber(num);
		if (num < numth && num > down && mode == 1) {
			switch (i % 4)
			{
			case 0:
				waves(3, 0, 1, 0);
				break;
			case 1:
				waves(3, 1, 0, 0);
				break;
			case 2:
				waves(3, 0, 0, 0);
				break;
			case 3:
				waves(3, 0, 1, 1);
				break;
			default:break;
			}
			shift_i();
		}//手动模式并且点按
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if (EXTI_GetITStatus(EXTI_Line9) == SET)
	{
		
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0)
		{
			num++;
			Delay_ms(1);
			if (num >= down)
			{
				break;
			}
		}
		if (num >= down)
		{
			if (mode == 1)
			{
				switch (i % 4)
				{
				case 3:
					waves(3, 0, 1, 0);
					break;
				case 2:
					waves(3, 1, 0, 0);
					break;
				case 1:
					waves(3, 0, 0, 0);
					break;
				case 0:
					waves(3, 0, 1, 1);
					break;
				default:break;
				}
				shift_i();
			}
			EXTI_ClearITPendingBit(EXTI_Line9);
		}
	}
}
void Amplifier_Nonlinear_Distortion::exp1(uint16_t s)
{
	while (true)
	{
		if (mode == 0)
		{
			waves(3, 0, 1, 0);
			shift_i();
			Delay_s(s);
			waves(3, 1, 0, 0);
			shift_i();
			Delay_s(s);
			waves(3, 0, 0, 0);
			shift_i();
			Delay_s(s);
			waves(3, 0, 1, 1);
			shift_i();
			Delay_s(s);
		}
	}
}

void Amplifier_Nonlinear_Distortion::waves(int count = 3, ...)
{
	va_list args;
	va_start(args, count);
	//	Serial_SendString("mode=");
	//	Serial_sendOneNumber(mode);
	//	Serial_SendString("i=");
	//	Serial_sendOneNumber(i);
	
	for (int j = 0; j < count; j++)
	{
		int arg = va_arg(args, int);
		if (arg == 0)
		{
			GPIO_ResetBits(GPIOB, pins[j]);
		}
		else if (arg == 1)
		{
			GPIO_SetBits(GPIOB, pins[j]);
		}
	}
}
Simple_Signal_Analyzer::Simple_Signal_Analyzer()
{
}

Simple_Signal_Analyzer::~Simple_Signal_Analyzer()
{
}
void Simple_Signal_Analyzer::generate_pwm(u8 duty)//PA0
{
	PWM_Init();
	PWM_SetCompare1(duty); //Duty=CCR/(ARR+1)=CCR/100
	PWM_SetPrescaler(720 - 1); //Freq=72M/(PSC+1)/100
}

void Simple_Signal_Analyzer::generate_waves(u8 type, u16 delayus)
{
	//{0:sin, 1 : triangular, 2 : square}
	u16 width = 512;
	u16 j = 0;
	u16 vol;
	u16* wave = 0;
	while (true)
	{
		switch (type)
		{
		case 0:wave = MyWaves(wave, 0, 12, 9); break;
		case 1:wave = MyWaves(wave, 1, 12, 9); break;
		case 2:wave = MyWaves(wave, 2, 12, 9); break;
		}
		for (j = 0; j < width; j++)
		{
			DAC_SetChannel1Data(DAC_Align_12b_R, *wave++);
			Delay_us(delayus);
			//			Serial_sendNumber(*wave++);
			//			Serial_SendString("END");
		}
	}
}

void Simple_Signal_Analyzer::exp2_Init(void)
{
	//	Adc_Init();
	//	PWM_Init();
	//	PWM_SetCompare1(20); //Duty=CCR/(ARR+1)=CCR/100
	//	PWM_SetPrescaler(720 - 1); //Freq=72M/(PSC+1)/100,最高只能产生10kHz的波形，
	IC_Init();
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_SetCounter(TIM3, 0);
}
Program_Controlled_DC_Regulator_Source::Program_Controlled_DC_Regulator_Source()
{
}

Program_Controlled_DC_Regulator_Source::~Program_Controlled_DC_Regulator_Source()
{
}
void Program_Controlled_DC_Regulator_Source::Source_Init() const
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	//如果是input，那么需要看具体应用的默认输入值是0还是1.  如果默认是输入0，则最好配置为pull down，反之则配置为pull up.
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1); //输出低电平默认工作。
	Adc_Init(); //PA5
	Dac1_Init(); //PA4
	DAC_SetChannel1Data(DAC_Align_12b_R, uint16_t(Init_davol / 3.3 * 4096)); //设置初始输出电压
	Timer_Init(60000 - 1, 36000 - 1);//自动重装值60000，以2kHz计数，每30s进一次中断，由于中断函数不能定义在类中，因此对每次的计数进行判断，自动置零
}
//extern "C" void Program_Controlled_DC_Regulator_Source::TIM3_IRQHandler(void)
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
//	{
//		LED2_Turn();
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位	
//	}
//}
#ifdef __cplusplus
extern "C" {
#endif
	void TIM3_IRQHandler(void)
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //溢出中断
		{
			LED2_Turn();
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除中断标志位
			
		}
	}
#ifdef __cplusplus
}
#endif

void Program_Controlled_DC_Regulator_Source::enable_turn()//设置一个使能按键，点击一下电源是否工作的状态翻转一次
{
	switch (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1))
	{
	case 0:
		GPIO_SetBits(GPIOB, GPIO_Pin_1);
		mode = false;
		time = 0.0;
		now_count = 0;
		last_count = 0;
		TIM_SetCounter(TIM3, 0);
		TIM_Cmd(TIM3, DISABLE);
		break;
	case 1:
		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
		mode = true;
		TIM_Cmd(TIM3, ENABLE);
		TIM_SetCounter(TIM3, 0);
		time = 0.0;
		now_count = 0;
		last_count = 0;
		break;
	}
}
void Program_Controlled_DC_Regulator_Source::step(foward_back fb)//V_{out_the}=k\times V_{DA}+b,V_{out_real}=k1\times V_{out_the}+b1
{
	now_davol_the += step_vol / k*fb;
	now_davol += step_vol / (k*k1)*fb;
	now_volout = k*now_davol_the + b;
	DAC_SetChannel1Data(DAC_Align_12b_R, uint16_t(now_davol * 4096 / 3.3));
}
void Program_Controlled_DC_Regulator_Source::update()
{
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_SetCounter(TIM3, 0); 
	u16* data = 0;
	u16 adcx = 0; //测量输出电压
	while (true)
	{
//		assert_param(IS_ADC_ALL_PERIPH(ADC1));
//		ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;
//		assert_param(IS_ADC_ALL_PERIPH(ADC1));
//		adcx = (u16) ADC1->DR; //随后经过简单运算，得到采样电压
//		sampling_vol = adcx * 3.3 / 4096;
//		sampling_curr = sampling_vol / Sampling_Resistance;
		data = usart_receive();
		if (mode)
		{
			now_count = TIM_GetCounter(TIM3);
			time += 4e-4*(now_count - last_count);
			last_count = now_count;
			if (now_count >= 50000)
			{
				TIM_SetCounter(TIM3, 0);
				last_count = 0;
				now_count = 0;
			}
			if (data)//非空时
			{
				switch (*(data + 5))
				{
				case enable_addr:
					enable_turn();
					break;
				case up_addr:
					step(foward);
					break;
				case down_addr:
					step(back);
					break;
				default:
					break;
				}
			}
			vgus_SendData(0x00, vol_addr, uint16_t(now_volout * 1000)>>8, uint8_t(now_volout * 1000));
//			vgus_SendData(0x00, sam_curr_addr, uint16_t(sampling_curr * 1000) >> 8, uint8_t(sampling_curr * 1000));
//			vgus_SendData(0x00, sam_vol_addr, uint16_t(sampling_vol * 1000) >> 8, uint8_t(sampling_vol * 1000));
//			vgus_SendData(0x00, curr_5_addr, uint16_t(now_volout / Load_Resistances[0] * 1000) >> 8, uint8_t(now_volout / Load_Resistances[0] * 1000));
//			vgus_SendData(0x00, curr_100_addr, uint16_t(now_volout / Load_Resistances[1] * 1000) >> 8, uint8_t(now_volout / Load_Resistances[1] * 1000));	
		}
		else
		{
			vgus_SendData(0x00, vol_addr, 0x00, 0x00);
//			vgus_SendData(0x00, sam_curr_addr, 0x00, 0x00);
//			vgus_SendData(0x00, curr_5_addr, 0x00, 0x00);
//			vgus_SendData(0x00, curr_100_addr, 0x00, 0x00);
			if (data && *(data + 5) == enable_addr)enable_turn();
		}
		//Serial_sendFloat(time, 4);
		vgus_SendData(0x00, vol_time_addr, uint16_t(time * 100) >> 8, uint8_t(time * 100));
		//LED1_Turn();
		//Delay_ms(110);
	}
}

Flame_Detection::Flame_Detection()
{
}

Flame_Detection::~Flame_Detection()
{
}

void Flame_Detection::Flame_Init()
{
	Adc_Init();//PA5
	Dac1_Init();//PA4
	if (real)PWM_Init(8400 - 1,100-1); //100Hz,PA0
	else DAC_SetChannel1Data(DAC_Align_12b_R, Square_Amplitude);//或者单片机输出直流
}
void Flame_Detection::Flame_update()
{
	u16 adcx;
	double vol_adcx;
	while (true)
	{
		assert_param(IS_ADC_ALL_PERIPH(ADC1));
		ADC1->CR2 |= (uint32_t)ADC_CR2_SWSTART;
		assert_param(IS_ADC_ALL_PERIPH(ADC1));
		adcx = (u16) ADC1->DR;
		const double k = 0.06191085;
		const double b = 0.0097665;
		vol_adcx = adcx * 3.3 / 4096 * k + b;
		//adcx = Get_Adc_Average(1, 5);
//		if (real)
//		{
//			r0 = c2_real / vol_adcx - c1_real * 100;
//			vout = vol_adcx * 1000;
//		}
//		else
//		{
//			r0 = R0(adcx) * 10*bits;
//			vout = Vout(adcx) * 10*bits;
//		}
		
		r0 = R0_real(vol_adcx);
		vout = vol_adcx * 1000;
		vgus_SendData(0x00, r0addr, uint16_t(r0*100) >> 8, uint8_t(r0*100)); //发送此时测得的电阻
		vgus_SendData(0x00, voutaddr, uint16_t(vout) >> 8, uint8_t(vout)); //发送此时的输出电压
		
		
		switch (u8(r0+0.5))
		{
		case 1:
			vgus_SendDataPic(0x00, 0x01);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x0b);
			break;
		case 2:
			vgus_SendDataPic(0x00, 0x02);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x0c);
			break;
		case 3:
			vgus_SendDataPic(0x00, 0x03);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x0d);
			break;
		case 4:
			vgus_SendDataPic(0x00, 0x04);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x0e);
			break;
		case 5:
			vgus_SendDataPic(0x00, 0x05);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x0f);
			break;
		case 6:
			vgus_SendDataPic(0x00, 0x06);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x10);
			break;
		case 7:
			vgus_SendDataPic(0x00, 0x07);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x11);
			break;
		case 8:
			vgus_SendDataPic(0x00, 0x08);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x12);
			break;
		case 9:
			vgus_SendDataPic(0x00, 0x09);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x13);
			break;
		case 10:
			vgus_SendDataPic(0x00, 0x0a);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x14);
			break;
		default:
			vgus_SendDataPic(0x00, 0x0a);
			Delay_ms(50);
			vgus_SendDataPic(0x00, 0x14);
			break;
		}
		Delay_ms(25);
	}
}