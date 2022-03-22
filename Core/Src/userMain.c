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
extern void resetQueue();
int flagovi=0;
TaskHandle_t mainTask;

void userMainInit(){

	Can_Init();
	UART_Init();
//	create_CanTask();
	xTaskCreate(userMain, "UserMainThread", 64, NULL, 20, &mainTask);
}


int userMain(void){

//	PwmInInit();

//	masterInit();
//	InitPL455();
//	vTaskDelay(pdMS_TO_TICKS(1000));
//	shutDownSlavesCommand();

char* desavanje="desio se prijem\n";
char* posle="posle ifa\n";
	while(1){
//		int f=(int)frequency();
//		char fc[4];
//		int d=(int)dutycycle();
//		char dc[4];
//		itoa(f,fc,10);
//		itoa(d,dc,10);




//		if(flagovi==1){
			char* tmp;
			tmp=UART_BlockReceiveString(5);
			UART_AsyncTransmitString(5, tmp);
			UART_AsyncTransmitString(5, desavanje);
//			flagovi=0;
//
//		}else{

		vTaskDelay(pdMS_TO_TICKS(500));
		UART_AsyncTransmitString(5, posle);
//		}

//		vTaskDelay(pdMS_TO_TICKS(500));
//		UART_AsyncTransmitString(5, "master\n");
//		UART_AsyncTransmitString(5, "du\n");
//		UART_AsyncTransmitString(5, "dc\n");
//		uint8_t niz[8]={0,1,0,1,0,1,0,1};
//		canSend(1, niz);
		vTaskDelay(pdMS_TO_TICKS(1000));

	}


	return 0;
}
