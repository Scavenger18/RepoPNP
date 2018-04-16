/*
 * motor_sproc.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */

#ifndef SOURCES_HARDWARE_MOTOR_H_
#define SOURCES_HARDWARE_MOTOR_H_

#include "platform.h"

#if PL_MOTOR_EN

typedef enum{
	MOT_FWD,
	MOT_REV
}MOT_Direction;

typedef enum{
	MOT_SPROC,
	MOT_TAPE
}MOT_Device;


bool MOT_Speed(MOT_Device motor, uint8_t speedPercent, MOT_Direction dir);

//void MOT_Brake(MOT_Device motor);

//void MOT_SpeedSlow(MOT_Device motor);
//void MOT_SpeedFast(MOT_Device motor);


#endif

#endif /* SOURCES_HARDWARE_MOTOR_H_ */
