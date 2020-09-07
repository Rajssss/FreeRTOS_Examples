/*
 * msp.c
 *
 *  Created on: 04-Sep-2020
 *      Author: Raj.S
 */

#include "main.h"


void HAL_MspInit(void)
{
	//Setup Priority Grouping
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//Enable Required exceptions (here ex: USGFAULT, BUSFAULT, MEMFAULT)
	SCB->SHCSR |= (0x7 << 16);

	//Configure Priority of the exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}



void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_UART2_Init = {0};

	//Enable clock of USART2 and GPIOA
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();;

	GPIO_UART2_Init.Alternate = GPIO_AF7_USART2;
	GPIO_UART2_Init.Mode = GPIO_MODE_AF_PP;
	GPIO_UART2_Init.Pull = GPIO_PULLUP;
	GPIO_UART2_Init.Speed = GPIO_SPEED_FAST;
	GPIO_UART2_Init.Pin = GPIO_PIN_2 | GPIO_PIN_3;

	HAL_GPIO_Init(GPIOA, &GPIO_UART2_Init);

}


