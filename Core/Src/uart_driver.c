/*
 * uart_driver.c
 *
 *  Created on: Feb 23, 2022
 *      Author: mihailozar
 */

#include "uart_driver.h"

#include <string.h>
#include "usart.h"
#include "slaveConfig.h"



extern TIM_HandleTypeDef htim4;
static volatile uint8_t flag = 0;
extern uint8_t* voltageBuff[10];
uint8_t uartData5;
uint8_t uartData1;
volatile uint8_t procitano=0;
extern int counter;
extern TIM_HandleTypeDef htim5;


uint8_t recBuf1[32];
uint8_t recBuf5[32];

volatile int receiveCnt=0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if(huart->Instance==&htim5.Instance){
		voltageBuff[receiveCnt]=recBuf1;
		receiveCnt=(receiveCnt+1)%10;
		if(abs(receiveCnt-counter)>1){
			voltageBuff[receiveCnt]="greska";
		}
	}
}

uartMsg tmp ;
void UART_AsyncTransmitString(int id, uint8_t const trStr[]) {
	if (trStr != NULL) {
		if(id==1){
			HAL_UART_Transmit(&huart1, trStr, sizeof(uint8_t)*strlen(trStr), 100);
		}else{
			HAL_UART_Transmit(&huart5, trStr, sizeof(uint8_t)*strlen(trStr), 100);
		}
	}
}

// RECEIVE UTIL
// -----------------------------------------------------------------------------

void UART_Receive(int id,int len){
	if(id==1){
		HAL_UART_Receive_IT(&huart1, recBuf1, len);
	}else{
		HAL_UART_Receive_IT(&huart5, recBuf5, len);
	}
}

void clearUartBuffer5(){
	for(int i=0; i<32;i++){
			recBuf5[i]='\0';
		}
}
void clearUartBuffer51(){
	for(int i=0; i<32;i++){
			recBuf1[i]='\0';
		}
}
