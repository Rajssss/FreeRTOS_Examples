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
TaskHandle_t UART_write_handle = NULL;


//Queue Handle
QueueHandle_t CMD_Queue = NULL;
QueueHandle_t UART_WRITE_Queue = NULL;


//Software Timer Handle
TimerHandle_t LED_Toggle_Timer_Handle;



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
	"3. LED BLINK_ON\r\n"
	"4. LED BLINK_OFF\r\n"
	"5. RTC_PRINT_DATE_TIME\r\n"
	"6. EXIT_APP\r\n"
	"Enter your selection number => "};



int main(void)
{
	//Initialize GPIO for On-board User LED and Button
	prvGPIOInit();

	//Retarget STDIO to UART2
	RetargetInit(USART2);

	//printf("UART Init Done!\n");


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
	xTaskCreate(vTask2_cmd_handling, "UART-CMD-Handler-Task", 500, NULL, 1, &CMD_handle);
	xTaskCreate(vTask3_uart_handler, "UART-Handler-Task", 500, NULL, 1, &UART_write_handle);


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
	APP_CMD_ty *newCMD;

	while(1)
	{
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		xQueueReceive(CMD_Queue, &newCMD, portMAX_DELAY);

		switch (newCMD->CMD_NUM)
		{
			case 1:
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
				xTaskNotify(Menu_handle, 0, eNoAction);
				break;

			case 2:
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
				xTaskNotify(Menu_handle, 0, eNoAction);
				break;

			case 3:
				prvLED_Toggle_Start();
				xTaskNotify(Menu_handle, 0, eNoAction);
				break;

			case 4:
				prvLED_Toggle_Stop();
				xTaskNotify(Menu_handle, 0, eNoAction);
				break;

			case 5:
				//prvRTC_GET_DATE_TIME();
				xTaskNotify(Menu_handle, 0, eNoAction);
				break;

			case 6:
				//Exit the Application
				break;

			default:
				xTaskNotify(Menu_handle, 0, eNoAction);
				break;

		}

		//vPortFree(newCMD);

	}
}





void vTask3_uart_handler(void *params)
{
	char *pData = NULL;
	APP_CMD_ty *newCMD = (APP_CMD_ty *) pvPortMalloc(sizeof(APP_CMD_ty));

	while(1)
	{
		xQueueReceive(UART_WRITE_Queue, &pData, portMAX_DELAY);
		printf(pData);
		scanf("%d", &newCMD->CMD_NUM);

		xQueueSend(CMD_Queue, &newCMD, portMAX_DELAY);

		xTaskNotify(CMD_handle, 0, eNoAction);
		//xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
	}

}




void prvLED_Toggle_Start()
{
	if(LED_Toggle_Timer_Handle == NULL)
	{
		LED_Toggle_Timer_Handle = xTimerCreate("LED-Toggle-Timer", 100, pdTRUE, NULL, prvLED_Toggle);

		xTimerStart(LED_Toggle_Timer_Handle, portMAX_DELAY);
	}

	else
	{
		xTimerStart(LED_Toggle_Timer_Handle, portMAX_DELAY);

	}

}



void prvLED_Toggle(void)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}




void prvLED_Toggle_Stop()
{
	xTimerStop(LED_Toggle_Timer_Handle, portMAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}





void prvRTC_GET_DATE_TIME(void)
{


}




