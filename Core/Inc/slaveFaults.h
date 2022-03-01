/*
 * slaveFaults.h
 *
 *  Created on: Feb 22, 2022
 *      Author: mihailozar
 */

#ifndef INC_SLAVEFAULTS_H_
#define INC_SLAVEFAULTS_H_

/*
    Read Fault from specific Slave Board by nDEV_ID.
    - Print Faults for GUI App
    - Print Faults for Human Readable Serial Monitor
    - Print More Info about specific fault witch occurred
    - CAN sending
*/

void readFault(int nDev_ID);

#endif /* INC_SLAVEFAULTS_H_ */
