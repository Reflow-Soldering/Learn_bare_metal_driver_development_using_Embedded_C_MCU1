/*
 * stm32f407xx_spi_driver.c
 *
 *  Created on: May 26, 2025
 *      Author: user
 */

#include "stm32f407xx_spi_driver.h"

static void SPI_RxneInterrupt_handle(SPI_Handle_t *pSPIHandle);
static void SPI_TxeInterrupt_handle(SPI_Handle_t *pSPIHandle);
static void SPI_OVRInterrupt_handle(SPI_Handle_t *pSPIHandle);

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

	tempreg |= pSPIHandles->SPI_PinConfig.SPI_SSM << SPI_CR1_SSM;

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
//	if(pSPIx->SR & FlagName)
	if((pSPIx->SR & FlagName))
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
			//1. DR에서 RXBuffer로 데이터 읽어오기
			*((uint16_t *)pRXBuffer) = pSPIx->DR;
			Len--;
			Len--;
			(uint16_t *)pRXBuffer++;
		}
		else
		{
			//8비트 전송일 경우
			*(pRXBuffer) = pSPIx->DR;
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
	// 프로세서 측면에서 코드 작성
	// Cortex -M4 Devices Generic User Guide 참조 할 것
	// NVIC관련 확인 필수
	if(EnorDi == ENABLE)		//endable
	{
		if(IRQNumber <= 31)
		{
			//ISER0 레지스터 코드 작성
			*NVIC_ISER0 |= (1 << IRQNumber);
		}
		else if((IRQNumber > 31)&&(IRQNumber<64))
		{
			*NVIC_ISER1 |= (1 << IRQNumber %32);
		}
		else if((IRQNumber >= 64)&&(IRQNumber < 96))
		{
			*NVIC_ISER3 |= (1 << IRQNumber % 64);
		}
	}
	else						//disable
	{
		if(IRQNumber <=31)
		{
			*NVIC_ICER0 |= (1 << IRQNumber);
		}
		else if((IRQNumber > 31)&&(IRQNumber<64))
		{
			*NVIC_ICER1 |= (1 << IRQNumber %32);
		}
		else if((IRQNumber >= 64)&&(IRQNumber < 96))
		{
			*NVIC_ICER3 |= (1 << IRQNumber % 64);
		}
	}
}
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
	//1 IPR 레지스터 찾기
	uint8_t iprx = IRQNumber / 4; //8비트 구성으로 4개이므로
	uint8_t iprx_section = IRQNumber % 1; // 32바이트에서 시작되는 곳 확인

	uint8_t shift_amount = (8*iprx_section)+(8-NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority<<shift_amount);
	//상위 4비트는 사용 하위 4비트는 미사용

}

/*********************************************************************
 * @함수명					-	SPI_SendDataIT
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
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTXBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;
	if(state != SPI_BUSY_IN_TX)
	{
		//1. Tx buffer 길이와 주소를 전역변수에 저장
		pSPIHandle->pTxBuffer = pTXBuffer;
		pSPIHandle->TxLen = Len;

		//2. SPI의 상태를 busy로 한다 ->> 다른 코드가 SPI 장치를 건드리지 못하게(busy면 나중에 다시 코드가 넘어와야함)
		pSPIHandle->TxState = SPI_BUSY_IN_TX;
		//3. SR에서 TXE가 셋 될 때마다 TXEIE가 활성화 되게 한다
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
	}
		//4. ISR코드에 의해 데이터 전송시킴

	return state;
}
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRXBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;
	if(state != SPI_BUSY_IN_RX)
	{
		//1. Rx buffer 길이와 주소를 전역변수에 저장
		pSPIHandle->pRxBuffer = pRXBuffer;
		pSPIHandle->RxLen = Len;

		//2. SPI의 상태를 busy로 한다 ->> 다른 코드가 SPI 장치를 건드리지 못하게(busy면 나중에 다시 코드가 넘어와야함)
		pSPIHandle->RxState = SPI_BUSY_IN_RX;
		//3. SR에서 RXE가 셋 될 때마다 TXEIE가 활성화 되게 한다
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXEIE);
	}
		//4. ISR코드에 의해 데이터 전송시킴

	return state;
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
	uint8_t temp1, temp2;

	//TXE 확인
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2)
	{
		//TXE 핸들러 구동
		SPI_TxeInterrupt_handle(pHandle);
	}

	//RXNE 확인
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXEIE);

	if(temp1 && temp2)
	{
		//TXE 핸들러 구동
		SPI_RxneInterrupt_handle(pHandle);

	}

	//OVR 확인
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);
	if(temp1 && temp2)
	{
		//TXE 핸들러 구동
		SPI_OVRInterrupt_handle(pHandle);
	}
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
 * @추가 내용					-	내부적으로 NSS핀 HIGH
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




//인터럽트 핸들러 구현


static void SPI_TxeInterrupt_handle(SPI_Handle_t *pSPIHandle)
{
	// 2. CR1의 DFF 비트 확인
	if(pSPIHandle->pSPIx->CR1 & (1<<SPI_CR1_DFF))
	{
		//16비트 설정 시
		//1. DR에서 데이터 불러오기
		pSPIHandle->pSPIx->DR = *((uint16_t *)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(uint16_t *)pSPIHandle->pTxBuffer++;
	}
	else
	{
		pSPIHandle->pSPIx->DR = *pSPIHandle->pTxBuffer;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}
	//길이가 0이면
	if(!pSPIHandle->TxLen)
	{
		//TxLen이 0이어서 전송을 종료해야하면 인터럽트 플래그를 종료해야함
		/*pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
		pSPIHandle->pTxBuffer = NULL;
		pSPIHandle->TxLen = 0;
		pSPIHandle->TxState = SPI_READY;*/
		//아래로 대체
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);

	}
}

static void SPI_RxneInterrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx->CR1 & (1<<SPI_CR1_DFF))
	{
		//16비트 설정 시
		//1. DR에서 RXBuffer로 데이터 읽어오기
		*((uint16_t *)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen -= 2;

		pSPIHandle->pRxBuffer++;
		pSPIHandle->pRxBuffer++;
	}
	else
	{
		//8비트 전송일 경우
		*(pSPIHandle->pRxBuffer) = (uint8_t) pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}
	if(pSPIHandle->RxLen)
	{
		/*
		pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXEIE);
		pSPIHandle->pRxBuffer = NULL;
		pSPIHandle->RxLen = 0;
		pSPIHandle->RxState = SPI_READY;*/
		//아래로 대체
		SPI_CloseTransmission(pSPIHandle);

		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);
	}



}
static void SPI_OVRInterrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//1. OVR 제거
	uint8_t temp;

	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
	}
	(void)temp;
	//2. 실행
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_ERR);


}


void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;

}
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}


__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t AppEv)
{

}
