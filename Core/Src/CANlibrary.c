/*
 * CANlibrary.c
 *
 *  Created on: Feb 24, 2022
 *      Author: mihailozar
 */


#include "CANlibrary.h"
#include "master.h"

uint32_t TxMailbox;
CANMsg *msg;

extern int prechargeFlag;
extern int ecuSHDReqFlag;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_RxHeaderTypeDef pHeader;
	uint8_t rxData[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &pHeader, rxData);
//	canSend(pHeader.StdId, rxData);
	CANMsg msg={ &pHeader, rxData};
//	xQueueSendToBackFromISR(CAN_Rx_Queue,&(msg), portMAX_DELAY );
	 //From ECATU
	        if (pHeader.StdId == 0x097)
	        {
	            if (rxData[0] == 1)
	            {
	                //start PrechargeProcess
	                if (prechargeFlag != 1)
	                    prechargeFlag = 1;
	            }
	        }
	        //From ECATU
	        if (pHeader.StdId == 0x300)
	        {
	            if (rxData[2] & 0x40)
	            {
	                //open SHUT DOWN Circuit
	                if (ecuSHDReqFlag != 1)
	                    ecuSHDReqFlag = 1;
	            }
	        }

//	        canSend(pHeader.StdId, rxData);

}


//void getCANMessage() {
//	//Provera ima li primljene poruke
//	xQueueReceive(CAN_Rx_Queue, &(msg), 0);
//
//	uint32_t messageID = msg->pHeader.StdId;
//	uint8_t *rxData = msg->data;
//	canSend(messageID, rxData);
//
//
//}


//static void can_task(void *parameters){
//	while(1){
//		int i=uxQueueSpacesAvailable(CAN_Rx_Queue);
//		if(uxQueueSpacesAvailable(CAN_Rx_Queue)<10){
//			xQueueReceive(CAN_Rx_Queue, &(msg), portMAX_DELAY);
//			uint32_t messageID = msg->headId;
//			uint8_t *rxData = msg->data;
//			canSend(messageID, rxData);
//		UART_AsyncTransmitString(5, string);
//		vTaskDelay(pdMS_TO_TICKS(500));
//		}
//	}
//}
//void create_CanTask(){
//
//		xTaskCreate(can_task, "Can_task", 32, NULL, 10, &canHandler);
//		CAN_Rx_Queue = xQueueCreate(32, sizeof(struct CANMsg *));
//		CANMutex = xSemaphoreCreateMutex();
//
//
//}

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
	HAL_CAN_Start(&hcan1);

}


void canSend(uint16_t id, uint8_t* canMsg){

	CAN_TxHeaderTypeDef pHeader;
		pHeader.DLC = 8;
		pHeader.RTR = CAN_RTR_DATA;
		pHeader.IDE = CAN_ID_STD;
		pHeader.StdId = id;
	HAL_CAN_AddTxMessage(&hcan1,&pHeader , canMsg, &TxMailbox);
	while (HAL_CAN_IsTxMessagePending(&hcan1, TxMailbox));
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
