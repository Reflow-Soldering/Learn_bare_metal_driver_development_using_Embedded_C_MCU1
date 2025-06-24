/*
 * 001.LED_Toggle.c
 *
 *  Created on: May 20, 2025
 *      Author: user
 */

#include "stm32f407xx.h"

void delay(void)
{
	for(uint32_t i = 0; i< 500000*2; i++);
}

int main(void)
{
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_VERY_HIGH;

	/*Push Pull 방식일 경우*/

	GpioLed.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_PUSH_PULL;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PP_PD;

	/*Open Drain 방식일 경우*/

//	GpioLed.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_OPEN_DRAIN;
//	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;


	GPIO_PeriClockControl(GPIOD, ENABLE);

	GPIO_Init(&GpioLed);

	while(1)
	{
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_12);
		delay();
	}
	return 0;
}
