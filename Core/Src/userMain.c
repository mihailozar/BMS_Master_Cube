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
#include "slaveConfig.h"



typedef struct CANMessage {
	CAN_RxHeaderTypeDef pHeader;
	uint8_t data[8];
} CANMsg;

extern void InitPL455();
//extern int UART_Init();
extern void PwmInInit();
extern void Can_Init();
extern void canSend(uint16_t id,  CANMsg* canMsg);
extern  void create_CanTask();
extern void shutDownSlavesCommand();
int flagovi=0;



int userMain(void){

//	PwmInInit();
//	create_CanTask();
//	masterInit();
	UART_Init();
//	InitPL455();
//	vTaskDelay(pdMS_TO_TICKS(1000));
//	shutDownSlavesCommand();


	while(1){
//		int f=(int)frequency();
//		char fc[4];
//		int d=(int)dutycycle();
//		char dc[4];
//		itoa(f,fc,10);
//		itoa(d,dc,10);
		char* tmp;
//		tmp=UART_BlockReceiveString(1);
//		UART_BlockReceiveString(5);
//		UART_AsyncTransmitString(5, "slave\n");
		if(flagovi==1){
			UART_AsyncTransmitString(5, "desio se prijem\n");
		}
		flagovi=0;
		vTaskDelay(pdMS_TO_TICKS(500));
		UART_AsyncTransmitString(5, "posle ifa\n");
//		vTaskDelay(pdMS_TO_TICKS(500));
//		UART_AsyncTransmitString(5, "master\n");
//		UART_AsyncTransmitString(5, "du\n");
//		UART_AsyncTransmitString(5, "dc\n");
//		uint8_t niz[8]={0,1,0,1,0,1,0,1};
//		canSend(1, niz);
		vTaskDelay(pdMS_TO_TICKS(500));

	}


	return 0;
}
