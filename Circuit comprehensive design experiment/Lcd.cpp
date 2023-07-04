#include <Lcd.h>

void vgus_SendData(uint8_t address1, uint8_t address2, uint8_t num1, uint8_t num2)
{
	uint8_t arr[] = { 0xA5, 0x5A, 0x05, 0x82, 0x00, 0x20, 0x00, 0x10 };
	arr[4] = address1;
	arr[5] = address2;
	arr[6] = num1;
	arr[7] = num2;
	vgus_SendArr(arr, 8);
	//Delay_ms(500);
}

void vgus_SendDataPic( uint8_t num1, uint8_t num2)
{
	uint8_t arr[] = { 0xA5, 0x5A, 0x04, 0x80, 0x03, 0x00, 0x00 };
	arr[5] = num1;
	arr[6] = num2;
	vgus_SendArr(arr, 7);
	//Delay_ms(500);
}

void vgus_SendDataWord(u8 address1, u8 address2, u8 num1)
{
	uint8_t arr[] = { 0xA5, 0x5A, 0x04, 0x84, 0x00, 0x50, 0x50, 0x50, 0xFF, 0xFF };
	arr[4] = address1;
	arr[5] = address2;
	arr[5] = num1;
	//arr[6] = num2;
	vgus_SendArr(arr, 10);
	//Delay_ms(500);
}


void vgus_SendArr(uint8_t* arr, uint8_t length)
{
	for (uint8_t i = 0; i < length; i++)
	{
		Serial_SendByte(arr[i]);
	}
}