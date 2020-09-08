/*
 * main.h
 *
 *  Created on: Sep 3, 2020
 *      Author: Raj.S
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_


//Standard Headers
#include <stdio.h>
#include <string.h>
#include <stdint.h>

//Driver/HAL Layer
#include "stm32f4xx_hal.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"


//Prototypes
void SystemClockConfig(void);
void vTask1_Handler(void *params);
void vTask2_Handler(void *params);
void prvUART2Init(void);
void printmsg(char *msg);
void Error_handler(void);



#endif /* INC_MAIN_H_ */
