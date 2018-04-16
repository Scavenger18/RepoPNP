/*
 * buttons.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */
#include "buttons.h"
#include "Application.h"
#include "CS1.h"

#include "SW_FWD.h"
#include "SW_REV.h"
#if PL_TAPE_EN
#include "SW_MSW.h"
#endif

uint32_t REV_Counter;
uint8_t  REV_Flag;


void BUT_SetFWD(){
	CS1_CriticalVariable();

	CS1_EnterCritical();
	APP_Counter++;
	CS1_ExitCritical();
}

void BUT_SetREV(){
	CS1_CriticalVariable();

	CS1_EnterCritical();
	APP_Counter--;
	CS1_ExitCritical();
}


BUT_State BUT_GetState(BUT_Device button){

	BUT_State but_state;// = BUT_IDLE;

	uint8_t state = 0;

	switch (button){
		case BUT_FWD:
		{
			state = SW_FWD_GetVal();
			if (FWD_INVERT) {
				if (state == 1){
					but_state = BUT_IDLE;
				}else{
					but_state = BUT_PRESS;
				}
			}else{
				if (state == 1){
					but_state = BUT_PRESS;
				}else{
					but_state = BUT_IDLE;
				}
			}

			if (but_state == BUT_PRESS){
				BUT_SetFWD();
			}
			break;
		}
		case BUT_REV:
		{
			state = SW_REV_GetVal();
			if (REV_INVERT) {
				if (state == 1){
					but_state = BUT_IDLE;
				}else{
					but_state = BUT_PRESS;
				}
			}else{
				if (state == 1){
					but_state = BUT_PRESS;
				}else{
					but_state = BUT_IDLE;
				}
			}

			if (but_state == BUT_IDLE){
				REV_Flag = 0;
			}else if ((but_state == BUT_PRESS)&&(REV_Flag == 0)){
				REV_Flag = 1;
				REV_Counter = 0;
			} else if ((REV_Counter == LPRESS_CNT)&&(but_state == BUT_PRESS)){
				REV_Counter = 0;
				but_state = BUT_LPRESS;
				BUT_SetREV();
			}

			break;
		}
#if PL_TAPE_EN
		case BUT_MSW:
		{
			state = SW_MSW_GetVal();
			if (MSW_INVERT) {
				if (state == 1){
					but_state = BUT_IDLE;
				}else{
					but_state = BUT_PRESS;
				}
			}else{
				if (state == 1){
					but_state = BUT_PRESS;
				}else{
					but_state = BUT_IDLE;
				}
			}
			break;
		}
#endif


		default:
		{
			state = 0;
			but_state = BUT_PRESS;
		}
	}
	return but_state;
}

void BUT_Count(void){

	CS1_CriticalVariable();
	CS1_EnterCritical();
	REV_Counter++;
	CS1_ExitCritical();
}

void BUT_Process(void){
	//(void) BUT_GetState(BUT_MSW);
	//(void) BUT_GetState(BUT_REV);
	//(void) BUT_GetState(BUT_FWD);
}

void BUT_Init(void){
//	PORT_PDD_SetPinPullSelect(PORTC_BASE_PTR, 10, PORT_PDD_PULL_UP);
//	PORT_PDD_SetPinPullEnable(PORTC_BASE_PTR, 10, PORT_PDD_PULL_ENABLE);
	return;
}
