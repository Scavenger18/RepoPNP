/*
 * encoder.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */

#ifndef SOURCES_HARDWARE_ENCODER_H_
#define SOURCES_HARDWARE_ENCODER_H_

#include "platform.h"

#define ENC_INV		(0)		// set depending on encoder connection 0/1
#define ENC_TICKS	(2)		// Amount of 2 mm steps for component feed



void ENC_IntNew(void);
void ENC_SetTrigger(unsigned int tmp);
unsigned int ENC_GetTrigger(void);


unsigned int ENC_GetVal(void);
unsigned int ENC_GetHole(void);

void ENC_ResetFlag(void);
//uint8_t ENC_GetFlag(void);

void ENC_ResetCnt(void);
//uint8_t ENC_GetCnt(void);

void ENC_SetStep(unsigned int stepSize);
//uint8_t ENC_GetStep(void);

void ENC_Init(void);

#endif /* SOURCES_HARDWARE_ENCODER_H_ */
