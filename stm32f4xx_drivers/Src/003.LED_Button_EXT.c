/*
 * 002.LED_Button.c
 *
 *  Created on: May 21, 2025
 *      Author: user
 */


#include "stm32f407xx.h"

void delay(void)
{
	for(uint32_t i = 0; i< 500000*2; i++);
}

int main(void)
{

	//버튼을 누르면 LED가 켜지고 떼면 꺼짐

	GPIO_Handle_t GpioLed,GpioButton;

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_VERY_HIGH;

	/*Push Pull 방식일 경우*/

	GpioLed.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_PUSH_PULL;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PP_PD;

	GPIO_PeriClockControl(GPIOD, ENABLE);		//클럭 인에이블 진행 후 초기화 하여야 버스에서 레지스터 데이터 전달 가능
	GPIO_Init(&GpioLed);





//	External Push Button Test
	GpioButton.pGPIOx = GPIOA;
	GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_0;
	GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT;
	GpioButton.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_VERY_HIGH;
//	GpioButton.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_PUSH_PULL; // 사용 안함
	GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PP_PD;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Init(&GpioButton);



	//내가 사용하는 스위치는 풀업 상태이므로 LED는 기본적으로 켜져있고,
	//스위치를 누르면 LED가 꺼짐 상태로 변경됨
	while(1)
	{
		if(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0) == SET)
		{
			GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_12, SET);
		}
		else
		{
			GPIO_WriteToOutputPin(GPIOD, GPIO_PIN_12, RESET);
		}
	}
	return 0;
}
