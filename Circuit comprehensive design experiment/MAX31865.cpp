/* Include necessary libraries */
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_exti.h>
#include <misc.h>
#include <stm32f4xx_syscfg.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_spi.h>
#include <stdio.h>
#include <cmath>
/* Define chip select and data ready pins */
#define CS_PIN	GPIO_Pin_0
#define DRDY_PIN	GPIO_Pin_1

/* Define constants for temperature calculation */
#define RTD_A 3.9083e-3
#define RTD_B -5.775e-7

volatile uint16_t SpiData;

/* Function declarations */
void MAX31865_Init(SPI_TypeDef* SPIx, GPIO_TypeDef* GPIOx);
float MAX31865_Read_Temp(SPI_TypeDef* SPIx, GPIO_TypeDef* GPIOx);

int main(void)
{
	/* Initialize necessary peripherals and pins */
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);

	/* Initialize chip select and data ready pins */
	GPIO_InitStructure.GPIO_Pin = CS_PIN | DRDY_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC, CS_PIN | DRDY_PIN);

	/* Initialize SPI communication parameters */
	SPI_StructInit(&SPI_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);

	/* Initialize MAX31865 */
	MAX31865_Init(SPI1, GPIOC);

	/* Read temperature and display using printf */
	float temperature;

	while (1)
	{
		temperature = MAX31865_Read_Temp(SPI1, GPIOE);

		printf("Temperature: %0.2f\n", temperature);

		/* Delay before next measurement */
		for (int i = 0; i < 500000; i++) ;
	}
}

/* Initialize MAX31865 */
void MAX31865_Init(SPI_TypeDef* SPIx, GPIO_TypeDef* GPIOx)
{
	/* Disable chip select */
	GPIO_SetBits(GPIOx, CS_PIN);

	/* Set sample rate to 60 Hz */
	uint16_t data_out = 0x88;
	GPIO_ResetBits(GPIOx, CS_PIN);
	SPI_I2S_SendData(SPIx, data_out);
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)) ;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)) ;
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)) ;
	data_out = SPI_I2S_ReceiveData(SPIx);
	GPIO_SetBits(GPIOx, CS_PIN);

	/* Set 4-wire RTD mode */
	data_out = 0xC0;
	GPIO_ResetBits(GPIOx, CS_PIN);
	SPI_I2S_SendData(SPIx, data_out);
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)) ;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)) ;
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)) ;
	data_out = SPI_I2S_ReceiveData(SPIx);
	GPIO_SetBits(GPIOx, CS_PIN);

	/* Configure RTD wiring */
	data_out = 0x90;
	GPIO_ResetBits(GPIOx, CS_PIN);
	SPI_I2S_SendData(SPIx, data_out);
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)) ;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)) ;
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)) ;
	data_out = SPI_I2S_ReceiveData(SPIx);
	GPIO_SetBits(GPIOx, CS_PIN);

	/* Configure reference resistor value */
	data_out = 0xA0;
	GPIO_ResetBits(GPIOx, CS_PIN);
	SPI_I2S_SendData(SPIx, data_out);
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)) ;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)) ;
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)) ;
	data_out = SPI_I2S_ReceiveData(SPIx);
	GPIO_SetBits(GPIOx, CS_PIN);

	/* Enable continuous conversion mode */
	data_out = 0x40;
	GPIO_ResetBits(GPIOx, CS_PIN);
	SPI_I2S_SendData(SPIx, data_out);
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)) ;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)) ;
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)) ;
	data_out = SPI_I2S_ReceiveData(SPIx);
	GPIO_SetBits(GPIOx, CS_PIN);
}

/* Read temperature from MAX31865 */
float MAX31865_Read_Temp(SPI_TypeDef* SPIx, GPIO_TypeDef* GPIOx)
{
	/* Wait for conversion to complete */
	while (GPIO_ReadInputDataBit(GPIOx, DRDY_PIN)) ;

	/* Read temperature data */
	uint16_t rtd_data = 0;
	uint16_t fault_data = 0;
	float temperature = 0;

	GPIO_ResetBits(GPIOx, CS_PIN);
	SPI_I2S_SendData(SPIx, 0x00);
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)) ;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)) ;
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)) ;
	rtd_data = SPI_I2S_ReceiveData(SPIx);

	GPIO_ResetBits(GPIOx, CS_PIN);
	SPI_I2S_SendData(SPIx, 0x01);
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)) ;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)) ;
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)) ;
	rtd_data = SPI_I2S_ReceiveData(SPIx);

	GPIO_ResetBits(GPIOx, CS_PIN);
	SPI_I2S_SendData(SPIx, 0x02);
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)) ;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_BSY)) ;
	while (!SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)) ;
	fault_data = SPI_I2S_ReceiveData(SPIx);
	GPIO_SetBits(GPIOx, CS_PIN);

	/* Calculate temperature from RTD data */
	rtd_data >>= 1; /* Convert to 15-bit value */
	temperature = rtd_data / 32768.0 * 400.0;

	float r_rtd = 100 * (1.0 - rtd_data / 32768.0); /* Calculate resistance of RTD */
	float delta = RTD_A * RTD_A - 4 * RTD_B * (1 - r_rtd / 100);
	temperature = (-RTD_A + sqrt(delta)) / (2 * RTD_B); /* Calculate temperature */

	return temperature;
}