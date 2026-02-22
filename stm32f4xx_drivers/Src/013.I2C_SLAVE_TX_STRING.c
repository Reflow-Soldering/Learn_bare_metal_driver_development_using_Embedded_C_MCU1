/*
 * 010.I2C_MASTER_TX_TESTING.c
 *
 *  Created on: Oct 13, 2025
 *      Author: user
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>

//extern void initialise_monitor_handles();


#define SLAVE_ADDR		0x68
#define MY_ADDRESS			SLAVE_ADDR

I2C_Handle_t	I2C1Handle;

void delay(void)
{
	for(uint32_t i = 0; i< 500000*2; i++);
}

//테스트 데이터
uint8_t Tx_buf[32] = "STM32 Slave Mode Testing..";
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
	//위에서 ALTFN으로 설정, 4번 모드로 작동하므로 핀만 넣어주면 된다
	//그러고 나서 init 진행
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

	I2C_SlaveEnableDisableCallbackEvents(I2C1, ENABLE);

	//i2c 장치 enable
	I2C_PeripheralControl(I2C1, ENABLE);

	//i2c ack enable
	//ack enable은 반드시 PE 비트가 1이 된 이후에 되어야한다.
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

	while(1)
	{

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
	static uint8_t commandCode = 0;
	static uint8_t Cnt = 0;
	if(AppEv == I2C_EV_DATA_REQ)
	{
		//마스터가 데이터를 원할 때 슬레이브는 해당 진행을 함
		if(commandCode == 0x51)
		{
			//마스터에게 데이터 길이부터 전송함
			I2C_SlaveSendData(pI2CHandle->pI2Cx,strlen((char*)Tx_buf));
		}
		else if(commandCode)
		{
			//Tx_buf에 있는 정보를 전송
			I2C_SlaveSendData(pI2CHandle->pI2Cx, Tx_buf[Cnt++]);
		}
	}
	else if(AppEv == I2C_EV_DATA_RCV)
	{
		//데이터 수신 완료 플래그. 슬레이브가 읽을 때까지 기다리는 중. 슬레이브는 해당 데이터를 읽어야 함
		commandCode = I2C_SlaveReceiveData(pI2CHandle->pI2Cx);

	}
	else if(AppEv == I2C_ERROR_AF)
	{
		//슬레이브가 데이터를 보낼 때만 발생함
		//마스터는 NACK신호를 보내고, 슬레이브는 NACK으로 인해 마스터가 데이터 전송을 받지 않는 것을 확인함
		commandCode = 0xff;
		Cnt = 0;
	}
	else if(AppEv == I2C_EV_STOP)
	{
		//슬레이브가 데이터를 받을 때만 발생
		//마스터는 슬레이브와 같이 I2C 통신을 종료한다.
	}

}
