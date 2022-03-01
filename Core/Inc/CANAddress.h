/*
 * CANAddress.h
 *
 *  Created on: Feb 22, 2022
 *      Author: mihailozar
 */

#ifndef INC_CANADDRESS_H_
#define INC_CANADDRESS_H_


/*
 *
 * VEHICLE_START -> Start button pushed.   Going into state: PRECHARGE
 * 					Start button + Brakes. Going into state: INVERTER_ACTIVATION
 *
 */

#define		VEHICLE_START_ID	0x0064
#define		VEHICLE_START		0x01

/*
 *
 * GENERAL_STATE_<DEVICE> -> Total voltage, current and SOC info. HV Peripherals info. (HV, LV)
 *
 */

#define		GENERAL_STATE_HV_ID	0x0065
#define		GENERAL_STATE_LV_ID	0x0066

/*
 *  BMS Master Board Functionality Debug
*/

#define     BMS_MASTER_FUNC_DBG 0x0098

/*
 *
 * 	BMS*L_FAULT_ID -> ID for UV, OV, AUX_UV_OV, SYS faults 		  (from slave no.*)
 * 	BMS*H_FAULT_ID -> ID for COMP_UV, COMP_OV, COMMN, CHIP FAULTS (from slave no.*)
 *
 */


#define 	BMS0L_FAULT_ID 		0x012C
#define 	BMS0H_FAULT_ID 		0x012D
#define 	BMS1L_FAULT_ID		0x012E
#define 	BMS1H_FAULT_ID		0x012F
#define 	BMS2L_FAULT_ID		0x0130
#define 	BMS2H_FAULT_ID 		0x0131
#define		BMS3L_FAULT_ID 		0x0132
#define 	BMS3H_FAULT_ID 		0x0133
#define 	BMS4L_FAULT_ID		0x0134
#define 	BMS4H_FAULT_ID		0x0135
#define 	BMS5L_FAULT_ID		0x0136
#define 	BMS5H_FAULT_ID		0x0137
#define 	BMS6L_FAULT_ID		0x0138
#define 	BMS6H_FAULT_ID		0x0139
#define 	BMS7L_FAULT_ID		0x013A
#define 	BMS7H_FAULT_ID		0x013B
#define 	BMS8L_FAULT_ID		0x013C
#define 	BMS8H_FAULT_ID		0x013D
#define 	BMS9L_FAULT_ID		0x013E
#define 	BMS9H_FAULT_ID  	0x013F

/*
 *
 * 	BMS*L_HV_V_ID -> ID for voltages from CELL0 to CELL7  [0-5V] (from slave no.*)
 * 	BMS*H_HV_V_ID -> ID for voltages from CELL8 to CELL14 [0-5V] (from slave no.*)
 *
 */

#define 	BMS0L_HV_V_ID 		0x015E
#define 	BMS0H_HV_V_ID 		0x015F
#define 	BMS1L_HV_V_ID 		0x0160
#define 	BMS1H_HV_V_ID 		0x0161
#define 	BMS2L_HV_V_ID 		0x0162
#define 	BMS2H_HV_V_ID 		0x0163
#define		BMS3L_HV_V_ID 		0x0164
#define 	BMS3H_HV_V_ID 		0x0165
#define 	BMS4L_HV_V_ID 		0x0166
#define 	BMS4H_HV_V_ID 		0x0167
#define 	BMS5L_HV_V_ID 		0x0168
#define 	BMS5H_HV_V_ID 		0x0169
#define 	BMS6L_HV_V_ID		0x016A
#define 	BMS6H_HV_V_ID		0x016B
#define 	BMS7L_HV_V_ID		0x016C
#define 	BMS7H_HV_V_ID		0x016D
#define 	BMS8L_HV_V_ID 		0x016E
#define 	BMS8H_HV_V_ID 		0x016F
#define 	BMS9L_HV_V_ID 		0x0170
#define 	BMS9H_HV_V_ID		0x0171

/*
 *
 * BMS*_HV_TEMP_ID -> ID for 7 temperatures (per one slave) [0-80C] (from slave no.*)
 *
 */
#define 	BMS0_HV_TEMP_ID 	0x017C
#define 	BMS1_HV_TEMP_ID 	0x017D
#define 	BMS2_HV_TEMP_ID 	0x017E
#define 	BMS3_HV_TEMP_ID 	0x017F
#define 	BMS4_HV_TEMP_ID 	0x0180
#define 	BMS5_HV_TEMP_ID 	0x0181
#define 	BMS6_HV_TEMP_ID 	0x0182
#define 	BMS7_HV_TEMP_ID 	0x0183
#define 	BMS8_HV_TEMP_ID 	0x0184
#define 	BMS9_HV_TEMP_ID 	0x0185

/*
 * BMS_HV_VCELL* -> Position of CELL* voltage value in CAN message (1 slave -> 15 CELL voltages)
 *
 */
#define 	BMS_HV_VCELL0		0x00
#define 	BMS_HV_VCELL1		0x01
#define 	BMS_HV_VCELL2		0x02
#define 	BMS_HV_VCELL3		0x03
#define 	BMS_HV_VCELL4		0x04
#define 	BMS_HV_VCELL5		0x05
#define 	BMS_HV_VCELL6		0x06
#define 	BMS_HV_VCELL7		0x07
#define 	BMS_HV_VCELL8		0x00
#define 	BMS_HV_VCELL9		0x01
#define 	BMS_HV_VCELL10		0x02
#define 	BMS_HV_VCELL11		0x03
#define 	BMS_HV_VCELL12		0x04
#define 	BMS_HV_VCELL13		0x05
#define 	BMS_HV_VCELL14		0x06
/*
 *
 * BMS_HV_TEMP* -> Position of temperature value in CAN message (7 temperatures in one CAN message,
 * 																 1 slave measures 7 temperatures)
 */
#define 	BMS_HV_TEMP0		0x00
#define 	BMS_HV_TEMP1		0x01
#define 	BMS_HV_TEMP2		0x02
#define 	BMS_HV_TEMP3		0x03
#define 	BMS_HV_TEMP4		0x04
#define 	BMS_HV_TEMP5		0x05
#define 	BMS_HV_TEMP6		0x06

/*
*
 * BMS_LV_V_TEMP_ID -> ID for voltages and temperatures from CELL0 to CELL3
 *
 */
#define 	BMS_LV_V_TEMP_ID  	0x0190
/*
*
 * BMS_LV_CELL* -> Position of voltage value in CAN message
 * BMS_LV_TEMP* -> Position of temperature value in CAN message
 *
 */
#define 	BMS_LV_VCELL0 		0x00
#define 	BMS_LV_VCELL1 		0x01
#define 	BMS_LV_VCELL2 		0x02
#define 	BMS_LV_VCELL3 		0x03
#define 	BMS_LV_TEMP0 		0x04
#define 	BMS_LV_TEMP1 		0x05
#define 	BMS_LV_TEMP2 		0x06
#define 	BMS_LV_TEMP3 		0x07




#endif /* INC_CANADDRESS_H_ */
