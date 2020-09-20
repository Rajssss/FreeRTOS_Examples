/*
 * main.c
 *
 *  Created on: Sep 12, 2020
 *      Author: Raj.S
 *      Description: This Example creates two tasks:
 *      			 task_LED: To handle On-Board User LED.
 *      			 task_Button: To handle the On-Board User Button.
 *      			 This Example demonstrates the LED blink with two different tasks
 *      			 with task Notification and wait feature.
 *
 */

#include "main.h"


//Use On-Board User LED and Button
#define GPIO_LED		GPIO_PIN_5
#define GPIO_BUTTON		GPIO_PIN_13


//defined outside main
extern void prvGPIOInit();


//Task Handles
TaskHandle_t Button_handle = NULL;
TaskHandle_t LED_Handle = NULL;


int main(void)
{
	//Initialize GPIO for On-board User LED and Button
	prvGPIOInit();

	//create tasks
	xTaskCreate(vTask_LED_Handler, "TaskLED", 500, NULL, 5, &LED_Handle);				//Stack Size= 500 Words ~ 2KB
	xTaskCreate(vTask_Button_Handler, "TaskBtn", 500, NULL, 5, &Button_handle);

	//Start Scheduler
	vTaskStartScheduler();

	return 0;
}



void vTask_LED_Handler(void *param)
{
	while(1)
	{
		if(xTaskNotifyWait(0, 0, NULL, portMAX_DELAY) == pdTRUE)
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_LED);
		}
	}

}



void vTask_Button_Handler(void *param)
{
	while(1)
	{
		if(HAL_GPIO_ReadPin(GPIOC, GPIO_BUTTON) == GPIO_PIN_RESET)			//When User button is pressed its grounded
		{
			HAL_Delay(100);
			xTaskNotify(LED_Handle, 0, eNoAction);
		}
	}

}


//delay using Ticks
void rtos_delay(uint32_t delay_in_ms)
{
	uint32_t current_tick_count = xTaskGetTickCount();

	uint32_t delay_in_ticks = (delay_in_ms * configTICK_RATE_HZ ) /1000 ;

	while(xTaskGetTickCount() <  (current_tick_count + delay_in_ticks));

}

