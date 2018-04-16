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

#if PL_MOTOR_EN
//#include "motor.h"
#endif

volatile bool enc_state = 0;

// change if step is defined through other software part

void ENC_IntNew(void){

	if (enc_cnt==enc_step){
		enc_cnt = 0;
		ENC_SetTrigger(1);
	}
	enc_cnt++;
}


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
		LED1_Neg();
		enc_state = tmp;
		enc_cnt++;
	}


//    if(ENC1_GetVal()==ENC_INV){
//    	if(enc_flag == 0){
//    		//LED1_Neg();
//    		enc_cnt++;
//    		enc_flag = 1;
//    	}
//    }else{
//    	enc_flag = 0;
//    }

    if(enc_cnt == enc_step){
    	/* motor off*/
 //   	MOT_Speed(MOT_SPROC,0,MOT_FWD);	// -> moved to application
 //   	LED2_Neg();
    	ENC_SetTrigger(1);
    	enc_cnt = 0;				// reset counter
//    	enc_flag = 0;
    	return 1;
    }
    return 0;
}




void ENC_ResetFlag(){
	enc_flag = 0;
}

void ENC_ResetCnt(){
	enc_cnt = 0;
}

void ENC_SetStep(unsigned int stepSize){
	enc_step = stepSize;
}



#endif
