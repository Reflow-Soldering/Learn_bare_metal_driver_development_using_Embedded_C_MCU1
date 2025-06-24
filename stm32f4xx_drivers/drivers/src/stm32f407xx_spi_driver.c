/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: May 26, 2025
 *      Author: user
 */

#include "stm32f407xx_spi_driver.h"


/*********************************************************************
 * @함수명					-	SPI_PeriClockControl
 *
 * @설명						-	함수를 이용하여 SPIx에 클럭 연결/해제 진행한다
 *
 *
 * @매개변수					-	SPI의 시작 메모리 주소
 * @매개변수					-	ENABLE 혹은 DISABLE 매크로
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	//각 주변장치마다 RCC에서 Enable/Disable 진행 필수
	if(EnorDi == ENABLE)
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
		else if(pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}
	}
	//GPIO Peripheral Clock Disable
	//GPIO 주변장치 클럭 해제
	else
	{
		if(pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}
		else if(pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}
		else if(pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
		else if(pSPIx == SPI4)
		{
			SPI4_PCLK_DI();
		}
	}
}


/*********************************************************************
 * @함수명					-	SPI_Init
 *
 * @설명						-	SPI 초기화
 *
 *
 * @매개변수					-	SPI 선언 구조체
 * @매개변수					-
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void SPI_Init(SPI_Handle_t *pSPIHandles)
{
	uint32_t tempreg = 0;

	//SPI 클럭 인에이블 - 여기다 써두면 초기화 할 떄 사용 가능
	SPI_PeriClockControl(pSPIHandles->pSPIx, ENABLE);


	//1. 디바이스 모드 설정
	tempreg |= (pSPIHandles->SPI_PinConfig.SPI_DeviceMode << SPI_CR1_MSTR);

	//2. 통신방식 설정
	if(pSPIHandles->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_FULL)
	{
		//BIDIMODE 0이어야함
		tempreg &= ~(1<< SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandles->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_HALF)
	{
		//BIDIMODE 1이어야함
		tempreg |= (1<<SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandles->SPI_PinConfig.SPI_BusConfig == SPI_BUS_CONFIG_RXONLY)
	{
		//BIDIMODE 0이고 RXONLY가 1이어야함
		tempreg &= ~(1<< SPI_CR1_BIDIMODE);
		tempreg |= (1<<SPI_CR1_RXONLY);
	}

	//3. 클럭 스피트 설정
	tempreg |= pSPIHandles->SPI_PinConfig.SPI_SclkSpeed << SPI_CR1_BR;

	//4. 데이터 포맷 결정
	tempreg |= pSPIHandles->SPI_PinConfig.SPI_DFF << SPI_CR1_DFF;

	//5. 클럭 극성 결정
	tempreg |= pSPIHandles->SPI_PinConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6. 클럭 에지 결정
	tempreg |= pSPIHandles->SPI_PinConfig.SPI_CPHA << SPI_CR1_CPHA;

	//레지스터 저장
	pSPIHandles->pSPIx->CR1 = tempreg;
}
/*********************************************************************
 * @함수명					-	SPI_DeInit
 *
 * @설명						-	SPI 초기화 해제
 *
 *
 * @매개변수					-	SPI 선언 구조체 변수
 * @매개변수					-
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
	else if(pSPIx == SPI4)
	{
		SPI4_REG_RESET();
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*********************************************************************
 * @함수명					-	SPI_SendData
 *
 * @설명						-	함수를 이용하여 데이터 전송
 *
 *
 * @매개변수					-	SPIx
 * @매개변수					-	8bit 데이터
 * @매개변수					-	길이
 *
 * @반환값					-	None
 *
 * @추가 내용					-	폴링 방식 코드. 모든 데이터를 보낼 때까지 다른것이 간섭 불가
 */

//폴링방식
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTXBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		// 1. TXE가 1인지 계속 확인한다.
//		while((pSPIx->SR & (1<<1)) == 0);
		while(SPI_GetFlagStatus(pSPIx,SPI_TXE_FLAG) == FLAG_RESET);

		// 2. CR1의 DFF 비트 확인
		if(pSPIx->CR1 & (1<<SPI_CR1_DFF))
		{
			//16비트 설정 시
			//1. DR에서 데이터 불러오기
			pSPIx->DR = *((uint16_t *)pTXBuffer);
			Len--;
			Len--;
			(uint16_t *)pTXBuffer++;
		}
		else
		{
			pSPIx->DR = *pTXBuffer;
			Len--;
			pTXBuffer++;
		}
	}
}

/*********************************************************************
 * @함수명					-	SPI_ReceiveData
 *
 * @설명						-	함수를 이용하여 데이터 수신
 *
 *
 * @매개변수					-	SPIx
 * @매개변수					-	8bit 데이터
 * @매개변수					-	길이
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRXBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		// 1. TRXNE가 1인지 계속 확인한다.
//		while((pSPIx->SR & (1<<1)) == 0);
		while(SPI_GetFlagStatus(pSPIx,SPI_RXNE_FLAG) == FLAG_RESET);

		// 2. CR1의 DFF 비트 확인
		if(pSPIx->CR1 & (1<<SPI_CR1_DFF))
		{
			//16비트 설정 시
			//1. RXBuffer에서 데이터 읽어오기
			*((uint16_t *)pRXBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t *)pRXBuffer++;
		}
		else
		{
			//8비트 전송일 경우
			*pRXBuffer = pSPIx->DR;
			Len--;
			pRXBuffer++;
		}
	}
}


//인터럽트 방식
/*********************************************************************
 * @함수명					-	SPI_IRQInterruptConfig
 *
 * @설명						-	함수를 이용하여 인터럽트 넘버와 Enable/Disable 결정
 *
 *
 * @매개변수					-	인터럽트 IRQNumber
 * @매개변수					-	ENABLE 혹은 DISABLE 매크로
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{

}

/*********************************************************************
 * @함수명					-	SPI_IRQPriorityConfig
 *
 * @설명						-	인터럽트의 우선순위 설정
 *
 *
 * @매개변수					-	인터럽트 IRQNumber
 * @매개변수					-	인터럽트 우선수위
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{

}

/*********************************************************************
 * @함수명					-	SPI_IRQHandling
 *
 * @설명						-	활성화된 인터럽트 핀 번호 설정
 *
 *
 * @매개변수					-	SPI 선언 구조체 변수
 * @매개변수					-
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void SPI_IRQHandling(SPI_Handle_t *pHandle)
{

}


/*
 * Peripheral control API
 */

/*********************************************************************
 * @함수명					-	SPI_PeripheralControl
 *
 * @설명						-	SPI 최종 enable 함수
 *
 *
 * @매개변수					-	GPIO 및 SPI 클럭 enable 후 SPI 동작을 위한 레지스터 설정
 * @매개변수					-
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}
	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}


/*********************************************************************
 * @함수명					-	SPI_SSIConfig
 *
 * @설명						-
 *
 *
 * @매개변수					-	SPI
 * @매개변수					-	EABLE/DISABLE
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR1 |= (1<<SPI_CR1_SSI);
	}
	else
	{
		pSPIx->CR1 &= ~(1<<SPI_CR1_SSI);
	}
}

/*********************************************************************
 * @함수명					-	SPI_SSOEConfig
 *
 * @설명						-
 *
 *
 * @매개변수					-
 * @매개변수					-
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx->CR2 |= (1<<SPI_CR2_SSOE);
	}
	else
	{
		pSPIx->CR2 &= ~(1<<SPI_CR2_SSOE);
	}
}
