/*
 * it.c
 *
 *  Created on: 15-Sep-2020
 *      Author: Raj.S
 */
#include "main.h"


//flag
extern uint8_t buttonPressed;


void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);

	buttonPressed ^= GPIO_PIN_SET;				//Toggle the flag using XOR

}


