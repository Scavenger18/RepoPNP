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
#include "HMODE.h"


static void MOT_Dir_Sproc(MOT_Direction dir){
bool tmp = TRUE;
	if (dir == MOT_FWD){
		tmp = TRUE;
	}else if (dir == MOT_REV){
		tmp = FALSE;
	}

	if(MOT_SPROC_INV){
		tmp = !tmp;
	}

	DIRA_PutVal(tmp);
}

static void MOT_Dir_Tape(MOT_Direction dir){
bool tmp = TRUE;
	if (dir == MOT_FWD){
		tmp = TRUE;
	}else if (dir == MOT_REV){
		tmp = FALSE;
	}

	if(MOT_TAPE_INV){
		tmp = !tmp;
	}
	DIRB_PutVal(tmp);

}

uint8_t MOT_Speed(MOT_Device motor, uint8_t speedPercent, MOT_Direction dir){
	uint32_t pwmVal;

	if (speedPercent > 100){
		speedPercent = 100;
	}
	pwmVal = (((100-speedPercent)*0xFFFF)/100);	// 0 is 100%

	if(motor == MOT_SPROC){
		MOT_Dir_Sproc(dir);
		PWMA_SetRatio16(pwmVal);	// sets H-Bridge speed
		return 1;

	}else if (motor == MOT_TAPE){
		MOT_Dir_Tape(dir);
		PWMB_SetRatio16(pwmVal);	// sets H-Bridge speed
		return 1;
	} // else error?
	else{
		return 0;
	}

}



void MOT_Init(){

	MOT_Speed(MOT_SPROC,0,MOT_FWD);
	MOT_Speed(MOT_TAPE,0,MOT_FWD);

	HMODE_SetVal();
}


#endif
