/*
 * slaveBalance.h
 *
 *  Created on: Feb 22, 2022
 *      Author: mihailozar
 */

#ifndef INC_SLAVEBALANCE_H_
#define INC_SLAVEBALANCE_H_

void enableBalancingOnFaultAllBoard();
void disableBalancingOnFaultAllBoard();
void enableBalancingOnFaultSpecBoard(int nDev_Id);

void balanceCellOn(int nDev_Id, unsigned char cellNum);
void balanceCellOff(int nDev_Id, unsigned char cellNum);

void balanceOffSglBoard(int nDev_Id);
void balanceOffAllBoard();


void simpleAlgBalancing();

#endif /* INC_SLAVEBALANCE_H_ */
