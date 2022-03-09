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
static volatile uint8_t flag=0;
static uint8_t sin=0;

//static TimerHandle_t timerChecker;
//void vCallbackFunction ( TimerHandle_t xTimer );
//
static void UART_TransmitTask(void *parameters)
{

	uartMsg buffer;
	while (1)
	{
			xQueueReceive(UART_TransmitQueueHandle, &buffer, portMAX_DELAY);
			if(buffer.idUart==1){
				HAL_UART_Transmit_IT(&huart1, &buffer.string, sizeof(uint8_t));
			}else{
				HAL_UART_Transmit_IT(&huart5, &buffer.string, sizeof(uint8_t));
			}

			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart->Instance==huart1.Instance || huart->Instance==huart5.Instance){
		sin=1;
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






static void UART_ReceiveTask(void *parameters)
{
	int currUart=(int)parameters;
	uint8_t buffer;
	while (1)
	{
		if(currUart==5){
			HAL_UART_Receive_IT(&huart5, &buffer, sizeof(uint8_t));
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			xQueueSendToBack(UART5_ReceiveQueueHandle, &buffer, portMAX_DELAY);

		}else if(currUart==1){
			HAL_UART_Receive_IT(&huart1, &buffer, sizeof(uint8_t));
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
			xQueueSendToBack(UART1_ReceiveQueueHandle, &buffer, portMAX_DELAY);
		}

	}
}



extern int flagovi;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart5.Instance)
	{
		flagovi=1;
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART5_ReceiveTaskHandle, &woken);
		portYIELD_FROM_ISR(woken);
	}else if(huart->Instance== huart1.Instance){
		flag=1;
		BaseType_t woken = pdFALSE;
		vTaskNotifyGiveFromISR(UART1_ReceiveTaskHandle, &woken);
		portYIELD_FROM_ISR(woken);
	}
}

// GENERAL
// -----------------------------------------------------------------------------

void UART_Init()
{	xTaskCreate(UART_TransmitTask, "receiveTask", 64, NULL, 17, &UART_TransmitTaskHandle);
	UART_TransmitQueueHandle = xQueueCreate(128, sizeof(uartMsg));
	UART_TransmitMutexHandle = xSemaphoreCreateMutex();

	xTaskCreate(UART_ReceiveTask, "receiveTask", 64, 1, 20, &UART1_ReceiveTaskHandle);
	xTaskCreate(UART_ReceiveTask, "receiveTask", 64, 5, 18, &UART5_ReceiveTaskHandle);
	UART5_ReceiveQueueHandle = xQueueCreate(64, sizeof(uint8_t));
	UART1_ReceiveQueueHandle = xQueueCreate(64, sizeof(uint8_t));
	UART_ReceiveMutexHandle = xSemaphoreCreateMutex();

//	timerChecker=xTimerCreate("timeChecker", pdMS_TO_TICKS(timeCheckerUart), pdFALSE, NULL, vCallbackFunction);

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

//void UART_AsyncTransmitCharacter(char character)
//{
//	xSemaphoreTake(UART_TransmitMutexHandle, portMAX_DELAY);
//
//	xQueueSendToBack(UART_TransmitQueueHandle, &character, portMAX_DELAY);
//
//	xSemaphoreGive(UART_TransmitMutexHandle );
//}



void UART_AsyncTransmitString(int id,uint8_t const string[])
{
	if (string != NULL)
		{
			xSemaphoreTake(UART_TransmitMutexHandle, portMAX_DELAY);

			for (uint32_t i = 0; i < strlen(string); i++)
			{
				uartMsg tmp={string[i],id};
				xQueueSendToBack(UART_TransmitQueueHandle, &tmp, portMAX_DELAY);
			}

			xSemaphoreGive(UART_TransmitMutexHandle);
		}
}

//void UART_AsyncTransmitDecimal(uint32_t decimal)
//{
//	xSemaphoreTake(UART_TransmitMutexHandle, portMAX_DELAY);
//	char digits[32];
//	uint32_t index = 32;
//	while (index >= 0 && decimal != 0)
//	{
//		digits[--index] = '0' + decimal % 10;
//		decimal /= 10;
//	}
//
//	for (uint32_t i = index; i < 32; i++)
//	{
//		xQueueSendToBack(UART_TransmitQueueHandle, digits + i, portMAX_DELAY);
//	}
//
//	xSemaphoreGive(UART_TransmitMutexHandle);
//}

// RECEIVE UTIL
// -----------------------------------------------------------------------------

//char UART_BlockReceiveCharacter()
//{
//	xSemaphoreTake(UART_ReceiveMutexHandle, portMAX_DELAY);
//
//	char character;
//	xQueueReceive(UART5_ReceiveQueueHandle, &character, portMAX_DELAY);
//
//	xSemaphoreGive(UART_ReceiveMutexHandle);
//
//	return character;
//}

char* UART_BlockReceiveString(int id)
{
	xSemaphoreTake(UART_ReceiveMutexHandle, portMAX_DELAY);

	char *string = pvPortMalloc(64);

	if (string != NULL)
	{
		uint32_t index = 0;

		char character = '\0';
		while (character != '\r' && index < 64)
		{
			if(id==5){
				xQueueReceive(UART5_ReceiveQueueHandle, &character, portMAX_DELAY);
			}else {
				xQueueReceive(UART1_ReceiveQueueHandle, &character, portMAX_DELAY);
			}

			string[index++] = character;
		}

		string[--index] = '\0';
	}

	xSemaphoreGive(UART_ReceiveMutexHandle);

//	UART_AsyncTransmitString(5, string);

	return string;
}

//uint32_t UART_BlockReceiveDecimal()
//{
//	xSemaphoreTake(UART_ReceiveMutexHandle, portMAX_DELAY);
//
//	uint32_t decimal = 0;
//
//	char character = '\0';
//	while (character != '\r')
//	{
//		xQueueReceive(UART_ReceiveQueueHandle, &character, portMAX_DELAY);
//		if (character >= '0' && character <= '9')
//		{
//			decimal = decimal * 10 + (character - '0');
//		}
//	}
//
//	xSemaphoreGive(UART_ReceiveMutexHandle);
//
//	return decimal;
//}
