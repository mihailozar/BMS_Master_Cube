/*
 * uart_driver.c
 *
 *  Created on: Feb 23, 2022
 *      Author: mihailozar
 */

#include "uart_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#include <string.h>
#include "usart.h"
#include "slaveConfig.h"

static TaskHandle_t UART_TransmitTaskHandle;
static QueueHandle_t UART_TransmitQueueHandle;

static SemaphoreHandle_t UART_TransmitMutexHandle;

extern TIM_HandleTypeDef htim4;
static volatile uint8_t flag = 0;
static uint8_t sin = 0;

uint8_t uartData5;
uint8_t uartData1;
volatile uint8_t procitano=0;

static TimerHandle_t timerChecker;
void vCallbackFunction ( TimerHandle_t xTimer );
//
static void UART_TransmitTask(void *parameters) {

	uartMsg buffer;
	while (1) {
		xQueueReceive(UART_TransmitQueueHandle, &buffer, portMAX_DELAY);
		if (buffer.idUart == 1) {
			HAL_UART_Transmit_IT(&huart1, &buffer.string, sizeof(uint8_t));
		} else {
			HAL_UART_Transmit_IT(&huart5, &buffer.string, sizeof(uint8_t));
		}

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

	if (huart->Instance == huart1.Instance
			|| huart->Instance == huart5.Instance) {
		sin = 1;
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART_TransmitTaskHandle, &woken);
		portYIELD_FROM_ISR(woken);

	}

}

// RECEIVE
// -----------------------------------------------------------------------------

static TaskHandle_t UART5_ReceiveTaskHandle;
static TaskHandle_t UART1_ReceiveTaskHandle;
static QueueHandle_t UART5_ReceiveQueueHandle;
static QueueHandle_t UART1_ReceiveQueueHandle;
static SemaphoreHandle_t UART_ReceiveMutexHandle;

static void UART_ReceiveTask(void *parameters) {
	int currUart = (int) parameters;
	uint8_t buffer;
	while (1) {
		if (currUart == 5) {
			HAL_UART_Receive_IT(&huart5, &buffer, sizeof(uint8_t));
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			while(procitano==0);
			xQueueSendToBack(UART5_ReceiveQueueHandle, &buffer, portMAX_DELAY);

		} else if (currUart == 1) {
			HAL_UART_Receive_IT(&huart1, &buffer, sizeof(uint8_t));
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			xQueueSendToBack(UART1_ReceiveQueueHandle, &buffer, portMAX_DELAY);
		}

	}
}

extern int flagovi;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	TaskHandle_t pom;
	BaseType_t woken = pdFALSE;
	procitano=1;
	if (huart->Instance == huart5.Instance) {
		flagovi=1;
//		pom=UART5_ReceiveTaskHandle;

		xQueueSendToBackFromISR(UART5_ReceiveQueueHandle, &uartData5, &woken);
		HAL_UART_Receive_IT(&huart5, &uartData5, sizeof(uint8_t));

	} else if (huart->Instance == huart1.Instance) {
//		flag=1;
//		pom=UART1_ReceiveTaskHandle;
		xQueueSendToBackFromISR(UART1_ReceiveQueueHandle, &uartData1, &woken);
		HAL_UART_Receive_IT(&huart1, &uartData1, sizeof(uint8_t));

	}


//	vTaskNotifyGiveFromISR(pom, &woken);
//	portYIELD_FROM_ISR(woken);
}

// GENERAL
// -----------------------------------------------------------------------------

void UART_Init() {

	UART_TransmitQueueHandle = xQueueCreate(128, sizeof(uartMsg));
	UART_TransmitMutexHandle = xSemaphoreCreateMutex();
	xTaskCreate(UART_TransmitTask, "transmitTask", 64, NULL, 20,
			&UART_TransmitTaskHandle);

	UART5_ReceiveQueueHandle = xQueueCreate(128, sizeof(uint8_t));
	UART1_ReceiveQueueHandle = xQueueCreate(32, sizeof(uint8_t));
	UART_ReceiveMutexHandle = xSemaphoreCreateMutex();

//	xTaskCreate(UART_ReceiveTask, "receiveTask1", 64, (void*)1, 20, &UART1_ReceiveTaskHandle);
//	xTaskCreate(UART_ReceiveTask, "receiveTask5", 64, (void*)5, 20, &UART5_ReceiveTaskHandle);

	HAL_UART_Receive_IT(&huart5, &uartData5, sizeof(uint8_t));
	HAL_UART_Receive_IT(&huart1, &uartData1, sizeof(uint8_t));

	timerChecker=xTimerCreate("timeChecker", pdMS_TO_TICKS(timeCheckerUart), pdFALSE, NULL, vCallbackFunction);

}


void vCallbackFunction(TimerHandle_t xTimer){
	if(procitano==0){
		procitano=1;
		uint8_t poruka='\t';
		BaseType_t woken = pdFALSE;
		xQueueSendToBack(UART5_ReceiveQueueHandle,&poruka, portMAX_DELAY);
//		xQueueSendToBackFromISR(UART5_ReceiveQueueHandle, &poruka, &woken);

	}
}
// TRANSMIT UTIL
// -----------------------------------------------------------------------------

//TIMER CALLBACK
//-----------------------------------------------------------------------------

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//
//	if(htim==&htim4){
//		if(flag==1){
//			flag=0;
//			UART_AsyncTransmitString(5, "Dobio poruku na vreme");
//		}else{
//			UART_AsyncTransmitString(5, "Greska nije bilo prijema");
//		}
//		HAL_TIM_Base_Stop(htim);
//		htim->Instance->CNT=0;
//	}
//}

void UART_AsyncTransmitString(int id, uint8_t const string[]) {
	if (string != NULL) {
		xSemaphoreTake(UART_TransmitMutexHandle, portMAX_DELAY);

		for (uint32_t i = 0; i < strlen(string); i++) {
			uartMsg tmp = { string[i], id };
			xQueueSendToBack(UART_TransmitQueueHandle, &tmp, portMAX_DELAY);
		}

		xSemaphoreGive(UART_TransmitMutexHandle);
	}
}

// RECEIVE UTIL
// -----------------------------------------------------------------------------

char UART_BlockReceiveCharacter(int id) {
	xSemaphoreTake(UART_ReceiveMutexHandle, portMAX_DELAY);

	char character;
	if (id == 5) {

		xQueueReceive(UART5_ReceiveQueueHandle, &character, portMAX_DELAY);
	} else {
		xQueueReceive(UART1_ReceiveQueueHandle, &character, portMAX_DELAY);
	}

	xSemaphoreGive(UART_ReceiveMutexHandle);

	return character;
}

char* UART_BlockReceiveString(int id) {
	xSemaphoreTake(UART_ReceiveMutexHandle, portMAX_DELAY);

	char *string = pvPortMalloc(64);

	if (string != NULL) {
		uint32_t index = 0;

		char character = '\0';
		procitano=0;
//		xTimerReset(timerChecker, portMAX_DELAY);
		xTimerStart(timerChecker,portMAX_DELAY);
		while (character != '\n' && character!='\t'&& index < 64) {
			if (id == 5) {

				xQueueReceive(UART5_ReceiveQueueHandle, &character,
						portMAX_DELAY);

			} else {

				xQueueReceive(UART1_ReceiveQueueHandle, &character,
						portMAX_DELAY);
				procitano=1;
			}

			string[index++] = character;
		}

		if(character=='\t'){
			string[index++] = character;
		}
		string[--index] = '\0';
	}
	procitano=0;

	xSemaphoreGive(UART_ReceiveMutexHandle);

	return string;
}

void resetQueue() {
	xQueueReset(UART5_ReceiveQueueHandle);
}
void dealokacijaMemorije(char *string){
	vPortFree(string);
}

