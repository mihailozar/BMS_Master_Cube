/*
 * userMain.c
 *
 *  Created on: Feb 23, 2022
 *      Author: mihailozar
 */

#include "userMian.h"
#include "PwmIn.h"
#include "uart_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include "can.h"


typedef struct CANMessage {
	CAN_RxHeaderTypeDef pHeader;
	uint8_t data[8];
} CANMsg;

extern void InitPL455();
//extern int UART_Init();
extern void PwmInInit();
extern void Can_Init();
extern void canSend(uint16_t id,  CANMsg* canMsg);



int userMain(void){

	UART_Init();
	PwmInInit();
	Can_Init();
//	masterInit();
//	InitPL455();


	while(1){
		int f=(int)frequency();
		char fc[4];
		int d=(int)dutycycle();
		char dc[4];
		itoa(f,fc,10);
		itoa(d,dc,10);
		UART_AsyncTransmitString(5, "fr");
		UART_AsyncTransmitString(5, fc);
		UART_AsyncTransmitString(5, "du");
		UART_AsyncTransmitString(5, dc);
		uint8_t niz[8]={0,1,0,1,0,1,0,1};
		canSend(0, niz);
		vTaskDelay(pdMS_TO_TICKS(500));

	}


	return 0;
}
