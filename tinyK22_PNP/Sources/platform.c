/*
 * platform.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */
#include "platform.h"

#if PL_BUTTON_EN
	#include "buttons.h"
#endif
#if PL_ENCODER_EN
	#include "encoder.h"
#endif
#if PL_MOTOR_EN
	#include "motor.h"
#endif



