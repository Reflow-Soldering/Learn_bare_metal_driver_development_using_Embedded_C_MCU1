/*
 * 008.SPI_CMD_Handling.c
 *
 *  Created on: Jun 4, 2025
 *      Author: user
 */

#include "stm32f407xx.h"
#include <string.h>
#include <stdio.h>
//PB14 - SPI2_MISO
//PB15 - SPI2_MOSI
//PB13 - SPI2_SCLK
//PB12 - SPI2_NSS
//ALT - Number 5

/*
 * 아두이노 관련 매크로
 */
#define	COMMAND_LED_CTRL						0x50
#define COMMAND_SENSOR_READ						0x51
#define COMMAND_LED_READ						0x52
#define COMMAND_PRINT							0x53
#define COMMAND_ID_READ							0x54

#define LED_ON									1
#define LED_OFF									0

#define ANALOG_PIN0								0
#define ANALOG_PIN1								1
#define ANALOG_PIN2								2
#define ANALOG_PIN3								3
#define ANALOG_PIN4								4
#define ANALOG_PIN5								5


#define LED_PIN_9								9

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
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = OUTPUT_SPD_VERY_HIGH;

	//SCLK 설정
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_13;
	GPIO_Init(&SPIPins);

	//MOSI 설정
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_15;
	GPIO_Init(&SPIPins);

	//MISO 설정
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_14;
	GPIO_Init(&SPIPins);

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

uint8_t SPI_VerifyResponse(uint8_t AckByte)
{
	if(AckByte == 0xF5)
	{
		//ack
		return 1;
	}
	else
	{
		//nack
		return 0;
	}
}

int main(void)
{

	/*
	 * SPI 순서
	 * 1. SPI를 사용할 GPIO 그룹의 클럭 인에이블
	 * 2. SPI를 사용할 핀의 레지스터 설정
	 * 3. SPI를 사용할 모드 설정
	 */

	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;
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


		/*
		 * LED 온오프 기능 ************************************************************
		 */
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));

		delay();

		//SPI2 peri enable 필수
		SPI_PeriClockControl(SPI2, ENABLE);

		//CMD_LED_CTRL
		uint8_t commandcode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];



		//1.아두이노에 명령어 전송
		SPI_SendData(SPI2, &commandcode, 1);

		//1-1 MISO로 들어온 RX 데이터를 읽어 삭제한다
		//더미 데이터 읽기
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//2.아두이노에 데이터 잘 받았는지 수신하기 위한 더미데이터 전송
		SPI_SendData(SPI2, &dummy_write, 1);

		//LSB to MSB 데이터가 ack인지 확인하기 위한 리시브
		//이 경우는 SPI의 rxbuffer로 저장되므로, rxbuffer에서 ackbyte로 가야 한다.
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if(SPI_VerifyResponse(ackbyte))
		{
			//send arguments
			args[0] = LED_PIN_9;
			args[1] = LED_ON;
			SPI_SendData(SPI2, args, 2);
		}

		/*
		 *  아날로그 핀 값 읽어오기************************************************
		 */
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));

		delay();

		//SPI2 peri enable 필수이나 첫 번째에서 진행 완료
//		SPI_PeriClockControl(SPI2, ENABLE);

		//센서 리드 명령어
		commandcode = COMMAND_SENSOR_READ;

		SPI_SendData(SPI2, &commandcode, 1);

		//더미데이터 읽기 -> 아두이노에 있던 데이터
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//아두이노에서 데이터 잘 받았는지 수신 명령
		SPI_SendData(SPI2, &dummy_write, 1);

		//ack nack확인
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		//ack이면 데이터 읽어오게 하기
		if(SPI_VerifyResponse(ackbyte))
		{
			//send arguments
			args[0] = ANALOG_PIN0;
			SPI_SendData(SPI2, args, 1);
			//더미데이터를 한 번 더 보냈으므로
			SPI_ReceiveData(SPI2, &dummy_read, 1);

			//아두이노에서 adc가 끝나는 시간을 맞추기 위해 딜레이 삽입
			delay();

			//더미데이터를 보내 아두이노에서 읽은 아날로그 값을 가져온다
			SPI_SendData(SPI2, &dummy_write, 1);

			//가져온 데이터를 저장한다
			uint8_t analog_read;
			SPI_ReceiveData(SPI2, &analog_read, 1);

		}


		/*
		 * LED 온오프상태 읽어오기
		 */
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));

		delay();

		//SPI2 peri enable 필수이나 첫 번째 에서 했음
//		SPI_PeriClockControl(SPI2, ENABLE);

		//센서 리드 명령어
		commandcode = COMMAND_LED_READ;

		SPI_SendData(SPI2, &commandcode, 1);

		//더미데이터 읽기 -> 아두이노에 있던 데이터
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//아두이노에서 데이터 잘 받았는지 수신 명령 -더미데이터 전송
		SPI_SendData(SPI2, &dummy_write, 1);

		//ack nack확인
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		//ack이면 데이터 읽어오게 하기
		if(SPI_VerifyResponse(ackbyte))
		{
			//send arguments
			args[0] = LED_PIN_9;
			SPI_SendData(SPI2, args, 1);

			//데이터를 한 번 더 보냈으므로 RXNE 버퍼 초기화 위해서
			SPI_ReceiveData(SPI2, &dummy_read, 1);

			//아두이노에서 adc가 끝나는 시간을 맞추기 위해 딜레이 삽입
			delay();

			//더미데이터를 보내 아두이노에서 읽은 아날로그 값을 가져온다
			SPI_SendData(SPI2, &dummy_write, 1);

			//가져온 데이터를 저장한다
			uint8_t led_status_read;
			SPI_ReceiveData(SPI2, &led_status_read, 1);

		}

		/*
		 * 아두이노에서 문자열 프린트하기
		 */
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));

		delay();

		//SPI2 peri enable 필수이나 첫 번째 에서 했음
//		SPI_PeriClockControl(SPI2, ENABLE);

		//센서 리드 명령어
		commandcode = COMMAND_PRINT;

		SPI_SendData(SPI2, &commandcode, 1);

		//더미데이터 읽기 -> 아두이노에 있던 데이터
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//아두이노에서 데이터 잘 받았는지 수신 명령 -더미데이터 전송
		SPI_SendData(SPI2, &dummy_write, 1);

		//ack nack확인
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		//ack이면 데이터 읽어오게 하기
		uint8_t message[] = "Hello world!!";
		if(SPI_VerifyResponse(ackbyte))
		{
			//send arguments
			args[0] = strlen((char *)message);
			SPI_SendData(SPI2, args, 1);

			//데이터를 한 번 더 보냈으므로 RXNE 버퍼 초기화 위해서
			SPI_ReceiveData(SPI2, &dummy_read, 1);

			//아두이노 준비시간 대기
			delay();

			//데이터 전송 message 길이만큼
			for(uint8_t i = 0; i< args[0]; i++)
			{
				//1번 보내면 MISO에서 받은 데이터 읽어줘야함 -> 초기화 위해서
				SPI_SendData(SPI2, &message[i], 1);
				SPI_ReceiveData(SPI2, &dummy_read, 1);
			}

		}

		/*
		 * 아두이노 아이디 읽어오기
		 */
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_0));

		delay();

		//SPI2 peri enable 필수이나 첫 번째 에서 했음
//		SPI_PeriClockControl(SPI2, ENABLE);

		//센서 리드 명령어
		commandcode = COMMAND_ID_READ;

		SPI_SendData(SPI2, &commandcode, 1);

		//더미데이터 읽기 -> 아두이노에 있던 데이터
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//아두이노에서 데이터 잘 받았는지 수신 명령 -더미데이터 전송
		SPI_SendData(SPI2, &dummy_write, 1);

		//ack nack확인
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		//ack이면 아이디 읽어오기
		uint8_t id[11];
		uint32_t i = 0;
		if(SPI_VerifyResponse(ackbyte))
		{
			//데이터 전송 message 길이만큼
			for(i = 0; i< args[0]; i++)
			{
				//1번 보내면 MISO에서 받은 데이터 읽어줘야함 -> 초기화 위해서
				SPI_SendData(SPI2, &dummy_write, 1);
				SPI_ReceiveData(SPI2, &id[i], 1);
			}
			id[10] = '\n';
			printf("COMMAND_ID : %s \n", id);
		}

	//		SPI_SendData(SPI2,user_data,strlen((const char *)user_data));

			//SPI가 busy인지 확인
			while(SPI_GetFlagStatus(SPI2, SPI_SR_BSY));		//1을 줄 때까지 대기

			//disable SPI2
			SPI_PeriClockControl(SPI2, DISABLE);

			printf("COMMAND_ID : %s \n", id);
	}

	return 0;
}
