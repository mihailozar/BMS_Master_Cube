/*
 * slaveConfig.h
 *
 *  Created on: Feb 22, 2022
 *      Author: mihailozar
 */

#ifndef INC_SLAVECONFIG_H_
#define INC_SLAVECONFIG_H_

#define TOTALBOARDS             10      //Total Number of Slave Boards
#define TOTALCELLS              14      //Total Cell Measurement
#define TOTALTEMP               7       //Total Temperature Measurement

#define HARDWARE_WAIT           1s      //seconds

#define TOTAL_MIN_VOLTAGE       448     //V, 3.4 * 140
#define TOTAL_MAX_VOLTAGE       588     //V, 4.2 * 140

#define UNDER_CELL_VOLTAGE      4.2     //V
#define OVER_CELL_VOTAGE        3.4     //V

#define OVER_CURRENT            150.0   //A

#define TEMP_UP_LIMIT           60      //Celzius
#define TEMP_HIGH_LIMIT         120     //Celzius

#define timeCheckerUart 100


#endif /* INC_SLAVECONFIG_H_ */
