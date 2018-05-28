/*
 * encoder.c
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */
#include "encoder.h"



#if PL_ENCODER_EN
#include "enc1.h"
#include "CS1.h"
#include "LED1.h"
#include "LED2.h"
#include "FRTOS1.h"

#include "motor.h"



#define ENC_TICK_CNT (3)

static volatile bool enc_state = 0;
static volatile unsigned int enc_cnt = 0; // step count
static volatile unsigned int enc_tick = 0; // tick count

static volatile unsigned int enc_step = 0;
static volatile unsigned int enc_trigger = 0;

// change if step is defined through other software part



void ENC_SetTrigger(unsigned int tmp){
	CS1_CriticalVariable();

	CS1_EnterCritical();
	enc_trigger = tmp;
	CS1_ExitCritical();
}

unsigned int ENC_GetTrigger(void){
	CS1_CriticalVariable();
	unsigned int tmp;
	CS1_EnterCritical();
	tmp = enc_trigger;
	CS1_ExitCritical();
	return tmp;
}

unsigned int ENC_GetVal(void){

	bool tmp = ENC1_GetVal();

    //WAIT1_Waitus(100); /* burn some time */
	if (enc_state != tmp){
		enc_state = tmp;
		enc_tick = 0;
		LED1_Neg();
	} else {
		if (enc_tick == ENC_TICK_CNT){
			enc_cnt++;
		}
		enc_tick++;
	}


    if(enc_cnt == (enc_step)){
    	/* motor off*/
    	//MOT_Speed(MOT_SPROC,0,MOT_FWD);
    	ENC_SetTrigger(1);
    	//LED1_Neg();
    	enc_cnt = 0;				// reset counter
    	return 1;
    } else{
    	return 0;
    }
    //return 0;
}

unsigned int ENC_GetHole(void){
	return ENC1_GetVal();
}



void ENC_ResetCnt(void){
	CS1_CriticalVariable();

	CS1_EnterCritical();
	enc_cnt = 0;
	CS1_ExitCritical();

}
unsigned int ENC_GetStep(){
	return enc_step;
}
void ENC_SetStep(unsigned int stepSize){
	enc_step = stepSize;
}

static void ENC_task(void *param) {
	unsigned int enc_val = 0;
	(void)param;
	for(;;) {
		// if Encoder counter is done turn off sprocket motor
		enc_val = ENC_GetVal();
		if(enc_val==1){
			MOT_Speed(MOT_SPROC,0,MOT_FWD);
		}
		vTaskDelay(pdMS_TO_TICKS(1));
	} /* for */
}

void ENC_Init(void){
	enc_state = 1;
	enc_cnt = 0;
	enc_tick = 0;
	enc_step = 4;
	enc_trigger = 0;
	if (xTaskCreate(ENC_task, "ENC", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL) != pdPASS) {
		for(;;){} /* error! probably out of memory */
	}
}

#endif
