/*
 * 007.SPI_TXOnly_Arduino.c
 *
 *  Created on: May 27, 2025
 *      Author: user
 */


#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include <string.h>
//PB14 - SPI2_MISO
//PB15 - SPI2_MOSI
//PB13 - SPI2_SCLK
//PB12 - SPI2_NSS
//ALT - Number 5


void delay(void)
{
	for(uint32_t i = 0; i< 500000*2; i++);
}

/*
 * SPI2 초기화 함수
 */
void SPI2_GPIOInit(void)
{
	//GPIOB의 핀 설정을 진행한것
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_AltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_PUSH_PULL;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PP_PD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_HIGH;

	//SCLK 설정
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	GPIO_Init(&SPIPins);

	//MOSI 설정
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_15;
	GPIO_Init(&SPIPins);

	//MISO 설정
	//MOSI만 쓰므로 우선 주석처리
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_14;
//	GPIO_Init(&SPIPins);

	//NSS 설정
	//아두이노 슬레이브 사용
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_12;
	GPIO_Init(&SPIPins);



}

void SPI2_Init(void)
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPI_PinConfig.SPI_BusConfig = SPI_BUS_CONFIG_FULL;
	SPI2handle.SPI_PinConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPI_PinConfig.SPI_SclkSpeed = SPI_SCLK_DIV_8;
	SPI2handle.SPI_PinConfig.SPI_DFF = SPI_DFF_8BIT;
	SPI2handle.SPI_PinConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPI_PinConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPI_PinConfig.SPI_SSM = SPI_SSM_DI;		//아두이노 통신하므로

	SPI_Init(&SPI2handle);


}

void GPIO_ButtonInit(void)
{
	//버튼을 누르면 LED가 켜지고 떼면 꺼짐

	GPIO_Handle_t GpioButton;

	GpioButton.pGPIOx = GPIOA;
	GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_0;
	GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_INPUT;
	GpioButton.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_VERY_HIGH;
//	GpioButton.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_PUSH_PULL; // 사용 안함
	GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PP_PD;

	GPIO_Init(&GpioButton);
}

int main(void)
{

	/*
	 * SPI 순서
	 * 1. SPI를 사용할 GPIO 그룹의 클럭 인에이블
	 * 2. SPI를 사용할 핀의 레지스터 설정
	 * 3. SPI를 사용할 모드 설정
	 */

	uint8_t user_data[] = "Hello World";

	GPIO_ButtonInit();
//	GPIO_PeriClockControl(GPIOB,ENABLE);
	SPI2_GPIOInit();		//내부에 GPIO CLOCK ENABLE 있음

	//SPI2 사용을 위한 기능 설정 init
	SPI2_Init();		//SPI_Init 내부에 peripheral 인에이블 함수 있음!!

	/*
	 * SSOE 1이면 NSS 출력이 인에이블
	 * NSS핀 자동으로 변경
	 * SPE=1이면, NSS자동으로 LOW
	 * SPE=0이면 NSS 자동으로 HIGH변경
	 */
	SPI_SSOEConfig(SPI2, ENABLE);
	while(1)
	{

		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));

		delay();

		//SPI2 peri enable 필수
		SPI_PeripheralControl(SPI2, ENABLE);

		//first send length info
		uint8_t dataLen = (uint8_t)(strlen((const char *)user_data));
//		SPI2->CR1 |= 1<<SPI_CR1_SPE;
		SPI_SendData(SPI2,&dataLen,1);

		SPI_SendData(SPI2,(uint8_t *)user_data,strlen((const char *)user_data));

		//SPI가 busy인지 확인
		while(SPI_GetFlagStatus(SPI2, SPI_BSY_FLAG)==SET);		//1을 줄 때까지 대기

		//disable SPI2
		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;
}
