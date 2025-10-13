/*
 * 010.I2C_MASTER_TX_TESTING.c
 *
 *  Created on: Oct 13, 2025
 *      Author: user
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

#define MY_ADDRESS		0x61

#define SLAVE_ADDR		0x68

I2C_Handle_t	I2C1Handle;

void delay(void)
{
	for(uint32_t i = 0; i< 500000*2; i++);
}

//테스트 데이터
uint8_t test_data[] = "We are testing I2C master Tx\n";
//아두이노에 와이어 라이브러리에서는 한 번에 32바이트밖에 못보냄.
//그러므로 32바이트 이상을 보내려면 두 번 이상 나누어서 보내야 함.

/*
 * I2C1 초기화 함수
 */

//PB6 -> SCL
//PB7 - SDA
void I2C1_GPIOInit(void)
{
	GPIO_Handle_t	I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = OUTPUT_OP_TYPE_PUSH_PULL;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
	I2CPins.GPIO_PinConfig.GPIO_AltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_HIGH;


	//scl pin config하기
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_6;
	GPIO_Init(&I2CPins);

	//sda pin config하기
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_7;
	GPIO_Init(&I2CPins);

}

void I2C1_Init(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDRESS;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);
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
	//버튼 초기화
	GPIO_ButtonInit();

	//i2c pin 초기화
	I2C1_GPIOInit();

	//i2c peripehral 초기화 -> 클럭 enable도 있는거임
	I2C1_Init();

	//i2c 장치 enable
	I2C_PeripheralControl(I2C1, ENABLE);

	while(1)
	{
		//버튼 누를 때까지 대기
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));

		//디바운스 딜레이
		delay();

		//슬레이브에 데이터 전송
		I2C_MasterSendData(&I2C1Handle, test_data, strlen((char *)test_data), SLAVE_ADDR, 0);
	}



	while(1);

}
