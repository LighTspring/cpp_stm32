#pragma once
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_tim.h>
#include <system_stm32f4xx.h>
#include <stm32f4xx_it.h>
#include <misc.h>
#include <cstdarg>

#include <Delay.h>
#include <DAC.h>
#include <ADC.h>
#include <usart.h>
#include <Lcd.h>
#include <PWM.h>
#include <InputCapture.h>
#include <Timer.h>
#include <MyWave.h>
class MyClass {
public:
	MyClass();
	void myMethod();
};

class Amplifier_Nonlinear_Distortion
{

public:
	Amplifier_Nonlinear_Distortion();
	~Amplifier_Nonlinear_Distortion();
	void Exp1_Init();
	void key_exit_Init();
	void EXTI9_5_IRQHandler(void);
	void exp1(uint16_t s);
	void waves(int count, ...);
private:
	uint8_t mode = 0; //默认自动
	uint8_t i = 0; //波形
	uint16_t pins[3] = { GPIO_Pin_1, GPIO_Pin_5, GPIO_Pin_7 };
	//010 100 000 011
	//PB8控制SW1,K1,PB9控制SW2,K0
	void shift_i();
};

class Simple_Signal_Analyzer
{
public:
	Simple_Signal_Analyzer();
	~Simple_Signal_Analyzer();
	void generate_pwm(u8 duty);
	void generate_waves(u8 type = 0, u16 delayus = 500);
	void exp2_Init(void);
private:
					
};

class Program_Controlled_DC_Regulator_Source
{
private:
	const double k = -3.1;
	const double b = 8.856;
	const double k1 = 0.8445;
	const double b1 = 1.008909;
	const double step_vol = 0.1;
	//const double Init_davol = (b-5) / k;
	const double Init_davol = 1.24387;
	//const double Init_davol_real = 1.23 + step_vol / (k*k1);
	const double Init_davol_real = 1.35;//同上
	const u16 times = 1000;
	const double Sampling_Resistance = 0.2;
	u8 Load_Resistances[2] = { 5, 100 };
	double now_davol = Init_davol_real;
	double now_davol_the = Init_davol;
	double now_volout = 5;
	double sampling_vol = 0;
	double sampling_curr = sampling_vol / Sampling_Resistance;
	bool mode = true;
	double now_current = now_volout / Sampling_Resistance;
	u16 now_count = 0;
	u16 last_count = 0;
	double time = 0.0;
	enum foward_back
	{
		foward = 1, 
		back   = -1
	};
	enum addr
	{
		vol_addr      = 0x10,
		enable_addr   = 0x30,
		up_addr       = 0x40,
		down_addr     = 0x50,
		vol_time_addr = 0x60,
		sam_vol_addr  = 0x70,
		sam_curr_addr = 0x80,
		curr_5_addr   = 0x85,
		curr_100_addr = 0x90,
	};
	//头文件定义短小函数称为内联函数
//	void f()
//	{
//		std::cout << std::endl;
//	}
public:
	Program_Controlled_DC_Regulator_Source();
	~Program_Controlled_DC_Regulator_Source();
	void Source_Init() const;//只要类方法不修改调用对象，就应将其声明为const
	void enable_turn();
	void step(foward_back fb);
	void update();
	//void TIM3_IRQHandler(void);//在类中不能声明extern "C"
	
};

class Flame_Detection
{
public:
	Flame_Detection();
	~Flame_Detection();
	void Flame_Init();
	void Flame_update();
private:
	const bool real = true;
	const u16 Square_Amplitude = 4095;
	const double Rmax = 10;
	const double Rmin = 0.5;
	const double R2 = 2;
	const double voutmax = 1.6744;
	const double voutmin = 0.089;
	const double vin = 3.3;
	const double c1 = (voutmin*Rmax - voutmax*Rmin) / (voutmax - voutmin);
	const double c2 = (Rmin + c1)*voutmax;
	inline double vol(u16 adcx)const{return adcx * 3.3 / 4096;}
	inline double R0(u16 adcx){return vol(adcx)*R2 / (vin - vol(adcx));}
	inline double Vout(u16 adcx){return c2 / (c1 + R0(adcx));}
	//[ 1.49444087  0.04226561  0.12245056 -0.0511651 ]
	const double n1 = 1.49444087;
	const double n2 = 0.04226561;
	const double c0 = 0.12245056;
	const double c1_real = -0.0511651;
	//const double c2_real = 1.63518073;
	inline double R0_real(double vol_adcx){return n2 / vol_adcx / vol_adcx + n1 / vol_adcx + c0 + c1_real*vol_adcx;}
	double r0, vout;
	const u8 bits = 3;
	enum addr
	{
		voutaddr = 0x10,
		r0addr   = 0x20,
		picaddr  = 0x30,
	};
};
