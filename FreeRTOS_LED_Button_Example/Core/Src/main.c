/*
 * main.c
 *
 *  Created on: Sep 12, 2020
 *      Author: Raj.S
 *      Description: This Example creates two tasks:
 *      			 task_LED: To handle On-Board User LED.
 *      			 task_Button: To handle the On-Board User Button.
 *      			 This Example demonstrates the LED blink with two different tasks.
 *
 */

#include "main.h"


//Use On-Board User LED and Button
#define GPIO_LED		GPIO_PIN_5
#define GPIO_BUTTON		GPIO_PIN_13


//defined outside main
extern void prvGPIOInit();

//Global Flag
uint8_t buttonPressed = RESET;


int main(void)
{
	//Initialize GPIO for On-board User LED and Button
	prvGPIOInit();

	//create tasks
	xTaskCreate(vTask_LED_Handler, "TaskLED", configMINIMAL_STACK_SIZE, NULL, 5, NULL);				//Handle structure not required
	xTaskCreate(vTask_Button_Handler, "TaskBtn", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

	//Start Scheduler
	vTaskStartScheduler();

	return 0;
}



void vTask_LED_Handler(void *param)
{
	while(1)
	{
		//As user button is already pulled to high, so reverse logic
		if(buttonPressed == SET)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_LED, GPIO_PIN_SET);
		}
		else if(buttonPressed == RESET)
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_LED, GPIO_PIN_RESET );
		}
	}

}



void vTask_Button_Handler(void *param)
{
	while(1)
	{
		buttonPressed = HAL_GPIO_ReadPin(GPIOC, GPIO_BUTTON) ? RESET : SET;
	}

}




