/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: May 19, 2025
 *      Author: user
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_
/********************************************************************
 * 해당 헤더 파일은 GPIO의 연결, 속성, 동작 등을 규정하는
 * 함수 및 코드만을 작성한다.
 * - GPIOx의 클럭 연결
 * - GPIOx의 클럭 해제
 * - GPIOx 핀의 속성 설정
 * - GPIOx 핀의 동작 설정
 * - GPIOx의 구조체
 */
#include "stm32f407xx.h"

typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;						// GPIO 포트 모드 설정 매크로 참조
	uint8_t GPIO_PinSpeed;						// GPIO 핀 출력 속도 설정 매크로 참조
	uint8_t GPIO_PinPuPdControl;				// GPIO 핀 풀업 풀다운 매크로
	uint8_t GPIO_PinOPType;						// GPIO 포트 아웃풋 타입 설정 매크로
	uint8_t GPIO_AltFunMode;

}GPIO_PinConfig_t;

/*
 * GPIO pin handle structure
 */

typedef struct
{
	// GPIO 장치들의 기본값을 포인터로 지정
	GPIO_RegDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;

}GPIO_Handle_t;

/*
 * GPIO 핀 번호 설정 매크로
 * 핀 번호가 0부터 15까지이므로 미리 매크로 작성
 */
#define GPIO_PIN_0									0
#define GPIO_PIN_1									1
#define GPIO_PIN_2									2
#define GPIO_PIN_3									3
#define GPIO_PIN_4									4
#define GPIO_PIN_5									5
#define GPIO_PIN_6									6
#define GPIO_PIN_7									7
#define GPIO_PIN_8									8
#define GPIO_PIN_9									9
#define GPIO_PIN_10									10
#define GPIO_PIN_11									11
#define GPIO_PIN_12									12
#define GPIO_PIN_13									13
#define GPIO_PIN_14									14
#define GPIO_PIN_15									15



/*
 * GPIO 포트 모드 설정 매크로
 * 핀별로 설정 가능 32비트 각 2비트 사용
 * 레퍼런스 매뉴얼 8.4.1 GPIO port mode register
 */

#define	GPIO_MODE_INPUT								0
#define	GPIO_MODE_OUTPUT							1
#define	GPIO_MODE_ALTFN								2
#define	GPIO_MODE_ANALOG							3
#define GPIO_MODE_INT_FT							4
#define GPIO_MODE_INT_RT							5
#define GPIO_MODE_INT_FT_RT							6

/*
 * GPIO 포트 아웃풋 타입 설정 매크로
 * 핀별로 설정 가능 32비트 중 하위 16비트를 1비트씩 사용
 * 레퍼런스 매뉴얼 8.4.2 GPIO port output type register
 */

#define	OUTPUT_OP_TYPE_PUSH_PULL					0
#define OUTPUT_OP_TYPE_OPEN_DRAIN					1

/*
 * GPIO 핀 출력 속도 설정 매크로
 * 핀별로 설정 가능 32비트 각 2비트 사용
 * 레퍼런스 매뉴얼 8.4.3 GPIO port output speed register
 */

#define OUTPUT_SPD_LOW								0
#define OUTPUT_SPD_MID								1
#define OUTPUT_SPD_HIGH								2
#define OUTPUT_SPD_VERY_HIGH						3


/*
 * GPIO 핀 풀업 풀다운 매크로
 * 핀별로 설정 가능 32비트 각 2비트 사용
 * 레퍼런스 매뉴얼 8.4.4 GPIO port pull-up/pull-down register
 */

#define GPIO_NO_PP_PD								0
#define GPIO_PU										1
#define GPIO_PD										2
#define OUTPUT_RESERVED								3//사용하지 않음
/**********************************************************
 * API
 */


/*
 * GPIO CLK Set
 * GPIO 클럭 연결 여부
 */

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/*
 * GPIO Init-Deinit
 * GPIO
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandles);

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Data read/write
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);


/*
 * IRQ Configuration
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority);

void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
