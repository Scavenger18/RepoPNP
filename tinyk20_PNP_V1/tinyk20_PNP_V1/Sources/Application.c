/*
 * Application.c
 *
 *  Created on: 17.02.2018
 *      Author: Erich Styger
 */

#include "Application.h"
#include "FRTOS1.h"
#include "LED1.h"
#include "LED2.h"
#include "WAIT1.h"
#include "ENC1.h"
#include "SW_FWD.h"
#include "SW_REV.h"
#include "DIRA.h"
#include "DIRB.h"
#include "PWMA.h"
#include "PWMB.h"
#include "CLS1.h"

#include "platform.h"

#include "buttons.h"
#include "encoder.h"
#include "motor.h"
#include "comm.h"

uint8_t	error_res;


static void RunPeeler(void){
	switch (peelState){
		case PEEL_OFF:
		{
			MOT_Speed(MOT_TAPE,0,MOT_FWD);
			break;
		}
		case PEEL_FWD:
		{
			if(BUT_GetState(BUT_MSW)==0){
				MOT_Speed(MOT_TAPE,100,MOT_FWD);
			}

			break;
		}
		case PEEL_REV:
		{
			MOT_Speed(MOT_TAPE,100,MOT_REV);
			break;
		}
		case PEEL_FWD_DONE:
		{
			if(BUT_GetState(BUT_MSW)==1){	// should change for rev
				peelState = PEEL_OFF;
			}
			break;
		}
		case PEEL_REV_DONE:
		{
			if(BUT_GetState(BUT_MSW)==0){	// should change for rev
				peelState = PEEL_OFF;
			}
			break;
		}

		default:
		{
			peelState = PEEL_OFF;
		}
	}
}

static void APP_ErrorHandler(){
	switch(error_res){
	case ERR_FAILED:
	{
		//--> Fall through to last Error
		//LED2_Neg();
		//error_res = COMM_SendError("ERR_FAILED");
		//break;
	}
	case ERR_VALUE:
	{
		//--> Fall through to last Error
		//LED2_Neg();
		//error_res = COMM_SendError("ERR_FAILED");
		//break;
	}
	case ERR_OVERFLOW:
	{
		//--> Fall through to last Error
		//LED2_Neg();
		//error_res = COMM_SendError("ERR_FAILED");
		taskState = FSM_ERROR;
		break;
	}
	case ERR_OK:
	{
		LED2_On();
		//error_res = COMM_SendError("ERR_FAILED");
		break;
	}
	}
}

static void RunFeeder(void){
	switch (taskState){
		case FSM_INIT:
		{
			ENC_ResetCnt();
			ENC_SetStep(4);
			peelState = PEEL_OFF;;
			MOT_Speed(MOT_SPROC,0,MOT_FWD);
			MOT_Speed(MOT_TAPE,0,MOT_FWD);
			taskState = FSM_IDLE;
			for(int i = 0; i < 4; i++){
				LED1_Neg();
				WAIT1_Waitms(100);
			}
		}
		break;
		case FSM_IDLE:
		{
			ENC_ResetCnt();

			if(APP_Counter > 0){
				taskState = FSM_FWD;
				APP_Counter--;
				break;
			}else if(APP_Counter < 0){
				taskState = FSM_REV_PEEL;
				APP_Counter++;
				break;
			} //else 0 -> Stay in IDLE
			if(BUT_GetState(BUT_REV)==BUT_PRESS){
				taskState = FSM_STOP;
				peelState = PEEL_OFF;
				break;
			}
		}
		break;
		case FSM_FWD:
		{
			// turn sprocket motor forward and count steps
			MOT_Speed(MOT_SPROC,100,MOT_FWD);
			taskState = FSM_RUN;
			peelState = PEEL_FWD;
		}
		break;
		case FSM_REV_PEEL:
		{
			// First turn peeler back
			peelState = PEEL_REV;
			taskState = FSM_REV_SPROC;
		}
		break;
		case FSM_REV_SPROC:
		{
			// wait for a certain time (time calculated)
			WAIT1_WaitOSms(1500);
			// then change case to reverse sprocket
			MOT_Speed(MOT_SPROC,100,MOT_REV);
			taskState = FSM_RUN;
			peelState = PEEL_REV_DONE;

		}
		break;
		case FSM_RUN:
		{

			if(ENC_GetTrigger() == 1){
				ENC_SetTrigger(0);
				taskState  = FSM_IDLE;
				peelState = PEEL_FWD_DONE;
			}

			// use button REV to stop
			if(BUT_GetState(BUT_REV)==BUT_PRESS){
				taskState = FSM_STOP;
				peelState = PEEL_OFF;
				break;
			}

		}
		break;
		case FSM_STOP:
			MOT_Speed(MOT_SPROC, 0, MOT_FWD);
			taskState = FSM_IDLE;
			break;
		case FSM_ERROR:
		{
			//Blink Status LED
			LED2_Neg();
		}
		default:
		taskState = FSM_ERROR;
	}
}

 static void FSM_task(void *param) {

	(void)param;
	for(;;) {
		RunFeeder();
		RunPeeler();
		BUT_Process();

	    vTaskDelay(pdMS_TO_TICKS(50));

	} /* for */
}

void APP_Init(void){
	error_res = ERR_OK;		// set ErrorFlag to OK
	APP_Counter = 0;		// set start to 0 (no inc/dec of tape needed)
	taskState = FSM_INIT;
	MOT_Init();
	BUT_Init();
}

void APP_Run(void) {

	// start communication (UART)
	COMM_Init();
	// start encoder polling
	ENC_Init();

	if (xTaskCreate(FSM_task, "FSM", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
		for(;;){} /* error! probably out of memory */
	}

	vTaskStartScheduler();
	for(;;) {
	}
}
