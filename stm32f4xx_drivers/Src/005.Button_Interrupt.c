/*
 * 002.LED_Button.c
 *
 *  Created on: May 21, 2025
 *      Author: user
 */


#include "stm32f407xx.h"

void delay(void)
{
	for(uint32_t i = 0; i< 50000; i++);
}

int main(void)
{

	//버튼을 누르면 LED가 켜지고 떼면 꺼짐

	GPIO_Handle_t GpioLed,GpioButton;
	memset(&GpioLed,0,sizeof(GpioLed));			//지역변수에 대한 문제이므로 반드시 초기화
	memset(&GpioButton,0,sizeof(GpioButton));

	GpioLed.pGPIOx = GPIOD;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_12;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_VERY_HIGH;

	/*Push Pull 방식일 경우*/

	GpioLed.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_PUSH_PULL;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PP_PD;

	GPIO_PeriClockControl(GPIOD, ENABLE);		//클럭 인에이블 진행 후 초기화 하여야 버스에서 레지스터 데이터 전달 가능
	GPIO_Init(&GpioLed);





	GpioButton.pGPIOx = GPIOA;
	GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_0;
	GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INT_FT;
	GpioButton.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_VERY_HIGH;
//	GpioButton.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_PUSH_PULL; // 사용 안함
	GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PP_PD;

	GPIO_PeriClockControl(GPIOA, ENABLE);

	GPIO_Init(&GpioButton);


	//IRQ 만들기
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PRIO0);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);
	while(1);
	return 0;
}

void EXTI0_IRQHandler(void)
{
	delay();
	GPIO_IRQHandling(GPIO_PIN_0);
	GPIO_ToggleOutputPin(GPIOD, GPIO_PIN_12);
}
