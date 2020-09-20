/*
 * main.h
 *
 *  Created on: Sep 12, 2020
 *      Author: Raj.S
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

//Standard Headers
#include "stdint.h"

//FreeRTOS
#include "FreeRTOS.h"
#include "task.h"

//HAL
#include "stm32f4xx_hal.h"


//Prototypes
void vTask_LED_Handler(void *param);
void vTask_Button_Handler(void *param);
void prvGPIOInit(void);
void rtos_delay(uint32_t delay_in_ms);

#endif /* INC_MAIN_H_ */
