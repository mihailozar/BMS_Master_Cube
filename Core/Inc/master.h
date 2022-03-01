/*
 * master.h
 *
 *  Created on: Feb 22, 2022
 *      Author: mihailozar
 */

#ifndef INC_MASTER_H_
#define INC_MASTER_H_

/* BMS Master Initialization */
void masterInit();

/* IMD Value read  */
void getIMD_Status();

/* General state calculation Function */
void generalDeviceStatus();

/* Precharge sequence Function */
void getPrecharged();

/* Precharge RELAY FAULTS and Sequence Debug */
void getPrechargeState();

/* Open Shut Down Circuit and Close all airs and relays*/
void openSHDonFAULT();

//*** SHD Utils
int isSHDClosed();
int isSHDOpened();

//*** Open SHD on ECU request
void getECU_SHD_Request();

//*** Open SHD on BMS Master request
void checkSHD_BMSMasterFault();

#endif /* INC_MASTER_H_ */
