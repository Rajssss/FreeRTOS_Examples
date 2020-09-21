/*
 * main.c
 *
 *  Created on: Sep 12, 2020
 *      Author: Raj.S
 *      Description: This Example creates two tasks:
 *      			 vTask_LED_Blink_200ms: Blinks the LED per 200ms.
 *      			 vTask_LED_Blink_1s: Blinks the LED per 1Sec.
 *      			 This Example demonstrates the task delete feature of FreeRTOS.
 *      			 vTask_LED_Blink_1s deletes itself when user button is pressed.
 *
 */

#include "main.h"


//Use On-Board User LED and Button
#define GPIO_LED		GPIO_PIN_5
#define GPIO_BUTTON		GPIO_PIN_13


//defined outside main
extern void prvGPIOInit();


//task handles
TaskHandle_t LED_Blink_200ms_Handle = NULL;
TaskHandle_t LED_Blink_1s_Handle = NULL;



int main(void)
{
	//Initialize GPIO for On-board User LED and Button
	prvGPIOInit();

	//create tasks
	xTaskCreate(vTask_LED_Blink_200ms, "LED_Blink_1s", configMINIMAL_STACK_SIZE, NULL, 1, &LED_Blink_200ms_Handle);				//never run till tas2 exists with higher priority
	xTaskCreate(vTask_LED_Blink_1s, "LED_Blink_200ms", configMINIMAL_STACK_SIZE, NULL, 2, &LED_Blink_1s_Handle);				//as it has higher priority, it will always run

	//Start Scheduler
	vTaskStartScheduler();

	return 0;
}



void vTask_LED_Blink_200ms(void *param)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_LED);
		HAL_Delay(200);
	}
}


void vTask_LED_Blink_1s(void *param)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_LED);
		HAL_Delay(1000);
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_BUTTON) == GPIO_PIN_RESET)
		{
			break;
		}
	}
	vTaskDelete(LED_Blink_1s_Handle);
}




