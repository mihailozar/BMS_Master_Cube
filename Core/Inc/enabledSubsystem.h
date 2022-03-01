/*
 * enabledSubsystem.h
 *
 *  Created on: Feb 22, 2022
 *      Author: mihailozar
 */

#ifndef INC_ENABLEDSUBSYSTEM_H_
#define INC_ENABLEDSUBSYSTEM_H_

//==========================================================
//                      WORK UTILS
//==========================================================


//--- GUI printing data for BMS Diagnostic:
#define GUI_COMMUNICATION               1

//--- Serial Printing Relay state:
//#define PRINT_RELAY_STATE               1

//---- See full HV data in Human format via Serial Port:
//#define SERIAL_HUMAN_READABLE_PRINT     1

//--- CAN bus Voltage and Temeparture sending:
#define CAN_SEND_VOTALGE_AND_TEMPS      1

//--- Enable reading IMD status and sending data via CAN Bus
#define IMD_READ_STATUS                 1
//--- Print IMD Value in Human readable Format on Serial Monitor
#define IMD_SERIALPRINT_STATUS          1


//==========================================================
//                      DEBUG UTILS
//==========================================================
//#define CAN_TEMEPERATURE_SERIAL_PRINT   1

#endif /* INC_ENABLEDSUBSYSTEM_H_ */
