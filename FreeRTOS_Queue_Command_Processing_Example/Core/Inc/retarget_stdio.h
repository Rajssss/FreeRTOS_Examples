/*
 * retarget_stdio.h
 *
 *  Created on: 17-Oct-2020
 *      Author: Raj.S
 */

#ifndef INC_RETARGET_STDIO_H_
#define INC_RETARGET_STDIO_H_


#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

void RetargetInit(USART_TypeDef *UARTx);


#endif /* INC_RETARGET_STDIO_H_ */
