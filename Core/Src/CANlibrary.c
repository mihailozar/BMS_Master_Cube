/*
 * CANlibrary.c
 *
 *  Created on: Feb 24, 2022
 *      Author: mihailozar
 */


#include "CANlibrary.h"
#include "master.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


TaskHandle_t canHandler;
QueueHandle_t CAN_Rx_Queue;
SemaphoreHandle_t CANMutex;
uint32_t TxMailbox;
CANMsg *msg;



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_RxHeaderTypeDef pHeader;
	uint8_t rxData[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &pHeader, rxData);
//	canSend(pHeader.StdId, rxData);
	CANMsg msg={ &pHeader, rxData};
	xQueueSendToBackFromISR(CAN_Rx_Queue,&(msg), portMAX_DELAY );


}


void getCANMessage() {
	//Provera ima li primljene poruke
	xQueueReceive(CAN_Rx_Queue, &(msg), 0);

	uint32_t messageID = msg->pHeader.StdId;
	uint8_t *rxData = msg->data;
	canSend(messageID, rxData);


}


static void can_task(void *parameters){
	while(1){
		if(uxQueueSpacesAvailable(CAN_Rx_Queue)<10){
			xSemaphoreTake(CANMutex, portMAX_DELAY);
			getCANMessage();
			xSemaphoreGive(CANMutex);
		}
	}
}
void create_CanTask(){
		CAN_Rx_Queue = xQueueCreate(10, sizeof(struct CANMessage));
		CANMutex = xSemaphoreCreateMutex();
		xTaskCreate(can_task, "Can_task", 128, NULL, 30, &canHandler);
		HAL_CAN_Start(&hcan1);
}

void Can_Init(){


	CAN_FilterTypeDef CanFilter;
	CanFilter.FilterIdHigh = 0x0000;
	CanFilter.FilterIdLow = 0;
	CanFilter.FilterMaskIdHigh = 0x0000;
	CanFilter.FilterMaskIdLow = 0;
	CanFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CanFilter.FilterBank = 0;
	CanFilter.FilterMode = CAN_FILTERMODE_IDMASK;
	CanFilter.FilterScale = CAN_FILTERSCALE_32BIT;
	CanFilter.FilterActivation = CAN_FILTER_ENABLE;


	HAL_CAN_ConfigFilter(&hcan1, &CanFilter);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

}


void canSend(uint16_t id, uint8_t canMsg[]){

	CAN_TxHeaderTypeDef pHeader;
		pHeader.DLC = 8;
		pHeader.RTR = CAN_RTR_DATA;
		pHeader.IDE = CAN_ID_STD;
		pHeader.StdId = id;
	HAL_CAN_AddTxMessage(&hcan1,&pHeader , canMsg, &TxMailbox);
//	while (HAL_CAN_IsTxMessagePending(&hcan1, TxMailbox));
}




//*** Pack 2B data into CAN messages
//Pakuje unit16_t u dva podatka od po 1B, na poziciji pos u CAN poruci
void pack_data_2B( CANMsg* canMsg, uint16_t data, uint8_t position){
	 canMsg->data[position << 1] = data & 0xFF;
	 canMsg->data[(position << 1) + 1] = data >> 8;
}
//*** Pack 1B data into CAN message
//Pakuje unit16_t na poziciji pos u CAN poruci
void pack_data_1B( CANMsg* canMsg, uint16_t data, uint8_t position){
	 canMsg->data[position] = data & 0xFF;
}
