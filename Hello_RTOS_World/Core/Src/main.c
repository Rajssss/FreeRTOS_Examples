/*
 * main.c
 *
 *  Created on: Sep 3, 2020
 *      Author: Raj.S
 *      Description: FreeRTOS Hello world Example.
 *      			 This examples creates two Tasks to transmit data via UART2 and
 *      			 schedule it with equal priority then start it.
 *
 *
 */

#include "main.h"


#define AVAILABLE 			1
#define NOT_AVAILABLE 		0



TaskHandle_t xTask1_Handle = NULL;
TaskHandle_t xTask2_Handle = NULL;

UART_HandleTypeDef UART2Init;

#ifdef SEMIHOSTING_ENABLE
//semihosting
extern void initialise_monitor_handles(void);
#endif

char pData[50];

uint8_t UART_ACCESS_KEY = AVAILABLE;


int main(void)
{
#ifdef SEMIHOSTING_ENABLE
	//semihosting
	initialise_monitor_handles();

	printf("Application Started.\n\r");
#endif

	//Initialize UART2
	prvUART2Init();

	//create two tasks
	xTaskCreate(vTask1_Handler, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, &xTask1_Handle);

	xTaskCreate(vTask2_Handler, "Task2", configMINIMAL_STACK_SIZE, NULL, 2, &xTask2_Handle);

#ifdef SEMIHOSTING_ENABLE
	printf("Tasks created.\n\r");
#endif

	//start the scheduler
	vTaskStartScheduler();

	while(1);


	return 0;
}



void SystemClockConfig(void)
{
	//use default clock config
}



//Task1 Handler
void vTask1_Handler(void *params)
{
	memset(pData, 0, sizeof(pData));
	sprintf(pData, "Task1 Running.\n\r");

	while(1)
	{
		/*
		 * As both the tasks has same priority, but when any of these tasks is executing
		 * the other task is not getting chance to execute completely, as before it can complete
		 * a context switch is happening. So we have to make a key to give access of UART to a task
		 * only when another task is not using it, and when the task completes its usage it should
		 * manually give a chance to other tasks via taskYIELD().
		 */
		if(UART_ACCESS_KEY == AVAILABLE)
		{
			UART_ACCESS_KEY = NOT_AVAILABLE;
			taskDISABLE_INTERRUPTS();
			HAL_UART_Transmit(&UART2Init, (uint8_t *)pData, strlen(pData), HAL_MAX_DELAY);
			UART_ACCESS_KEY = AVAILABLE;
			taskYIELD();
			taskENABLE_INTERRUPTS();
		}
	}
	memset(pData, 0, sizeof(pData));
}


//Task2 Handler
void vTask2_Handler(void *params)
{
	memset(pData, 0, sizeof(pData));
	sprintf(pData, "Task2 Running.\n\r");

	while(1)
	{
		/*
		 * As both the tasks has same priority, but when any of these tasks is executing
		 * the other task is not getting chance to execute completely, as before it can complete
		 * a context switch is happening. So we have to make a key to give access of UART to a task
		 * only when another task is not using it, and when the task completes its usage it should
		 * manually give a chance to other tasks via taskYIELD().
		 */
		if(UART_ACCESS_KEY == AVAILABLE)
		{
			taskDISABLE_INTERRUPTS();
			UART_ACCESS_KEY = NOT_AVAILABLE;
			HAL_UART_Transmit(&UART2Init, (uint8_t *)pData, strlen(pData), HAL_MAX_DELAY);
			UART_ACCESS_KEY = AVAILABLE;
			taskYIELD();
			taskENABLE_INTERRUPTS();
		}
	}
	memset(pData, 0, sizeof(pData));
}



void Error_handler(void)
{
	while(1);
}



void prvUART2Init(void)
{
	UART2Init.Instance = USART2;
	UART2Init.Init.BaudRate = 115200;
	UART2Init.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART2Init.Init.Mode = UART_MODE_TX_RX;
	UART2Init.Init.Parity = UART_PARITY_NONE;
	UART2Init.Init.StopBits = UART_STOPBITS_1;
	UART2Init.Init.WordLength = UART_WORDLENGTH_8B;
	if(HAL_UART_Init(&UART2Init) != HAL_OK)
	{
		Error_handler();
	}
}



