/*
 * uart_driver.h
 *
 *  Created on: Feb 23, 2022
 *      Author: mihailozar
 */

#ifndef INC_UART_DRIVER_H_
#define INC_UART_DRIVER_H_

#include <stdint.h>

extern void UART_Init();

extern void UART_AsyncTransmitCharacter(char character);
extern void UART_AsyncTransmitString(int id,char const *string);
extern void UART_AsyncTransmitDecimal(uint32_t decimal);

extern char UART_BlockReceiveCharacter();
extern char* UART_BlockReceiveString(int id);
extern uint32_t UART_BlockReceiveDecimal();

typedef struct UartMess{
	uint8_t string;
	int idUart;
}uartMsg;

#endif /* INC_UART_DRIVER_H_ */
