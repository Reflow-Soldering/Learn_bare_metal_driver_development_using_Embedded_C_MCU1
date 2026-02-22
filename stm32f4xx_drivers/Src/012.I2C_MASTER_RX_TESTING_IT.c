/*
 * 010.I2C_MASTER_TX_TESTING.c
 *
 *  Created on: Oct 13, 2025
 *      Author: user
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

extern void initialise_monitor_handles();

uint8_t RxComplt = RESET;

#define MY_ADDRESS		0x61

//ACK Failure 확인을 위해 0x68을 0x66등으로 바꾸어 해보자
#define SLAVE_ADDR		0x68

I2C_Handle_t	I2C1Handle;

void delay(void)
{
	for(uint32_t i = 0; i< 500000*2; i++);
}

//테스트 데이터
uint8_t rcv_buf[32];
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


	uint8_t commandcode;

	uint8_t len;

	initialise_monitor_handles();

	printf("App is running\n");

	//버튼 초기화
	GPIO_ButtonInit();

	//i2c pin 초기화
	I2C1_GPIOInit();

	//i2c peripehral 초기화 -> 클럭 enable도 있는거임
	I2C1_Init();

	//i2c IRQ config
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	//i2c 장치 enable
	I2C_PeripheralControl(I2C1, ENABLE);

	//i2c ack enable
	//ack enable은 반드시 PE 비트가 1이 된 이후에 되어야한다.
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

	while(1)
	{
		//버튼 누를 때까지 대기
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));

		//디바운스 딜레이
		delay();

		commandcode = 0x51;

		while(I2C_MasterSendDataIT(&I2C1Handle, &commandcode, 1, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

		while(I2C_MasterReceiveDataIT(&I2C1Handle, &len, 1, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);


		commandcode = 0x52;

		while(I2C_MasterSendDataIT(&I2C1Handle, &commandcode, 1, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

		while(I2C_MasterReceiveDataIT(&I2C1Handle, rcv_buf, len, SLAVE_ADDR, I2C_ENABLE_SR) != I2C_READY);

		while(RxComplt != SET)
		{

		}
		rcv_buf[len+1] = '\0';

		printf("Data : %s", rcv_buf);

		RxComplt = RESET;
	}
}


//I2C 인터럽트가 행동되고, Event로 되었는지 Error로 되었는지 Flag가 뜨기 때문에
//인터럽트 신호가 뜨면 IRQHandler로 진입한다.
//여기서 중요한 점은 IRQ 내에서 함수를 호출하는 것이다.
//그런데 이게 함수 호출은 맞지만 구조상 콜 백이 맞는지 헷갈린다.
void I2C1_EV_IRQHandler(void)
{
	I2C_EV_IRQHandling(&I2C1Handle);
}
void I2C1_ER_IRQHandler(void)
{
	I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv)
{
	if(AppEv == I2C_EV_TX_CMPLT)
	{
		printf("Tx is completes\n");
	}
	else if(AppEv == I2C_EV_RX_CMPLT)
	{
		printf("Rx is comleted\n");
		RxComplt = SET;
	}
	else if(AppEv == I2C_ERROR_AF)
	{
		//\n을 쓰지 않으면 세미호스팅은 출력하지 않음
		printf("error : Ack failure\n");
		I2C_CloseSendData(pI2CHandle);
		I2C_GenerateStopCondition(I2C1);

		while(1);
	}
}
