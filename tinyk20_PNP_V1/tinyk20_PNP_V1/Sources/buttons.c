/*
 * buttons.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */
#include "buttons.h"

#include "SW_FWD.h"
#include "SW_REV.h"

#if PL_TAPE_EN
#include "SW_MSW.h"
#endif

unsigned int BUT_GetState(BUT_Device button){

	static unsigned int state = 0;

	switch (button){
		case BUT_FWD:
		{
			if(SW_FWD_GetVal()){
				if (FWD_INVERT==0) {
					state = 1;
				}else{
					state = 0;
				}
			}else{
				if (FWD_INVERT==0) {
					state = 0;
				}else{
					state = 1;
				}
			}
			break;
		}
		case BUT_REV:
		{
			state = SW_REV_GetVal();
			if (REV_INVERT) {
				if (state == 1){
					state = 0;
				}else{
					state = 1;
				}

			}
			break;
		}
#if PL_TAPE_EN
		case BUT_MSW:
		{
			state = SW_MSW_GetVal();
			if (MSW_INVERT) {
				if (state == 1){
					state = 0;
				}else{
					state = 1;
				}

			}
			break;
		}
#endif


		default:
		{
			state = 0;
		}
	}
	return state;
}


void BUT_Init(void){
//	PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 10, PORT_PDD_PULL_UP);
//	PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 10, PORT_PDD_PULL_ENABLE);
	return;
}
