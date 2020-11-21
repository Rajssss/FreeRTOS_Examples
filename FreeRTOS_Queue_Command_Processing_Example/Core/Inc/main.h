/*
 * main.h
 *
 *  Created on: Sep 12, 2020
 *      Author: Raj.S
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

//Standard Headers
#include <stdint.h>

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"

//HAL
#include "stm32f4xx_hal.h"


//Retarget IO to UART2
#include "retarget_stdio.h"


//Prototypes
void prvGPIOInit(void);
void prvLED_Toggle(void);
void prvLED_Toggle_Start(void);
void prvLED_Toggle_Stop(void);
void prvRTC_GET_DATE_TIME(void);



//Error Hanbdler
void Error_Handler(void);


//task handlers
void vTask1_menu_display(void *params);
void vTask2_cmd_handling(void *params);
void vTask3_uart_handler(void *params);


#endif /* INC_MAIN_H_ */
