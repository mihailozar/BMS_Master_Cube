/*
 * bq76pl455qa1.c
 *
 *  Created on: Feb 23, 2022
 *      Author: mihailozar
 */

#include  <bq76pl455qa1.h>
#include <string.h>
#include "gpio.h"
#include "slaveConfig.h"
#include "uart_driver.h"

#include "FreeRTOS.h"
#include "task.h"

/*Fatal error cases:

 1. Borad doesn't response after wakeUp and AutoAddressing process
 2. Uart doesnt't receive message from Slave
 3. FaultSummary !=0 for AnyBoard
 */
uint8_t fatalError = 0;         //Fault from board Fauls
uint8_t uartFault = 0;          //Uart fault - RECEIVE NOTHING
uint32_t boradFaults = 0;       //Information on which board fault has occurred

//*** CRC calcultaion
//Only used for sending
uint16_t CRC16(BYTE *pBuf, int nLen) {
	uint16_t wCRC = 0;
	int i;

	for (i = 0; i < nLen; i++) {
		wCRC ^= (*pBuf++) & 0x00FF;
		wCRC = crc16_table[wCRC & 0x00FF] ^ (wCRC >> 8);
	}

	return wCRC;
}

void WakePL445() {

	HAL_GPIO_WritePin(GPIOC, bmsWakeUp_Pin, 1);
//    ThisThread::sleep_for(50ms);
	vTaskDelay(pdMS_TO_TICKS(50));
	HAL_GPIO_WritePin(GPIOC, bmsWakeUp_Pin, 0);
//    ThisThread::sleep_for(10ms);
	vTaskDelay(pdMS_TO_TICKS(50));
}



uint64_t setNumOfCellsAndTemps(int numCell, int numTemp){

    int posCell = 1;
    if(numCell == 0){
        posCell = 0;
    }else{
         while(numCell-1 !=0){
            posCell = posCell << 1;
            posCell |= 0x01;
            numCell--;
        }
    }

    int posTemp= 1;
    if(numTemp == 0){
         posTemp= 0;
    }else{
        while(numTemp-1 !=0){
            posTemp = posTemp << 1;
            posTemp |= 0x01;
            numTemp--;
        }
    }

    uint64_t value = (posCell & 0xFFFF) <<16;
    value |= (posTemp & 0xFF) <<8;
    return value;
}

int WriteFrame(BYTE bID, uint16_t wAddr, BYTE *pData, BYTE bLen,
		BYTE bWriteType) {

	int bPktLen = 0;
	BYTE pFrame[32];
	BYTE *pBuf = pFrame;
	uint16_t wCRC;

	if (bLen == 7 || bLen > 8)
		return 0;

	memset(pFrame, 0x7F, sizeof(pFrame));
	if (wAddr > 255) {
		*pBuf++ = 0x88 | bWriteType | bLen; // use 16-bit address
		if (bWriteType == FRMWRT_SGL_R || bWriteType == FRMWRT_SGL_NR
				|| bWriteType == FRMWRT_GRP_R || bWriteType == FRMWRT_GRP_NR) { //(bWriteType != FRMWRT_ALL_NR)// || (bWriteType != FRMWRT_ALL_R))
			*pBuf++ = (bID & 0x00FF);
		}
		*pBuf++ = (wAddr & 0xFF00) >> 8;
		*pBuf++ = wAddr & 0x00FF;
	} else {

		*pBuf++ = 0x80 | bWriteType | bLen; // use 8-bit address
		if (bWriteType == FRMWRT_SGL_R || bWriteType == FRMWRT_SGL_NR
				|| bWriteType == FRMWRT_GRP_R || bWriteType == FRMWRT_GRP_NR) {
			*pBuf++ = (bID & 0x00FF);
		}
		*pBuf++ = wAddr & 0x00FF;
	}

	while (bLen--)
		*pBuf++ = *pData++;

	bPktLen = pBuf - pFrame;

	wCRC = CRC16(pFrame, bPktLen);
	*pBuf++ = wCRC & 0x00FF;
	*pBuf++ = (wCRC & 0xFF00) >> 8;
	bPktLen += 2;

	UART_AsyncTransmitString(1,pFrame);
	vTaskDelay(pdMS_TO_TICKS(1));

	return bPktLen;
}



//*** Read Frame function, helper function
//This function is copied from recommended code design
//Main role: pack Register and data for reading data from slaves
//Communication Direction: From Slaves to BMS Master
int  ReadFrameReq(BYTE bID, uint16_t wAddr, BYTE bByteToReturn)
{
    BYTE bReturn = bByteToReturn - 1;

    if (bReturn > 127)
        return 0;

    return WriteFrame(bID, wAddr, &bReturn, 1, FRMWRT_SGL_R);
}


//*** Read from Register function, helper function
// Help to pack data for communication to specific register, READING process
int  ReadReg(BYTE bID, uint16_t wAddr, void * pData, BYTE bLen, uint32_t dwTimeOut)
{
    int   bRes = 0;
    BYTE  bRX[8];

    memset(bRX, 0, sizeof(bRX));
    switch(bLen) {
        case 1:
            bRes = ReadFrameReq(bID, wAddr, 1);
            break;
        case 2:
            bRes = ReadFrameReq(bID, wAddr, 2);
            break;
        case 3:
            bRes = ReadFrameReq(bID, wAddr, 3);
            break;
        case 4:
            bRes = ReadFrameReq(bID, wAddr, 4);
            break;
        default:
            break;
    }
    return bRes;
}

//*** Write Frame function, helper function
//This function is copied from recommended code design
//Main role: pack Register and data for sending to slaves
//Communication Direction: From BMS Master to Slaves

//*** Write to Register function, helper function
// Help to pack data for communication to specific register, WRITING process
int WriteReg(BYTE bID, uint16_t wAddr, uint64_t dwData, BYTE bLen,
		BYTE bWriteType) {
	int bRes = 0;
	BYTE bBuf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	switch (bLen) {
	case 1:
		bBuf[0] = dwData & 0x00000000000000FF;

		bRes = WriteFrame(bID, wAddr, bBuf, 1, bWriteType);
		break;
	case 2:
		bBuf[0] = (dwData & 0x000000000000FF00) >> 8;
		bBuf[1] = dwData & 0x00000000000000FF;

		bRes = WriteFrame(bID, wAddr, bBuf, 2, bWriteType);
		break;
	case 3:
		bBuf[0] = (dwData & 0x0000000000FF0000) >> 16;
		bBuf[1] = (dwData & 0x000000000000FF00) >> 8;
		bBuf[2] = dwData & 0x00000000000000FF;

		bRes = WriteFrame(bID, wAddr, bBuf, 3, bWriteType);
		break;
	case 4:
		bBuf[0] = (dwData & 0x00000000FF000000) >> 24;
		bBuf[1] = (dwData & 0x0000000000FF0000) >> 16;
		bBuf[2] = (dwData & 0x000000000000FF00) >> 8;
		bBuf[3] = dwData & 0x00000000000000FF;

		bRes = WriteFrame(bID, wAddr, bBuf, 4, bWriteType);
		break;
	case 5:
		bBuf[0] = (dwData & 0x000000FF00000000) >> 32;
		bBuf[1] = (dwData & 0x00000000FF000000) >> 24;
		bBuf[2] = (dwData & 0x0000000000FF0000) >> 16;
		bBuf[3] = (dwData & 0x000000000000FF00) >> 8;
		bBuf[4] = dwData & 0x00000000000000FF;

		bRes = WriteFrame(bID, wAddr, bBuf, 5, bWriteType);
		break;
	case 6:
		bBuf[0] = (dwData & 0x0000FF0000000000) >> 40;
		bBuf[1] = (dwData & 0x000000FF00000000) >> 32;
		bBuf[2] = (dwData & 0x00000000FF000000) >> 24;
		bBuf[3] = (dwData & 0x0000000000FF0000) >> 16;
		bBuf[4] = (dwData & 0x000000000000FF00) >> 8;
		bBuf[5] = dwData & 0x00000000000000FF;

		bRes = WriteFrame(bID, wAddr, bBuf, 6, bWriteType);
		break;
	case 7:
		bBuf[0] = (dwData & 0x00FF000000000000) >> 48;
		bBuf[1] = (dwData & 0x0000FF0000000000) >> 40;
		bBuf[2] = (dwData & 0x000000FF00000000) >> 32;
		bBuf[3] = (dwData & 0x00000000FF000000) >> 24;
		bBuf[4] = (dwData & 0x0000000000FF0000) >> 16;
		bBuf[5] = (dwData & 0x000000000000FF00) >> 8;
		bBuf[6] = dwData & 0x00000000000000FF;
		;

		bRes = WriteFrame(bID, wAddr, bBuf, 7, bWriteType);
		break;
	case 8:
		bBuf[0] = (dwData & 0xFF00000000000000) >> 56;
		bBuf[1] = (dwData & 0x00FF000000000000) >> 48;
		bBuf[2] = (dwData & 0x0000FF0000000000) >> 40;
		bBuf[3] = (dwData & 0x000000FF00000000) >> 32;
		bBuf[4] = (dwData & 0x00000000FF000000) >> 24;
		bBuf[5] = (dwData & 0x0000000000FF0000) >> 16;
		bBuf[6] = (dwData & 0x000000000000FF00) >> 8;
		bBuf[7] = dwData & 0x00000000000000FF;

		bRes = WriteFrame(bID, wAddr, bBuf, 8, bWriteType);
		break;
	default:
		break;
	}
	return bRes;
}

void InitPL455() {

	//Flag Reset
	fatalError = 0;                         //fatal error flag
	uartFault = 0;                          //uart error flag
	boradFaults = 0;                        //Detection board error flag

	//Inital Wake
	WakePL445();

	// Wake all devices
	// The wake tone will awaken any device that is already in shutdown and the pwrdown will shutdown any device
	// that is already awake. The least number of times to sequence wake and pwrdown will be half the number of
	// boards to cover the worst case combination of boards already awake or shutdown.
	for (int nDev_ID = 0; nDev_ID < TOTALBOARDS >> 1; nDev_ID++) {
		WriteReg(nDev_ID, DEVICE_CONTROL, 0x40, 1, FRMWRT_ALL_NR); // send out broadcast pwrdown command
//		ThisThread::sleep_for(20ms);
		vTaskDelay(pdMS_TO_TICKS(20));
		WakePL445();
//		ThisThread::sleep_for(20ms);
		vTaskDelay(pdMS_TO_TICKS(20));
	}

	// Mask Customer Checksum Fault bit
	WriteReg(0, FAULT_MASK_DEV, 0x8000, 2, FRMWRT_ALL_NR); // clear all fault summary flags

	// Clear all faults
	WriteReg(0, FAULT_SUM, 0xFFC0, 2, FRMWRT_ALL_NR); // clear all fault summary flags
	WriteReg(0, DEVICE_STATUS, 0x38, 1, FRMWRT_ALL_NR); // clear fault flags in the system status register

	// Auto-address all boards (section 1.2.2)
	WriteReg(0, DEVICE_CONFIG, 0x19, 1, FRMWRT_ALL_NR); // set auto-address mode on all boards
	WriteReg(0, DEVICE_CONTROL, 0x08, 1, FRMWRT_ALL_NR); // enter auto address mode on all boards, the next write to this ID will be its address

	// Set addresses for all boards in daisy-chain (section 1.2.3)
	for (int nDev_ID = 0; nDev_ID < TOTALBOARDS; nDev_ID++) {
		WriteReg(nDev_ID, DEVICE_ADDR, nDev_ID, 1, FRMWRT_ALL_NR); // send address to each board
	}

	// Enable all communication interfaces on all boards in the stack (section 1.2.1)
	//default 250k bound, Communication Diresction: BMS Master to Slaves
	WriteReg(0, DEVICE_COMCONFIG, 0x10F8, 2, FRMWRT_ALL_NR); // set communications baud rate and enable all interfaces on all boards in stack

	uint32_t wTemp = 0;


	for (int nDev_ID = TOTALBOARDS - 1; nDev_ID >= 0; --nDev_ID) {
		// read device ID to see if there is a response
		ReadReg(nDev_ID, DEVICE_ADDR, &wTemp, 1, 0); // 0ms timeout
//		uartReceive();

		//ovde zajebava treba proveriti
		dealokacijaMemorije(UART_BlockReceiveString(5));



//		ThisThread::sleep_for(50ms);
	}

	vTaskDelay(pdMS_TO_TICKS(1));

	// Clear all faults
	WriteReg(0, FAULT_SUM, 0xFFC0, 2, FRMWRT_ALL_NR); // clear all fault summary flags
	WriteReg(0, DEVICE_STATUS, 0x38, 1, FRMWRT_ALL_NR); // clear fault flags in the system status register

	vTaskDelay(pdMS_TO_TICKS(10));

// Configure AFE (section 2.2.1)

	int nDev_ID = 0;
	WriteReg(nDev_ID, DEVICE_RESERVED, 0x00, 1, FRMWRT_SGL_NR); // set 0 mux delay
	WriteReg(nDev_ID, DEVICE_SMPLDELAY, 0x00, 1, FRMWRT_SGL_NR); // set 0 initial delay

// Configure voltage and internal sample period (section 2.2.2)
	WriteReg(nDev_ID, DEVICE_CELL_SPER, 0xCC, 1, FRMWRT_SGL_NR); // set 99.92us ADC sampling period

// Configure the oversampling rate (section 2.2.3)
	WriteReg(nDev_ID, DEVICE_OVERSMPL, 0x00, 1, FRMWRT_SGL_NR); // set no oversampling period

// select all cell channels 1-8, AUX channels 0 and 1, and internal digital die and internal analog die temperatures
	WriteReg(0, DEVICE_CHANNELS, setNumOfCellsAndTemps(TOTALCELLS, TOTALTEMP),4, FRMWRT_ALL_NR);
//0x007F0100

// * * * * Thresholds Note: if you don't set the Threshold limis for specific fault,
//the bq chip will not report specific fault

// Set cell over-voltage and cell under-voltage thresholds on a single board (section 2.2.6.1)
	nDev_ID = 0;
// Set cell over-voltage and cell under-voltage thresholds on all boards simultaneously (section 2.2.6.2)

	WriteReg(0, DEVICE_CELL_UV, 0xD1EC, 2, FRMWRT_ALL_NR); // set OV threshold = 4.1000V
	WriteReg(0, DEVICE_CELL_OV, 0x6148, 2, FRMWRT_ALL_NR); // set UV threshold = 1.9000V

}


void shutDownSlavesCommand(){
     for(int nDev_ID = 0; nDev_ID < TOTALBOARDS>>1; nDev_ID++) {
        WriteReg(nDev_ID, DEVICE_CONTROL, 0x40, 1, FRMWRT_ALL_NR);  // send out broadcast pwrdown command
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
