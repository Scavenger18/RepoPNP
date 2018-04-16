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
#include "DIRS.h"
#include "DIRT.h"
#include "PWMS.h"
#include "PWMT.h"


bool MOT_Speed(MOT_Device motor, uint8_t speedPercent, MOT_Direction dir){
	uint32_t pwmVal;

	if (speedPercent > 100){
		speedPercent = 100;
	}
	pwmVal = ((speedPercent*0xFFFF)/100);

	if (dir == MOT_FWD){
		if(motor == MOT_SPROC){
			DIRS_ClrVal();				// sets H-Bridge direction forward
			PWMS_SetRatio16(pwmVal);	// sets H-Bridge speed
			return true;

		}else if (motor == MOT_TAPE){
			DIRT_ClrVal();				// sets H-Bridge direction forward
			PWMT_SetRatio16(pwmVal);	// sets H-Bridge speed
			return true;
		} // else error?
		else{
			return false;
		}


	}else if (dir == MOT_REV){
		if(motor == MOT_SPROC){
			DIRS_SetVal();				// sets H-Bridge direction forward
			PWMS_SetRatio16(pwmVal);	// sets H-Bridge speed
			return true;

		}else if (motor == MOT_TAPE){
			DIRS_SetVal();				// sets H-Bridge direction forward
			PWMS_SetRatio16(pwmVal);	// sets H-Bridge speed
			return true;

		} // else error?
		else{
			return false;
		}
	}
}






#endif
