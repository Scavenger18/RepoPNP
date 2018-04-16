/*
 * encoder.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */
#include "encoder.h"



#if PL_ENCODER_EN
#include "enc.h"


void ENC_Interrupt(void){
	static uint8_t cntr;
	cntr++;
	if((cntr%ENC_TICKS)==0){
		// stop motor
		cntr++;
		cntr = 0;


	}

}


#endif
