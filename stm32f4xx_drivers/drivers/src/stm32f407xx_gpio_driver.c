/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: May 19, 2025
 *      Author: user
 */




#include "stm32f407xx_gpio_driver.h"
/*********************************************************************
 * @함수명					-	GPIO_PeriClockControl
 *
 * @설명						-	함수를 이용하여 GPIOx에 클럭 연결/해제 진행한다
 *
 *
 * @매개변수					-	GPIO의 시작 메모리 주소
 * @매개변수					-	ENABLE 혹은 DISABLE 매크로
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	//GPIO Peripheral Clock Enable
	//GPIO 주변장치 클럭 연결
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
//		else if(pGPIOx == GPIOI)
//		{
//			GPIOI_PCLK_EN();
//		}
	}
	//GPIO Peripheral Clock Disable
	//GPIO 주변장치 클럭 해제
	else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}
		else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
//		else if(pGPIOx == GPIOI)
//		{
//			GPIOI_PCLK_DI();
//		}
	}
}
/*
 * GPIO Init-Deinit
 * GPIO
 */

/*********************************************************************
 * @함수명					-	GPIO_Init
 *
 * @설명						-	함수의 Input/Output/Alternative 등의
 * 								기능 설정 진행 - Pin 단위로 책정
 *
 * @매개변수					-	GPIOx, GPIO 핀 설정값을 갖는 구조체 변수
 * @매개변수					-
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */

void GPIO_Init(GPIO_Handle_t *pGPIOHandles)
{

	/*
	 *
	1. GPIO 핀 모드 설정
		00: Input (reset state)
		01: General purpose output mode
		10: Alternate function mode
		11: Analog mode
	2. 속도 설정
		00: Low speed
		01: Medium speed
		10: High speed
		11: Very high speed
	3. 풀업/풀다운, 오픈드레인 여부 설정
		00: No pull-up, pull-down
		01: Pull-up
		10: Pull-down
		11: 미사용
	4. 출력 타입 설정
		0: Output push-pull (reset state)
		1: Output open-drain
	5. 기능출력 사용 여부 Alternate function low/high
		0000: AF0
		0001: AF1
		0010: AF2
		0011: AF3
		0100: AF4
		0101: AF5
		0110: AF6
		0111: AF7
		1000: AF8
		1001: AF9
		1010: AF10
		1011: AF11
		1100: AF12
		1101: AF13
		1110: AF14
		1111: AF15
	 *
	 */
	uint32_t temp = 0;

	//GPIO 그룹 핀 인에이블을 여기서 진행하면 계속 할 필요 없음
	GPIO_PeriClockControl(pGPIOHandles->pGPIOx, ENABLE);

	//1번 설정 - 인터럽트 제외 코드 작성
	if(pGPIOHandles->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
//	if(pGPIOHandles->GPIO_PinConfig.GPIO_PinMode < GPIO_MODE_INT_FT)
	{
		//GPIO핀 모드가 인터럽트 모드보다 작을때 -> 인터럽트 모드는 4부터 시작
		//GPIO핀마다 2비트 사용하므로 핀 번호의 2배만큼 시프트
		//ex) 1번핀 -> 1 * 2 >>> 레지스터 2비트 부터 시작
		//    10번핀 -> 10 * 2 >>> 레지스터 20비트 부터 시작 21번 비트까지
		temp = (pGPIOHandles->GPIO_PinConfig.GPIO_PinMode <<(2 * pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber));

		//이후 temp 값을 레지스터에 저장한다
		//GPIO_Handle_t 구조체는 GPIO_RegDef_t *pGPIOx와 GPIO_PinConfig_t GPIO_PinConfig 모두 가지고 있는 구조체임
		pGPIOHandles->pGPIOx->MODER &= ~(0x03 << pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber); //비트 클리어
		pGPIOHandles->pGPIOx->MODER |= temp;

	}
	else
	{
		if(pGPIOHandles->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_INT_FT)		//Falling Edge 일 때
		{
			//Falling Edge 레지스터 규정
			EXTI->FTSR |= (1 << pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber);
			//어떠한 이유로 인해 Rising Edge가 규정되어 있으수도 있으므로 클리어
			EXTI->RTSR &= ~(1 << pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandles->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_INT_RT)		//Rising Edge 일 때
		{
			//Rising Edge 레지스터 규정
			EXTI->RTSR |= (1 << pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber);
			//어떠한 이유로 인해 Rising Edge가 규정되어 있으수도 있으므로 클리어
			EXTI->FTSR &= ~(1 << pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandles->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_INT_FT_RT)		//Both Edge 일 때
		{
			//양쪽 에지 사용
			EXTI->RTSR |= (1 << pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber);
			//양쪽의 에지 모두 활성화 하므로
			EXTI->FTSR |= (1 << pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber);
		}

		//GPIO 보트 설정 SYSCFG_EXTICR
		uint8_t temp1 = (pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber / 4);
		uint8_t temp2 = (pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber % 4);
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandles->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode <<(temp2 * 4);


		//IMR에 전달할 exti 인에이블
		EXTI->IMR |= (1 << pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber);
	}

	temp = 0;

	//2번 설정 핀 속도
	temp = (pGPIOHandles->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandles->pGPIOx->OSPEEDR &= ~(0x03 <<(2* pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber)); //비트 클리어
	pGPIOHandles->pGPIOx->OSPEEDR |= temp;

	temp =0;

	//3번 설정 풀업 풀다운
	temp = (pGPIOHandles->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandles->pGPIOx->PUPDR &= ~(0x03 << (2 * pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber)); //비트 클리어
	pGPIOHandles->pGPIOx->PUPDR |= temp;

	temp = 0;

	//4번 설정 출력 타입 설정
	//16비트 설정함 r각 1비트
	temp = (pGPIOHandles->GPIO_PinConfig.GPIO_PinOPType << ( pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandles->pGPIOx->OTYPER &= ~(0x01 << pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber); //비트 클리어
	pGPIOHandles->pGPIOx->OTYPER |= temp;

	temp = 0;

	//5번 ALT 기능 모드 설정
	if(pGPIOHandles->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		uint32_t temp1, temp2;

		temp1 = pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandles->GPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandles->pGPIOx->AFR[temp1] &= ~(0xF << ( 4 * temp2 ) ); //clearing
		pGPIOHandles->pGPIOx->AFR[temp1] |= (pGPIOHandles->GPIO_PinConfig.GPIO_AltFunMode << ( 4 * temp2 ) );
		/*if(temp1 == 0)
		{
			pGPIOHandles->pGPIOx->AFRL &= ~(0xF << (4 * temp2) );
			pGPIOHandles->pGPIOx->AFRL |= (0xF << (4 * temp2) );
		}
		if(temp2 == 1)
		{
			pGPIOHandles->pGPIOx->AFRH &= ~(0xF << (4 * temp2) );
			pGPIOHandles->pGPIOx->AFRH |= (pGPIOHandles->GPIO_PinConfig.GPIO_AltFunMode << (4 * temp2) );
		}*/
	}


}

/*********************************************************************
 * @함수명					-	GPIO_DeInit
 *
 * @설명						-	Init으로 설정되었던 기능을 초기화 하는 함수
 *
 * @매개변수					-	GPIOx 구조체
 * @매개변수					-
 * @매개변수					-
 *
 * @반환값					-	None
 *
 * @추가 내용					-	None
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOB_PCLK_EN();
	}
	else if(pGPIOx == GPIOC)
	{
		GPIOC_PCLK_EN();
	}
	else if(pGPIOx == GPIOD)
	{
		GPIOD_PCLK_EN();
	}
	else if(pGPIOx == GPIOE)
	{
		GPIOE_PCLK_EN();
	}
	else if(pGPIOx == GPIOF)
	{
		GPIOF_PCLK_EN();
	}
	else if(pGPIOx == GPIOG)
	{
		GPIOG_PCLK_EN();
	}
	else if(pGPIOx == GPIOH)
	{
		GPIOH_PCLK_EN();
	}
//	else if(pGPIOx == GPIOI)
//	{
//		GPIOI_PCLK_EN();
//	}
}
/*
 * Data read/write
 */

/*********************************************************************
 * @함수명					-	GPIO_ReadFromInputPin
 *
 * @설명						-	설정된 pin의 입력값의 High/Low 판단
 *
 * @매개변수					-	GPIOx 구조체 변수
 * @매개변수					-	핀 번호(16개 중 1개)
 * @매개변수					-
 *
 * @반환값					-	핀 입력 High or Low (1 혹은 0)
 *
 * @추가 내용					-	None
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	//지역변수 하나 사용
	uint8_t value;
	//해당 지역변수로 값을 저장하기 위해 Input Data Register(IDR)의 레지스터를 핀 수만큼
	//오른쪽으로 시프트 하고, 0x00000001을 논리 곱셈하여 값이 1인지 0인지 확인한다.
	//반드시 형 변환(type casting) 사용
	value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);
	//이후 값을 리턴한다.
	return value;
}

/*********************************************************************
 * @함수명					-	GPIO_ReadFromInputPort
 *
 * @설명						-	GPIO 포트단위의 입력값을 확인
 *
 * @매개변수					-	GPIOx 구조체 변수 -> 포트 단위
 * @매개변수					-
 * @매개변수					-
 *
 * @반환값					-	16bit 단위의 데이터
 *
 * @추가 내용					-	None
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	//포트단위 입력값 확인
	uint16_t value;
	value = (uint16_t)(pGPIOx->IDR);
	return value;
}

/*********************************************************************
 * @함수명					-	GPIO_WriteToOutputPin
 *
 * @설명						-	설정된 pin의 출력을 설정
 *
 * @매개변수					-	GPIOx 구조체 변수 GPIOx
 * @매개변수					-	핀 번호
 * @매개변수					-	핀 출력 High or Low (1 혹은 0)
 *
 * @반환값					-	없음
 *
 * @추가 내용					-	Pin 단위로 설정
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		//1을 쓴다
		pGPIOx->ODR |= (1 << PinNumber);
	}
	else
	{
		pGPIOx->ODR &= ~(1 << PinNumber);
	}

}

/*********************************************************************
 * @함수명					-	GPIO_WriteToOutputPort
 *
 * @설명						-	설정된 pin의 출력을 설정
 *
 * @매개변수					-	GPIOx 구조체 변수 GPIOx 포트 그룹
 * @매개변수					-	16비트 값
 * @매개변수					-
 *
 * @반환값					-	없음
 *
 * @추가 내용					-	포트 단위로 설정
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

/*********************************************************************
 * @함수명					-	GPIO_ToggleOutputPin
 *
 * @설명						-	설정된 핀의 출력을 반전시킴
 *
 * @매개변수					-	GPIOx 구조체 변수 GPIOx
 * @매개변수					-	핀 번호
 * @매개변수					-
 *
 * @반환값					-	없음
 *
 * @추가 내용					-	포트 단위로 설정
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
//	pGPIOx->ODR = pGPIOx->ODR ^ (1 << PinNumber);

	pGPIOx->ODR ^= (1 << PinNumber);

	/*
	uint8_t value=0;

	value = ~((pGPIOx->ODR >> PinNumber) & (0x00000001));
	pGPIOx->ODR &= (value<<PinNumber);
	*/
}

/*
 * IRQ Configuration
 */
/*********************************************************************
 * @함수명					-	GPIO_IRQConfig
 *
 * @설명						-	인터럽트 활성화 설정
 *
 * @매개변수					-	인터럽트 넘버
 * @매개변수					-	인터럽트 우선순위
 * @매개변수					-	설정 혹은 해제
 *
 * @반환값					-	없음
 *
 * @추가 내용					-	없음
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
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

/*********************************************************************
 * @함수명					-	GPIO_IRQPriorityConfig
 *
 * @설명						-	인터럽트 우선순위 설정
 *
 * @매개변수					-   IRQNumber
 * @매개변수					-	IRQPriority
 * @매개변수					-
 *
 * @반환값					-	없음
 *
 * @추가 내용					-	없음
 */
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint32_t IRQPriority)
{
	//1 IPR 레지스터 찾기
	uint8_t iprx = IRQNumber / 4; //8비트 구성으로 4개이므로
	uint8_t iprx_section = IRQNumber % 1; // 32바이트에서 시작되는 곳 확인

	uint8_t shift_amount = (8*iprx_section)+(8-NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority<<shift_amount);
	//상위 4비트는 사용 하위 4비트는 미사용

}


/*********************************************************************
 * @함수명					-	GPIO_IRQHandling
 *
 * @설명						-	활성화된 인터럽트 핀 번호 설정
 *
 * @매개변수					-
 * @매개변수					-
 * @매개변수					-
 *
 * @반환값					-	없음
 *
 * @추가 내용					-	없음
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
	//핀 넘버에 해당하는 EXTI PR 레지스터 클리어
	//PR에서 해당되는 핀에 트리거가 발생하여 1로 셋팅되기 때문
	//이것은 내부적으로 1로 세팅되어 이 함수로 오게 되며
	//핀 넘버에 해당하는 인터럽트에 1을 다시 write하면 클리어 된다.
	//0을 쓰면 아무 일도 일어나지 않는다
	if(EXTI->PR &(1<<PinNumber))
	{
		//클리어
		EXTI->PR |= (1 << PinNumber);
	}


}
