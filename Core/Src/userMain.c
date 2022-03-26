/*
 * userMain.c
 *
 *  Created on: Feb 23, 2022
 *      Author: mihailozar
 */

#include "userMian.h"
#include "PwmIn.h"
#include "uart_driver.h"
#include "can.h"
#include "slaveConfig.h"





typedef struct CANMessage {
	CAN_RxHeaderTypeDef pHeader;
	uint8_t data[8];
} CANMsg;

extern void InitPL455();

extern void PwmInInit();
extern void Can_Init();
extern void canSend(uint16_t id,  CANMsg* canMsg);
extern void shutDownSlavesCommand();
extern void clearUartBuffer51();
int flagovi=0;
extern char recBuf1[32];
extern char recBuf5[32];
int prechargeFlag = 0;
int ecuSHDReqFlag = 0;
extern TIM_HandleTypeDef htim5;
extern int WriteReg(char bID, uint16_t wAddr, uint64_t dwData, char bLen, char bWriteType) ;
#define DEVICE_COMMAND          2       //Command Register
#define FRMWRT_SGL_R            0x00    // single device write with response
extern uint8_t* voltageBuff[10];

void userMainInit(){

	Can_Init();
}

volatile int counter=0;

uint8_t* voltageBuff[10];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	WriteReg(counter, DEVICE_COMMAND, 0x00, 1, FRMWRT_SGL_R); //Current Value
	WriteReg(counter, DEVICE_COMMAND, 0x20, 1, FRMWRT_SGL_R); //Volatge request
	counter=(counter+1)%10;
	UART_Receive(1, 17);



}

int userMain(void){

//	PwmInInit();

	masterInit();
	InitPL455();
	HAL_TIM_Base_Start_IT(&htim5);
//	vTaskDelay(pdMS_TO_TICKS(1000));
//	shutDownSlavesCommand();

char* desavanje="desio se prijem\n";
char* posle="posle ifa\n";
char* test="mnbvcxzasdfghjklpoiuytrewqzxcva\n";
	while(1){

			;

		HAL_Delay(500);
		UART_AsyncTransmitString(5, posle);

		HAL_Delay(1000);
		uint8_t rxData[8] = {0};

		canSend(0x10, rxData);


	}


	return 0;
}
