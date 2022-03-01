/*
 * master.c
 *
 *  Created on: Feb 24, 2022
 *      Author: mihailozar
 */
#include "gpio.h"
#include "master.h"
#include "slaveConfig.h"
#include "uart_driver.h"

extern char recBuff[1024];
extern int num;


//*** General State Fault Variables
extern uint8_t shutDownStatus;      //ShutDown Status bit[0]
extern uint8_t fatalError;          //Cell Status bit[1]
extern uint8_t uartFault;           //UART RECEIVE NOTHING bit[2]
extern uint8_t tsalStatus;          //TSAL Status
extern uint8_t IMDStatus;           //IMD Status

extern uint32_t boradFaults;        //Genral DEVICE STATE: GENERAL_STATE_HV_BoardFault

int prechargeStatus;                //Precharge status


//*** BMS MASTER PRECHARGE CAN INDICATOR
uint8_t airPlus = 0;                //AirPlus status bit[0]
uint8_t airMinus = 0;               //AirMinus status bit[1]
uint8_t prchStatus = 0;             //PreCharge status bit[2]
uint8_t shdStatus = 0;              //SHD status bit[3]

uint8_t prechargeFault = 0;         //Precharge Sequence Fault

float imdPeriod = 0;                //Value of IMD Status



//--------------------- PRECHARGE UTILS --------------------------
void openAirMinus()
{

	HAL_GPIO_WritePin(CloseAirMinus_GPIO_Port, CloseAirMinus_Pin, 1);

}
void closeAirMinus()
{
	HAL_GPIO_WritePin(CloseAirMinus_GPIO_Port, CloseAirMinus_Pin, 0);
}

void openAirPlus()
{
	HAL_GPIO_WritePin(CloseAirPlus_GPIO_Port, CloseAirPlus_Pin, 1);
}
void closeAirPlus()
{
	HAL_GPIO_WritePin(CloseAirPlus_GPIO_Port, CloseAirPlus_Pin, 0);
}

void openPrChgRelay()
{
	HAL_GPIO_WritePin(Prechrage_Control_GPIO_Port, Prechrage_Control_Pin, 1);
}
void closePrChgRelay()
{
	HAL_GPIO_WritePin(Prechrage_Control_GPIO_Port, Prechrage_Control_Pin, 0);
}

void openSHD()
{
	HAL_GPIO_WritePin(BMS_SHD_Control_GPIO_Port, BMS_SHD_Control_Pin, 0);
}
void closeSHD()
{
	HAL_GPIO_WritePin(BMS_SHD_Control_GPIO_Port, BMS_SHD_Control_Pin, 1);
}




void masterInit()
{
    closeAirMinus();
    closeAirPlus();
    closePrChgRelay();
    //TO DO: openSHD, then check
    closeSHD();

    //CAN Flags Initialization
    airPlus = 0;
    airMinus = 0;
    prchStatus = 0;
    shdStatus = 0;

    //Clear successful Precharge sequence flag
    prechargeStatus = 0;
}
