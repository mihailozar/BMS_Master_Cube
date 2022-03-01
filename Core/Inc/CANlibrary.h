/*
 * CANlibrary.h
 *
 *  Created on: Feb 22, 2022
 *      Author: mihailozar
 */

#ifndef INC_CANLIBRARY_H_
#define INC_CANLIBRARY_H_


#include <CANAddress.h>
#include <stdint.h>
#include "can.h"

typedef struct CANMessage {
	CAN_RxHeaderTypeDef pHeader;
	uint8_t data[8];
} CANMsg;
//More info on Google Dirve, table: CAN projektovanje

#define LOW     0   //Low part of CAN messages
#define HIGH    1   //High part of CAN messages

const uint16_t Id_Can_VOLTAGES[10][2] =
{
    {BMS0L_HV_V_ID, BMS0H_HV_V_ID},
    {BMS1L_HV_V_ID, BMS1H_HV_V_ID},
    {BMS2L_HV_V_ID, BMS2H_HV_V_ID},
    {BMS3L_HV_V_ID, BMS3H_HV_V_ID},
    {BMS4L_HV_V_ID, BMS4H_HV_V_ID},
    {BMS5L_HV_V_ID, BMS5H_HV_V_ID},
    {BMS6L_HV_V_ID, BMS6H_HV_V_ID},
    {BMS7L_HV_V_ID, BMS7H_HV_V_ID},
    {BMS8L_HV_V_ID, BMS8H_HV_V_ID},
    {BMS9L_HV_V_ID, BMS9H_HV_V_ID}
};
const uint16_t Id_Can_Temp[10] =
{
    BMS0_HV_TEMP_ID,
    BMS1_HV_TEMP_ID,
    BMS2_HV_TEMP_ID,
    BMS3_HV_TEMP_ID,
    BMS4_HV_TEMP_ID,
    BMS5_HV_TEMP_ID,
    BMS6_HV_TEMP_ID,
    BMS7_HV_TEMP_ID,
    BMS8_HV_TEMP_ID,
    BMS9_HV_TEMP_ID
};

const uint16_t Id_Can_FAULTS[10][2] =
{
    {BMS0L_FAULT_ID, BMS0H_FAULT_ID},
    {BMS1L_FAULT_ID, BMS1H_FAULT_ID},
    {BMS2L_FAULT_ID, BMS2H_FAULT_ID},
    {BMS3L_FAULT_ID, BMS3H_FAULT_ID},
    {BMS4L_FAULT_ID, BMS4H_FAULT_ID},
    {BMS5L_FAULT_ID, BMS5H_FAULT_ID},
    {BMS6L_FAULT_ID, BMS6H_FAULT_ID},
    {BMS7L_FAULT_ID, BMS7H_FAULT_ID},
    {BMS8L_FAULT_ID, BMS8H_FAULT_ID},
    {BMS9L_FAULT_ID, BMS9H_FAULT_ID}
};

//CANMessage canSendMessage_Voltage[10][2];                  //10 messages for 10 boards

void Can_Init();
void canSend(uint16_t id,  CANMsg* canMsg);

void pack_data_2B( CANMsg* canMsg, uint16_t data, uint8_t position);
void pack_data_1B( CANMsg* canMsg, uint16_t data, uint8_t position);



#endif /* INC_CANLIBRARY_H_ */
