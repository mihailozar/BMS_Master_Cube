/*
 * uart_driver.h
 *
 *  Created on: Feb 23, 2022
 *      Author: mihailozar
 */

#ifndef INC_UART_DRIVER_H_
#define INC_UART_DRIVER_H_

#include <stdint.h>

extern void UART_AsyncTransmitString(int id,uint8_t const string[]);
extern void UART_Receive(int id, int len);

typedef struct UartMess{
	uint8_t string;
	int idUart;
}uartMsg;

#endif /* INC_UART_DRIVER_H_ */
