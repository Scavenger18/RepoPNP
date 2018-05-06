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

static unsigned int peelState;

static void Peeler(void){
	if (peelState == 1){
		if(BUT_GetState(BUT_MSW)==0){
			MOT_Speed(MOT_TAPE,50,MOT_FWD);
			peelState = 0;
		}
	}

	if((BUT_GetState(BUT_MSW)==1)&&peelState ==0){
		MOT_Speed(MOT_TAPE,0,MOT_FWD);
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
			peelState = 0;
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
			//ENC_ResetFlag();
			ENC_ResetCnt();

			//MOT_Speed(MOT_SPROC,0,MOT_FWD);
			//MOT_Speed(MOT_TAPE,0,MOT_FWD);

			if(APP_Counter > 0){
				taskState = FSM_FWD;
				APP_Counter--;
				break;
			}else if(APP_Counter < 0){
				taskState = FSM_REV;
				APP_Counter++;
				break;
			} //else 0 -> Stay in IDLE
			if(BUT_GetState(BUT_REV)==BUT_PRESS){
				taskState = FSM_STOP;
				break;
			}


//			if(BUT_GetState(BUT_FWD)==1){
//				taskState = FSM_FWD;
//				break;
//			}

			/* commented so it can be used as stop */
//			if(BUT_GetState(BUT_REV)==1){
//				taskState = FSM_REV;
//				break;
//			}
//			if (COMM_GetFWD()==1){
//				taskState = FSM_FWD;
//				break;
//			}
//			if (COMM_GetREV()==1){
//				taskState = FSM_REV;
//				break;
//			}

			// if (UART_GetState == FWD); etc...
		}
		break;
		case FSM_FWD:
		{
			// turn sprocket motor forward and count steps
			MOT_Speed(MOT_SPROC,80,MOT_FWD);
			taskState = FSM_RUN;
		}
		break;
		case FSM_REV:
		{
			MOT_Speed(MOT_SPROC,80,MOT_REV);
			taskState = FSM_RUN;
		}
		break;
		case FSM_RUN:
		{
//	    	if(ENC_GetVal()==1){
//				taskState = FSM_IDLE;
//			}
			if(ENC_GetTrigger() == 1){
				ENC_SetTrigger(0);
				taskState  = FSM_IDLE;
			}

			// use button REV to stop
			if(BUT_GetState(BUT_REV)==BUT_PRESS){

				taskState = FSM_STOP;
				break;
			}
			peelState = 1;	// möglicherweise verzögert absetzen

		}
		break;
		case FSM_STOP:
//			if (ENC_GetHole() != 0){
//				MOT_Speed(MOT_SPROC, 10, MOT_REV);
//			} else{
				MOT_Speed(MOT_TAPE,0,MOT_FWD);
				MOT_Speed(MOT_SPROC, 0, MOT_FWD);
				taskState = FSM_IDLE;
//			}
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
		Peeler();
		BUT_Process();
	    //(void)CLS1_ReadAndParseWithCommandTable(buffer, sizeof(buffer), CLS1_GetStdio(), cmdTable);
//	    error_res = COMM_ReadAndParse(buffer, sizeof(buffer), CLS1_GetStdio());
//	    APP_ErrorHandler();

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
