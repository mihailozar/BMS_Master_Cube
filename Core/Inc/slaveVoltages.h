/*
 * slaveVoltages.h
 *
 *  Created on: Feb 22, 2022
 *      Author: mihailozar
 */

#ifndef INC_SLAVEVOLTAGES_H_
#define INC_SLAVEVOLTAGES_H_

/*
    Read Voltage from specific Slave Board by nDEV_ID.
    - Print Voltages for GUI App
    - Print Voltages for Human Readable Serial Monitor
    - CAN sending
*/
//Read the voltage from specific board
void readVoltageAndTemp(int nDev_ID);
void getTotalVoltage();


#endif /* INC_SLAVEVOLTAGES_H_ */
