/*
 * main.c
 *
 *  Created on: Sep 12, 2020
 *      Author: Raj.S
 *      Description: This Example creates a task:
 *      			 task_LED: To handle On-Board User LED.
 *      			 This Example demonstrates the LED blink with GPIO Interrupt for user Button.
 *      			 The ISR is used to SET the buttonPressed flag.
 *      			 Based on the status of the buttonPressed, the LED will be SET or RESET.
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

	//create task
	xTaskCreate(vTask_LED_Handler, "TaskLED", configMINIMAL_STACK_SIZE, NULL, 5, NULL);				//Handle structure not required

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




