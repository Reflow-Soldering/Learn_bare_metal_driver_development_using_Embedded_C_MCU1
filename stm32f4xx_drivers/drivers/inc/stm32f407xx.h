/*
 * stm32f407xx.h
 *
 *  Created on: May 18, 2025
 *      Author: user
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_


/*This is address list of Memory*/
#define	FLASH_BASEADDR							0x08000000U /* FLASH Base Address is here*/
#define SRAM1_BASEADDR							0x20000000U	/* SRAM1 Base Address is here - 112KB*/
#define SRAM2_BASEADDR							0x20001C00U /* SRAM2 Base Address is here - 16KB*/
#define ROM_BASEADDR							0x1FFF0000U /* ROM Base Address is here*/
#define SRAM									SRAM1_BASEADDR /* Equal SRAM1 Base Address*/

/*This is address list of Peripheral*/

#define PERIPH_BASE								0x40000000U
#define APB1PHERIPH_BASE						PERIPH_BASE
#define APB2PHERIPH_BASE						0x40010000U
#define AHB1PHERIPH_BASE						0x40020000U
#define AHB2PHERIPH_BASE						0x50000000U


/* Base Addresses of GPIOx
 * GPIO is haning of AHB1 Bus
 * 잊지 마시오!
 */

#define	GPIOA_BASEADDR							(AHB1PHERIPH_BASE + 0x0000)
#define GPIOB_BASEADDR							(AHB1PHERIPH_BASE + 0x0400)
#define GPIOC_BASEADDR							(AHB1PHERIPH_BASE + 0x0800)
#define GPIOD_BASEADDR							(AHB1PHERIPH_BASE + 0x0C00)
#define GPIOE_BASEADDR							(AHB1PHERIPH_BASE + 0x1000)
#define GPIOF_BASEADDR							(AHB1PHERIPH_BASE + 0x1400)
#define GPIOG_BASEADDR							(AHB1PHERIPH_BASE + 0x1800)
#define GPIOH_BASEADDR							(AHB1PHERIPH_BASE + 0x1C00)
#define GPIOI_BASEADDR							(AHB1PHERIPH_BASE + 0x2000)


/* BASE Addresses of peripherals
 * hanging on APB1  Bus
 *
 *
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

/*
 * These are not used and were not written in data sheet
 * But, These are written in Reference Manual
 */

//#define UART7_BASEADDR							(APB1PHERIPH_BASE + 0x7800)
//#define UART8_BASEADDR							(APB1PHERIPH_BASE + 0x7C00)




/* BASE Addresses of peripherals
 * hanging on APB2  Bus
 *
 *
 */

#define	EXTI_BASEADDR							(APB2PHERIPH_BASE + 0x3C00)

//This is USART Peripherals
#define USART1_BASEADDR							(APB2PHERIPH_BASE + 0x1000)
#define USART6_BASEADDR							(APB2PHERIPH_BASE + 0x1400)

//This is SPI Peripherals
#define SPI1_BASEADDR							(APB2PHERIPH_BASE + 0x3000)
#define SPI4_BASEADDR							(APB2PHERIPH_BASE + 0x3400)

//This is System Configuration
#define SYSCFG_BASEADDR							(APB2PHERIPH_BASE + 0x3800)

#endif /* INC_STM32F407XX_H_ */
