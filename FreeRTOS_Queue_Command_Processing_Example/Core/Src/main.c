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
TaskHandle_t Menu_handle = NULL;
TaskHandle_t CMD_handle = NULL;
TaskHandle_t CMD_Procesor_handle = NULL;
TaskHandle_t UART_write_handle = NULL;


//Queue Handle
QueueHandle_t CMD_Queue = NULL;
QueueHandle_t UART_WRITE_Queue = NULL;


//Command Structure
typedef struct APP_CMD
{
	uint8_t CMD_NUM;
	uint8_t CMD_ARGS[10];
}APP_CMD_ty;


//Command Menu
char CMD_menu[]= {
	"FreeRTOS Queue Command Processing\r\n"
	"1. LED On\r\n"
	"2. LED OFF\r\n"
	"3. LED TOGGLE_ON\r\n"
	"4. LED TOGGLE_OFF\r\n"
	"5. RTC_PRINT_DATE_TIME\r\n"
	"6. EXIT_APP\r\n"
	"Enter your selection number => "};



int main(void)
{
	//Initialize GPIO for On-board User LED and Button
	prvGPIOInit();

	//Retarget STDIO to UART2
	RetargetInit(USART2);


	//Queues (Must be created before starting the scheduler)
	CMD_Queue = xQueueCreate(10, sizeof(APP_CMD_ty *));			//10x4 = 40 Bytes queue.
	if(CMD_Queue == NULL)
	{
		Error_Handler();
	}

	UART_WRITE_Queue = xQueueCreate(10, sizeof(char *));
	if(UART_WRITE_Queue == NULL)
	{
		Error_Handler();
	}


	//Tasks
	xTaskCreate(vTask1_menu_display, "UART-Menu-Task", 500, NULL, 1, &Menu_handle);
	xTaskCreate(vTask2_cmd_handling, "UART-CMD-Task", 500, NULL, 1, &CMD_handle);
	xTaskCreate(vTask3_cmd_processor, "UART-CMD_Processor-Task", 500, NULL, 1, &CMD_Procesor_handle);
	xTaskCreate(vTask4_uart_write, "UART-Write-Task", 500, NULL, 1, &UART_write_handle);


	//Start Scheduler
	vTaskStartScheduler();

	return 0;
}


void Error_Handler(void)
{
	printf("Error Handler: you have fucked up!\n\r");
	while(1);
}



void vTask1_menu_display(void *params)
{
	char *pData = CMD_menu;
	while(1)
	{
		xQueueSend(UART_WRITE_Queue, &pData, portMAX_DELAY);

		//wait for command
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

	}
}


void vTask2_cmd_handling(void *params)
{

	while(1)
	{
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
		xQueueSend(CMD_Queue, &sel, portMAX_DELAY);
	}
}


void vTask3_cmd_processor(void *params)
{
	APP_CMD_ty *newCMD = NULL;

	while(1)
	{

	}
}


void vTask4_uart_write(void *params)
{
	char *pData = NULL;
	APP_CMD_ty *newCMD = NULL;

	while(1)
	{
		xQueueReceive(UART_WRITE_Queue, &pData, portMAX_DELAY);
		printf(pData);
		scanf("%d", newCMD->CMD_NUM);
		xTaskNotify(CMD_handle, 0, eNoAction);
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
	}
}


