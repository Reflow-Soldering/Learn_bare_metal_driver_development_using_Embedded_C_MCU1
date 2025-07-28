/*
 * stm32f407xx_spi_driver.h
 *
 *  Created on: May 26, 2025
 *      Author: user
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"
/* SPI 드라이버 설정 */
typedef struct
{
	uint8_t SPI_DeviceMode;		//MCU의 모드 설정 : 슬레이브/마스터
	uint8_t SPI_BusConfig;		//MCU 버스 설정 :
	uint8_t SPI_SclkSpeed;		//SPI 속도 설정 : Baudrate
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_PinConfig_t;

typedef struct
{
	// GPIO 장치들의 기본값을 포인터로 지정
	SPI_RegDef_t *pSPIx;
	SPI_PinConfig_t SPI_PinConfig;

}SPI_Handle_t;


/*
 * @SPI DeviceMode
 * SPI 모드 설정
 */
#define SPI_DEVICE_MODE_MASTER					1
#define SPI_DEVICE_MODE_SLAVE					0


/*
 * @SPI_BusConfig
 * SPI 통신방식 설정 반이중 전이중 단방향(송신, 수신)
 */
#define SPI_BUS_CONFIG_FULL						1
#define SPI_BUS_CONFIG_HALF						2
//#define SPI_BUS_CONFIG_TXONLY					3
#define SPI_BUS_CONFIG_RXONLY					3 //4


/*
 * @SPI_SclkSpeed
 * SPI 클럭 스피드 설정
 */
#define	SPI_SCLK_DIV_2								0		//PCLK/2
#define	SPI_SCLK_DIV_4								1		//PCLK/4
#define	SPI_SCLK_DIV_8								2
#define	SPI_SCLK_DIV_16								3
#define	SPI_SCLK_DIV_32								4
#define	SPI_SCLK_DIV_64								5
#define	SPI_SCLK_DIV_128							6
#define	SPI_SCLK_DIV_256							7


/*
 * @SPI_DFF
 * SPI 데이터 전송 포맷 설정 8비트, 16비트
 */
#define	SPI_DFF_8BIT								0
#define SPI_DFF_16BIT								1


/*
 * @SPI_CPOL
 * SPI 클럭 idle 극성 설정
 */
#define SPI_CPOL_LOW								0
#define SPI_CPOL_HIGH								1



/*
 * @SPI_CPHA
 * SPI 클럭 idle 극성 설정
 */
#define SPI_CPHA_LOW								0
#define SPI_CPHA_HIGH								1

/*
 * @SPI_SSM
 * SPI 소프트웨어적 slave모드 설정 여부
 */
#define SPI_SSM_DI									0
#define SPI_SSM_EN									1



//SPI TXE 마스킹 매크로

#define SPI_TXE_FLAG								(1<<SPI_SR_TXE)
#define SPI_RXNE_FLAG								(1<<SPI_SR_RXNE)
#define SPI_BSY_FLAG								(1<<SPI_SR_BSY)

/*
 * SPI CLK Set
 * SPI 클럭 연결 여부
 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
/*
 * SPI Init-Deinit
 * SPI
 */
void SPI_Init(SPI_Handle_t *pSPIHandles);

void SPI_DeInit(SPI_RegDef_t *pSPIx);


/*
 * SPI 통신 송신 및 수신
 */
/*
 * 종류는 세가지
 * 1. 폴링
 * 2. 인터럽트
 * 3. DMA
 *
 */

//폴링방식
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTXBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx,uint8_t *pRXBuffer, uint32_t Len);



//인터럽트 방식
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

void SPI_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);

void SPI_IRQHandling(SPI_Handle_t *pHandle);


#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
