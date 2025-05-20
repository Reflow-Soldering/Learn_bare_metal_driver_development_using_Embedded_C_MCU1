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
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_NO_PP_PD;

	GPIO_PeriClockControl(GPIOD, ENABLE);

	GPIO_Init(&GpioLed);

	while(1)
	{
		GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_12);
		delay();
	}
	return 0;
}
