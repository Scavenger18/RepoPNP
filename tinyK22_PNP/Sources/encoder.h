/*
 * encoder.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */

#ifndef SOURCES_HARDWARE_ENCODER_H_
#define SOURCES_HARDWARE_ENCODER_H_

#include "platform.h"

#define ENC_TICKS	10		// Amount of 2 mm steps for component feed


void ENC_Interrupt(void);

#endif /* SOURCES_HARDWARE_ENCODER_H_ */
