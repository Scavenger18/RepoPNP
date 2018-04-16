/*
 * platform.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */
#include "platform.h"
#include "stdint.h"

#if PL_BUTTON_EN
	#include "buttons.h"
#endif
#if PL_ENCODER_EN
	#include "encoder.h"
#endif
#if PL_MOTOR_EN
	#include "motor.h"
#endif
#if PL_COMM_EN
	#include "comm.h"
#endif

void PL_Init(){
#if PL_BUTTON_EN
	//BUT_Init();
#endif
#if PL_ENCODER_EN
	// init
#endif
#if PL_MOTOR_EN
	// init
	MOT_Init();
#endif
}

