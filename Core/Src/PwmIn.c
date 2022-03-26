/*
 * PwmIn.c
 *
 *  Created on: Feb 24, 2022
 *      Author: mihailozar
 */
#include "PwmIn.h"
#include "tim.h"



void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
		// Read the IC value
		float ICValue = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		if(ICValue!=0){
			Duty = 100-(HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) *100)/ICValue;
			Frequency = 4000000/ICValue;
		}
	}
}

void PwmInInit(){
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
	HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);
}


float frequency(){
	return Frequency;
}
float dutycycle(){
	return Duty;
}
