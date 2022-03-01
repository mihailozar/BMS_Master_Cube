/*
 * PwmIn.h
 *
 *  Created on: Feb 23, 2022
 *      Author: mihailozar
 */

#ifndef INC_PWMIN_H_
#define INC_PWMIN_H_


/** PwmIn class to read PWM inputs
 *
 * Uses InterruptIn to measure the changes on the input
 * and record the time they occur
 *
 * @note uses InterruptIn, so not available on p19/p20
 */

 	void PwmIn_Init();


    float frequency();

    /** Read the current dutycycle
     *
     * @returns the dutycycle as a percentage, represented between 0.0-1.0
     */
    float dutycycle();

    static float Duty;
    static float Frequency;


#endif /* INC_PWMIN_H_ */
