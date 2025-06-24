/*
 * stm32f407xx.h
 *
 *  Created on: May 18, 2025
 *      Author: user
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

//stdint.h 사용이유
/*
 * stdint.h는 uint16_t, uint8_t 등 변수의 형태를
 * 규정하므로 반드시 필요하다
 */
#include <stdint.h>
#include <memory.h>
//volatile define
/*
 * 일일히 volatile을 쓸 수 없으므로 약어로 정리
 */
#define __vo									volatile

/*********************************프로세서 스펙 상세******************************
 *
 * ARM Cortex Mx 프로세서 NVIC ISERx 레지스터 주소
 * NVIC ISER은 0부터 7까지 8개가 있음
 *
 * 해당 내용은 Cortex-M4 Devices Generic User Guide 참조할 것
 * Table 4-2
 */

//NVIC Interrupt Set Enable 레지스터
#define	NVIC_ISER0							((__vo uint32_t *)0xE000E100)
#define	NVIC_ISER1							((__vo uint32_t *)0xE000E104)
#define	NVIC_ISER2							((__vo uint32_t *)0xE000E108)
#define	NVIC_ISER3							((__vo uint32_t *)0xE000E10C)
#define	NVIC_ISER4							((__vo uint32_t *)0xE000E110)
#define	NVIC_ISER5							((__vo uint32_t *)0xE000E114)
#define	NVIC_ISER6							((__vo uint32_t *)0xE000E118)
#define	NVIC_ISER7							((__vo uint32_t *)0xE000E11C)

//NVIC Interrupt Clear Enable 레지스터
#define NVIC_ICER0							((__vo uint32_t *)0XE000E180)
#define NVIC_ICER1							((__vo uint32_t *)0XE000E184)
#define NVIC_ICER2							((__vo uint32_t *)0XE000E188)
#define NVIC_ICER3							((__vo uint32_t *)0XE000E18C)
#define NVIC_ICER4							((__vo uint32_t *)0XE000E190)
#define NVIC_ICER5							((__vo uint32_t *)0XE000E194)
#define NVIC_ICER6							((__vo uint32_t *)0XE000E198)
#define NVIC_ICER7							((__vo uint32_t *)0XE000E19C)

#define NVIC_PR_BASE_ADDR					((__vo uint32_t *)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED				4

//NVIC Interrupt Set-Pendinf 레지스터
/*
#define NVIC_ISPR0							((__vo uint32_t *)0XE000E200)
#define NVIC_ISPR1							((__vo uint32_t *)0XE000E204)
#define NVIC_ISPR2							((__vo uint32_t *)0XE000E208)
#define NVIC_ISPR3							((__vo uint32_t *)0XE000E20C)
#define NVIC_ISPR4							((__vo uint32_t *)0XE000E210)
#define NVIC_ISPR5							((__vo uint32_t *)0XE000E214)
#define NVIC_ISPR6							((__vo uint32_t *)0XE000E218)
#define NVIC_ISPR7							((__vo uint32_t *)0XE000E21C)
*/


//NVIC Interrupt Clear-Pending 레지스터
/*
#define NVIC_ICPR0							((__vo uint32_t *)0XE000E280)
#define NVIC_ICPR1							((__vo uint32_t *)0XE000E284)
#define NVIC_ICPR2							((__vo uint32_t *)0XE000E288)
#define NVIC_ICPR3							((__vo uint32_t *)0XE000E28C)
#define NVIC_ICPR4							((__vo uint32_t *)0XE000E290)
#define NVIC_ICPR5							((__vo uint32_t *)0XE000E294)
#define NVIC_ICPR6							((__vo uint32_t *)0XE000E298)
#define NVIC_ICPR7							((__vo uint32_t *)0XE000E29C)
*/

//NVIC Interrupt Active Bit 레지스터
/*
#define NVIC_IABR0							((__vo uint32_t *)0XE000E300)
#define NVIC_IABR1							((__vo uint32_t *)0XE000E304)
#define NVIC_IABR2							((__vo uint32_t *)0XE000E308)
#define NVIC_IABR3							((__vo uint32_t *)0XE000E30C)
#define NVIC_IABR4							((__vo uint32_t *)0XE000E310)
#define NVIC_IABR5							((__vo uint32_t *)0XE000E314)
#define NVIC_IABR6							((__vo uint32_t *)0XE000E318)
#define NVIC_IABR7							((__vo uint32_t *)0XE000E31C)
*/

//NVIC Interrupt Priority 레지스터


//NVIC Interrupt 소프트웨어 트리거 인터럽트 레지스터
#define NVIC_STIR							((__vo uint32_t *)0XE000E41C)

/*This is address list of Memory*/
/*
 * 각 메모리의 시작 위치를 규정한다
 * 왜 FSMC는 안쓸까?
 */
#define	FLASH_BASEADDR							0x08000000U /* FLASH Base Address is here*/
#define SRAM1_BASEADDR							0x20000000U	/* SRAM1 Base Address is here - 112KB*/
#define SRAM2_BASEADDR							0x2001C000U /* SRAM2 Base Address is here - 16KB*/
#define ROM_BASEADDR							0x1FFF0000U /* ROM Base Address is here*/
#define SRAM									SRAM1_BASEADDR /* Equal SRAM1 Base Address*/

/*This is address list of Peripheral*/
/*
 * 각 주변장치의 시작 메모리를 규정
 * 그리고 이 메모리는 2.3 Memory Map에 있다.
 * 이 경우 APB, AHB와 시작되는 Peri 주소를 규정하였고,
 * Peri의 시작은 APB1의 시작과 같다
 * 2.3 Memory Map을 확인한다.
 */

#define PERIPH_BASE								0x40000000U
#define APB1PHERIPH_BASE						PERIPH_BASE
#define APB2PHERIPH_BASE						0x40010000U
#define AHB1PHERIPH_BASE						0x40020000U
#define AHB2PHERIPH_BASE						0x50000000U


/* Base Addresses of GPIOx
 * GPIO is hanging of AHB1 Bus
 * AHB1에 GPIO말고 다른 것들이 있으나
 * GPIO 그룹이 많으므로 별도로 작성한다.
 */

#define	GPIOA_BASEADDR							(AHB1PHERIPH_BASE + 0x0000)
#define GPIOB_BASEADDR							(AHB1PHERIPH_BASE + 0x0400)
#define GPIOC_BASEADDR							(AHB1PHERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR							(AHB1PHERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR							(AHB1PHERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR							(AHB1PHERIPH_BASE + 0x1400)
#define GPIOG_BASEADDR							(AHB1PHERIPH_BASE + 0x1800)
#define GPIOH_BASEADDR							(AHB1PHERIPH_BASE + 0x1C00)
//#define GPIOI_BASEADDR							(AHB1PHERIPH_BASE + 0x2000)

/*RCC 시작주소 지정*/
/*
 * GPIO 세팅때문에 미리 하나 만들어둠
 */
#define RCC_BASEADDR							(AHB1PHERIPH_BASE + 0x3800)



/****************************** 주변장치 그룹화 ******************************
 * APB1, APB2, AHB1, AHB2, AHB3가 있으므로 각 그룹별로 나눈다.
 * 단, 여기서 GPIO 그룹은 위에 별도로 진행했으므로 GPIO는 빼도록 한다.
 */


/* BASE Addresses of peripherals
 * hanging on APB1  Bus
 * APB1 BUS에 있는  주변장치를 모두 규정한다.
 * TIM 2/3/4/5/6/7/12/13/14
 * USART 2/3
 * UART 4/5
 * SPI 2/3 <-> I2S 2/3
 * I2C 1/2/3 이외 기타
 * DATASHEET 3 FUNCTION OVERVIEW BLOCK DIAGRAM 참조
 */
//This is I2C Peripherals
#define I2C1_BASEADDR							(APB1PHERIPH_BASE + 0x5400)
#define I2C2_BASEADDR							(APB1PHERIPH_BASE + 0x5800)
#define I2C3_BASEADDR							(APB1PHERIPH_BASE + 0x5C00)

//This is SPI Peripherals
#define SPI2_BASEADDR							(APB1PHERIPH_BASE + 0x3800)
#define SPI3_BASEADDR							(APB1PHERIPH_BASE + 0x3C00)

//This is USART Peripherals
#define USART2_BASEADDR							(APB1PHERIPH_BASE + 0x4400)
#define USART3_BASEADDR							(APB1PHERIPH_BASE + 0x4800)

//this is UART Peripherals
#define UART4_BASEADDR							(APB1PHERIPH_BASE + 0x4C00)
#define UART5_BASEADDR							(APB1PHERIPH_BASE + 0x5000)

// 주석처리
/*
#define SPI2_CR1								(SPI2_BASEADDR + 0x00)		//SPI Control Register 1
#define SPI2_CR2								(SPI2_BASEADDR + 0x04)		//SPI Control Register 2
#define SPI2_SR									(SPI2_BASEADDR + 0x08)		//SPI Status Register
#define SPI2_DR									(SPI2_BASEADDR + 0x0C)		//SPI Data Register
#define SPI2_CRCPR								(SPI2_BASEADDR + 0x10)		//SPI CRC Polynomial Register - No I2S Mode
#define SPI2_RXCRCR								(SPI2_BASEADDR + 0x14)		//SPI RX CRC Register - No I2S Mode
#define SPI2_TXCRCR								(SPI2_BASEADDR + 0x18)		//SPI TX CRC Register - No I2S Mode
#define SPI2_I2SCFGR							(SPI2_BASEADDR + 0x1C)		//SPI I2S Configuration Register
#define SPI2_I2SPR								(SPI2_BASEADDR + 0x20)		//SPI I2S Prescaler Register
*/


/*
 * These are not used and were not written in data sheet
 * But, These are written in Reference Manual
 */

//#define UART7_BASEADDR							(APB1PHERIPH_BASE + 0x7800)
//#define UART8_BASEADDR							(APB1PHERIPH_BASE + 0x7C00)




/* BASE Addresses of peripherals
 * hanging on APB2  Bus
 * APB2에 있는 주변장치를 규정한다
 * TIM 1/8/9/10/11
 * USART 1/6
 * SPI 1
 * ADC 1/2/3
 * EXTI
 * 이외 기타 등등
 */



//This is USART Peripherals
#define USART1_BASEADDR							(APB2PHERIPH_BASE + 0x1000)
#define USART6_BASEADDR							(APB2PHERIPH_BASE + 0x1400)

//This is SPI Peripherals
#define SPI1_BASEADDR							(APB2PHERIPH_BASE + 0x3000)
#define SPI4_BASEADDR							(APB2PHERIPH_BASE + 0x3400)

//This is System Configuration
#define SYSCFG_BASEADDR							(APB2PHERIPH_BASE + 0x3800)

//This is External Interrupt
#define	EXTI_BASEADDR							(APB2PHERIPH_BASE + 0x3C00)


/*
 * 주변장치 레지스터 구조체
 * MCU에 있는 Peripheral 규정
 * Have to check Reference Manual
 */

/* GPIO Structure*/
typedef struct
{
	__vo uint32_t MODER;							//Input, Output, Analog, Alternative Function Select Register
	__vo uint32_t OTYPER;							//If some pins are selected output, define register used by output type
	__vo uint32_t OSPEEDR;							//If GPIO is configured by output, Speed configure register
	__vo uint32_t PUPDR;							//GPIO Pull-up, Pull-down Register
	__vo uint32_t IDR;								//GPIO Input data register, read word only
	__vo uint32_t ODR;								//GPIO Output Data register. Can read and write. Can change used by BSRR register
	__vo uint32_t BSRR;								//Port x reset, set bit
	__vo uint32_t LCKR;								//??????
	__vo uint32_t AFRL;								//Alternative Function Low register
	__vo uint32_t AFRH;								//Alternative Function High register
}GPIO_RegDef_t;

/*RCC Register Structure*/
typedef struct
{
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;
	__vo uint32_t Reserved0;
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
	__vo uint32_t Reserved1;
	__vo uint32_t Reserved2;
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;
	__vo uint32_t Reserved3;
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
	__vo uint32_t Reserved4;
	__vo uint32_t Reserved5;
	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;
	__vo uint32_t Reserved6;
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
	__vo uint32_t Reserved7;
	__vo uint32_t Reserved8;
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	__vo uint32_t Reserved9;
	__vo uint32_t Reserved10;
	__vo uint32_t SSCGR;
	__vo uint32_t PLLLI2SCFGR;
	__vo uint32_t PLLSAICFGR;
	__vo uint32_t DCKCFGR;
}RCC_RegDef_t;

/*EXTI 구조체*/
typedef struct
{
	__vo uint32_t IMR;
	__vo uint32_t EMR;
	__vo uint32_t RTSR;
	__vo uint32_t FTSR;
	__vo uint32_t SWIER;
	__vo uint32_t PR;
}EXTI_RegDef_t;

/*SYSCFG 레지스터 구조체*/
typedef struct
{
	__vo uint32_t MEMRMP;
	__vo uint32_t PMC;
	__vo uint32_t EXTICR[4];
	uint32_t Reserved[2];
	__vo uint32_t CMPCR;
}SYSCFG_RegDef_t;

/*SPI 레지스터 구조체*/
typedef struct
{
	__vo uint32_t CR1;
	__vo uint32_t CR2;
	__vo uint32_t SR;
	__vo uint32_t DR;
	__vo uint32_t CRCPR;			//CRC Polynomial Register
	__vo uint32_t RXCRCR;			//CRC Register
	__vo uint32_t TXCRCR;
	__vo uint32_t I2SCFGR;
	__vo uint32_t I2SPR;
}SPI_RegDef_t;

/* SPI 주메모리 지정 */
#define SPI1									((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2									((SPI_RegDef_t *)SPI2_BASEADDR)
#define SPI3									((SPI_RegDef_t *)SPI3_BASEADDR)
#define SPI4									((SPI_RegDef_t *)SPI4_BASEADDR)

/* GPIO Definition*/

#define GPIOA									((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB									((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC									((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD									((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE									((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF									((GPIO_RegDef_t *)GPIOF_BASEADDR)
#define GPIOG									((GPIO_RegDef_t *)GPIOG_BASEADDR)
#define GPIOH									((GPIO_RegDef_t *)GPIOH_BASEADDR)
//#define GPIOI									((GPIO_RegDef_t *)GPIOI_BASEADDR)

/*RCC Definition*/
#define RCC										((RCC_RegDef_t *)RCC_BASEADDR)

#define EXTI									((EXTI_RegDef_t *)EXTI_BASEADDR)

#define SYSCFG									((SYSCFG_RegDef_t *)SYSCFG_BASEADDR)


/*
 * SPI CR1 매크로
 */
#define SPI_CR1_CPHA										0
#define SPI_CR1_CPOL										1
#define SPI_CR1_MSTR										2
#define SPI_CR1_BR											3	//3~5
#define SPI_CR1_SPE											6
#define SPI_CR1_LSBFIRST									7
#define SPI_CR1_SSI											8
#define SPI_CR1_SSM											9
#define SPI_CR1_RXONLY										10
#define SPI_CR1_DFF											11
#define SPI_CR1_BIDIOE										14
#define SPI_CR1_BIDIMODE									15

/*
 * SPI CR2 매크로
 */
#define SPI_CR2_SSOE 										0
#define SPI_CR2_TXDMAEN 									1
#define SPI_CR2_RXDMAEN										2
#define SPI_CR2_FRF											4
#define SPI_CR2_ERRIE										5
#define SPI_CR2_RXEIE										6
#define SPI_CR2_TXEIE										7


/*
 * SPI SR 매크로
 */
#define SPI_SR_RXNE											0
#define SPI_SR_TXE											1
#define SPI_SR_CHSIDE										2
#define SPI_SR_UDR											3
#define SPI_SR_CRCERR										4
#define SPI_SR_MODF											5
#define SPI_SR_OVR											6
#define SPI_SR_BSY											7
#define SPI_SR_FRE											8



/*
 * Clock Enable Macro GPIOx
 * GPIO에 클럭을 활성화하는 매크로 작성
 */
#define	GPIOA_PCLK_EN()				(RCC->AHB1ENR |= (1 << 0))	//GPIOA Enable
#define	GPIOB_PCLK_EN()				(RCC->AHB1ENR |= (1 << 1))	//GPIOB Enable
#define	GPIOC_PCLK_EN()				(RCC->AHB1ENR |= (1 << 2))	//GPIOC Enable
#define	GPIOD_PCLK_EN()				(RCC->AHB1ENR |= (1 << 3))	//GPIOD Enable
#define	GPIOE_PCLK_EN()				(RCC->AHB1ENR |= (1 << 4))	//GPIOE Enable
#define	GPIOF_PCLK_EN()				(RCC->AHB1ENR |= (1 << 5))	//GPIOF Enable
#define	GPIOG_PCLK_EN()				(RCC->AHB1ENR |= (1 << 6))	//GPIOG Enable
#define	GPIOH_PCLK_EN()				(RCC->AHB1ENR |= (1 << 7))	//GPIOH Enable
//#define	GPIOI_PCLK_EN()				(RCC->AHB1ENR |= (1 << 8))	//GPIOI Enable


/*
 * Clock Disable Macro GPIOx
 * GPIO에 클럭을 비활성화 하는 매크로 작성
 */
#define	GPIOA_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 0))	//GPIOA Disable
#define	GPIOB_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 1))	//GPIOB Disable
#define	GPIOC_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 2))	//GPIOC Disable
#define	GPIOD_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 3))	//GPIOD Disable
#define	GPIOE_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 4))	//GPIOE Disable
#define	GPIOF_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 5))	//GPIOF Disable
#define	GPIOG_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 6))	//GPIOG Disable
#define	GPIOH_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 7))	//GPIOH Disable
//#define	GPIOI_PCLK_DI()				(RCC->AHB1ENR &= ~(1 << 8))	//GPIOI Disable

/*
 * Clock Enable I2C Peripheral Enable
 * I2C에 클럭을 활성화 하는 매크로 작성
 */
#define	I2C1_PCLK_EN()				(RCC->APB1ENR |= (1<<21))
#define	I2C2_PCLK_EN()				(RCC->APB1ENR |= (1<<22))
#define	I2C3_PCLK_EN()				(RCC->APB1ENR |= (1<<23))

/*
 * Clock Disable I2C Peripheral Enable
 * I2C 클럭 비활성화 매크로 작성
 */
#define	I2C1_PCLK_DI()				(RCC->APB1ENR &= ~(1<<21))
#define	I2C2_PCLK_DI()				(RCC->APB1ENR &= ~(1<<22))
#define	I2C3_PCLK_DI()				(RCC->APB1ENR &= ~(1<<23))


/*
 * Clock Enable SPI Peripheral Enable
 * SPI 클럭 활성화 매크로
 */
#define	SPI1_PCLK_EN()				(RCC->APB2ENR |= (1<<12))
#define	SPI2_PCLK_EN()				(RCC->APB1ENR |= (1<<14))
#define	SPI3_PCLK_EN()				(RCC->APB1ENR |= (1<<15))
#define	SPI4_PCLK_EN()				(RCC->APB2ENR |= (1<<13))
/*
 * Clock Disable SPI Peripheral Enable
 * SPI 클럭 비활성화 매크로
 */
#define	SPI1_PCLK_DI()				(RCC->APB2ENR &= ~(1<<12))
#define	SPI2_PCLK_DI()				(RCC->APB1ENR &= ~(1<<14))
#define	SPI3_PCLK_DI()				(RCC->APB1ENR &= ~(1<<15))
#define	SPI4_PCLK_DI()				(RCC->APB2ENR &= ~(1<<13))

/*
 * Clock Enable USART
 * UART/USART 클럭 활성화 매크로
 */
#define	USART1_PCLK_EN()			(RCC->APB2ENR |= (1<<4))
#define	USART2_PCLK_EN()			(RCC->APB1ENR |= (1<<17))
#define	USART3_PCLK_EN()			(RCC->APB1ENR |= (1<<18))
#define UART4_PCLK_EN()				(RCC->APB1ENR |= (1<<19))
#define UART5_PCLK_EN()				(RCC->APB1ENR |= (1<<20))
#define USART6_PCLK_EN()			(RCC->APB2ENR |= (1<<5))

/*
 * Clock Disable USART
 * UART/USART 클럭 비활성화 매크로
 */
#define	USART1_PCLK_DI()			(RCC->APB2ENR &= ~(1<<4))
#define	USART2_PCLK_DI()			(RCC->APB1ENR &= ~(1<<17))
#define	USART3_PCLK_DI()			(RCC->APB1ENR &= ~(1<<18))
#define UART4_PCLK_DI()				(RCC->APB1ENR &= ~(1<<19))
#define UART5_PCLK_DI()				(RCC->APB1ENR &= ~(1<<20))
#define USART6_PCLK_DI()			(RCC->APB2ENR &= ~(1<<5))



/*
 * GPIO 포트를 초기화하는 매크로
 * do while 루프를 사용하여 2개 이상의 명령어를 실행할 수 있게 한다
 * 이 경우 while(0)이면 do 이후의 코드를 실행하고
 * while을 확인하여 false이기 때문에 while루프가 돌지 않음. 결국 한 번만 실행된다.
 */
#define GPIOA_REG_RESET()			do{ (RCC->AHB1RSTR |= (1<<0)); (RCC->AHB1RSTR &= ~(1<<0)); }while(0)
#define GPIOB_REG_RESET()			do{ (RCC->AHB1RSTR |= (1<<1)); (RCC->AHB1RSTR &= ~(1<<1)); }while(0)
#define GPIOC_REG_RESET()			do{ (RCC->AHB1RSTR |= (1<<2)); (RCC->AHB1RSTR &= ~(1<<2)); }while(0)
#define GPIOD_REG_RESET()			do{ (RCC->AHB1RSTR |= (1<<3)); (RCC->AHB1RSTR &= ~(1<<3)); }while(0)
#define GPIOE_REG_RESET()			do{ (RCC->AHB1RSTR |= (1<<4)); (RCC->AHB1RSTR &= ~(1<<4)); }while(0)
#define GPIOF_REG_RESET()			do{ (RCC->AHB1RSTR |= (1<<5)); (RCC->AHB1RSTR &= ~(1<<5)); }while(0)
#define GPIOG_REG_RESET()			do{ (RCC->AHB1RSTR |= (1<<6)); (RCC->AHB1RSTR &= ~(1<<6)); }while(0)
#define GPIOH_REG_RESET()			do{ (RCC->AHB1RSTR |= (1<<7)); (RCC->AHB1RSTR &= ~(1<<7)); }while(0)
//#define GPIOI_REG_RESET()			do{ (RCC->AHB1RSTR |= (1<<8)); (RCC->AHB1RSTR &= ~(1<<8)); }while(0)


/*
 * SPI 포트 초기화 매크로
 * GPIO와 같이 do-while 사용할 것
 *
 */
#define SPI1_REG_RESET()			do{ (RCC->APB2RSTR |= (1<<12)); (RCC->APB2RSTR &= ~(1<12)); }while(0)
#define SPI2_REG_RESET()			do{ (RCC->APB1RSTR |= (1<<14)); (RCC->APB1RSTR &= ~(1<14)); }while(0)
#define SPI3_REG_RESET()			do{ (RCC->APB1RSTR |= (1<<15)); (RCC->APB1RSTR &= ~(1<15)); }while(0)
#define SPI4_REG_RESET()			do{ (RCC->APB2RSTR |= (1<<13)); (RCC->APB2RSTR &= ~(1<13)); }while(0)


/*인터럽트 넘버 설정
 * 레퍼런스 매뉴얼 12.2 테이블 확인
 * Interrupt Request number
 */

#define IRQ_NO_EXTI0				6
#define IRQ_NO_EXTI1				7
#define IRQ_NO_EXTI2				8
#define IRQ_NO_EXTI3				9
#define IRQ_NO_EXTI4				10
#define IRQ_NO_EXTI9_5				23
#define IRQ_NO_EXTI15_10			40


/*
 * 인터럽트 우선순위 매크로
 */
#define NVIC_IRQ_PRIO0				0
#define NVIC_IRQ_PRIO15				15

/* 인터럽트 GPIO 선택 매크로*/
#define GPIO_BASEADDR_TO_CODE(x)	((x == GPIOA) ? 0 :\
									(x == GPIOB) ? 1 :\
									(x == GPIOC) ? 2 :\
									(x == GPIOD) ? 3 :\
									(x == GPIOE) ? 4 :\
									(x == GPIOF) ? 5 :\
									(x == GPIOG) ? 6 :\
									(x == GPIOH) ? 7 : 0)
//									(x == GPIOI) ? 8 :

/*
 * Clock Enable SYSCLK
 *
 */
#define SYSCFG_PCLK_EN()			(RCC->APB2ENR |= (1<<14))

/*
 * Clock Disable SYSCLK
 */
#define SYSCFG_PCLK_DI()			(RCC->APB2ENR &= ~(1<<14))





/*일반 매크로*/

#define	ENABLE						1
#define DISABLE						0
#define SET							ENABLE
#define RESET						DISABLE
#define GPIO_PIN_SET				SET
#define GPIO_PIN_RESET				RESET
#define FLAG_RESET					RESET
#define FLAG_SET					SET

/*
 * 종속 되어있는 헤더 파일일 경우
 * 맨 마지막에 include해주어도 가능
 */

#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#endif /* INC_STM32F407XX_H_ */
