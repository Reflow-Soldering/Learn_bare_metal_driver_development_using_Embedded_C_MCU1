/*
 * 006.SPI_TX_Testing.c
 *
 *  Created on: May 26, 2025
 *      Author: user
 */

#include "stm32f407xx.h"
#include <string.h>
//PB14 - SPI2_MISO
//PB15 - SPI2_MOSI
//PB13 - SPI2_SCLK
//PB12 - SPI2_NSS
//ALT - Number 5


/*
 * SPI2 초기화 함수
 */
void SPI_GPIOInit(void)
{
	//GPIOB의 핀 설정을 진행한것
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_AltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_PUSH_PULL;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PP_PD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_VERY_HIGH;

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
	//Slave가 하나이므로 우선 주석처리
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_12;
//	GPIO_Init(&SPIPins);



}

void SPI2_Init(void)
{
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPI_PinConfig.SPI_BusConfig = SPI_BUS_CONFIG_FULL;
	SPI2handle.SPI_PinConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPI_PinConfig.SPI_SclkSpeed = SPI_SCLK_DIV_2;
	SPI2handle.SPI_PinConfig.SPI_DFF = SPI_DFF_8BIT;
	SPI2handle.SPI_PinConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPI_PinConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPI_PinConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2handle);


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

//	GPIO_PeriClockControl(GPIOB,ENABLE);
	SPI_GPIOInit();		//내부에 GPIO CLOCK ENABLE 있음

	//SPI2 사용을 위한 기능 설정 init
	SPI2_Init();		//SPI_Init 내부에 peripheral 인에이블 함수 있음!!

	//SSI Configure - 내부 NSS핀(마스터 슬레이브 핀 을 high로 만듬 MODF에러 없애기 위함
	SPI_SSIConfig();

	//SPI2 peri enable 필수
	SPI_PeriClockControl(SPI2, ENABLE);

	//1차로 슬레이브가 받을 수 있는 데이터 길이를 확인
	uint8_t dataLen = strlen((const char*)user_data);
	SPI_SendData(SPI2,&data_Len,1);

	//SPI 데이터 전송
	SPI_SendData(SPI2,user_data,strlen((const char *)user_data));

	//SPI가 busy인지 확인
	while(SPI_GetFlagStatus(SPI2, SPI_SR_BSY));		//1을 줄 때까지 대기

	//disable SPI2
	SPI_PeriClockControl(SPI2, DISABLE);
	while(1);

	return 0;
}
