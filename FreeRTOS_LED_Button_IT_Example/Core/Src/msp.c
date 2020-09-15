/*
 * msp.c
 *
 *  Created on: Sep 12, 2020
 *      Author: Raj.S
 */

#include "main.h"


void prvGPIOInit(void)
{
	//Initialize On-Board User LED and Button

	GPIO_InitTypeDef GPIO_UserLED = {0};
	GPIO_InitTypeDef GPIO_UserButton = {0};

	//Enable Clock for GPIOA an GPIOC
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	//LED
	GPIO_UserLED.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_UserLED.Pull = GPIO_NOPULL;
	GPIO_UserLED.Speed = GPIO_SPEED_LOW;
	GPIO_UserLED.Pin = GPIO_PIN_5;

	HAL_GPIO_Init(GPIOA, &GPIO_UserLED);

	//Button
	GPIO_UserButton.Mode = GPIO_MODE_INPUT;
	GPIO_UserButton.Pull = GPIO_NOPULL;
	GPIO_UserButton.Speed = GPIO_SPEED_LOW;
	GPIO_UserButton.Pin = GPIO_PIN_13;

	HAL_GPIO_Init(GPIOC, &GPIO_UserButton);

}


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

