/*
 * motor_sproc.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */

#include "motor.h"
/*	Motor Drivers
 *
 * 	Using DRV8835 H-Bridge IC for motor control.
 *	MODE = 1 (is set high through circuit design (J??)
 *	xEnable (0 = Brake, 1 = FWD/REV)
 *	xPhase 	(X = Brake, 0 = FWD, 1 = REV)
 *
 *	//Special care for when motor is connected backwards.
 */

#if PL_MOTOR_EN
#include "DIRA.h"
#include "DIRB.h"
#include "PWMA.h"
#include "PWMB.h"


uint8_t MOT_Speed(MOT_Device motor, uint8_t speedPercent, MOT_Direction dir){
	uint32_t pwmVal;

	if (speedPercent > 100){
		speedPercent = 100;
	}
	pwmVal = (((100-speedPercent)*0xFFFF)/100);

	if (dir == MOT_FWD){
		if(motor == MOT_SPROC){
			DIRA_ClrVal();				// sets H-Bridge direction forward
			PWMA_SetRatio16(pwmVal);	// sets H-Bridge speed
			return 1;

		}else if (motor == MOT_TAPE){
			DIRB_ClrVal();				// sets H-Bridge direction forward
			PWMB_SetRatio16(pwmVal);	// sets H-Bridge speed
			return 1;
		} // else error?
		else{
			return 0;
		}


	}else if (dir == MOT_REV){
		if(motor == MOT_SPROC){
			DIRA_SetVal();				// sets H-Bridge direction forward
			PWMA_SetRatio16(pwmVal);	// sets H-Bridge speed
			return 1;

		}else if (motor == MOT_TAPE){
			DIRA_SetVal();				// sets H-Bridge direction forward
			PWMA_SetRatio16(pwmVal);	// sets H-Bridge speed
			return 1;

		} // else error?
		else{
			return 0;
		}
	}
}



void MOT_Init(){
	MOT_Speed(MOT_SPROC,0,MOT_FWD);
	MOT_Speed(MOT_TAPE,0,MOT_FWD);
}


#endif
