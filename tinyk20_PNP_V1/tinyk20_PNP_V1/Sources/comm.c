/*
 * comm.c
 *
 *  Created on: 04.04.2018
 *      Author: Simon
 */

#include "comm.h"


#if PL_COMM_EN
#include "CLS1.h"
#include "UTIL1.h"
#include "CS1.h"

#if PL_ENCODER_EN
#include "encoder.h"
#endif

uint8_t	COMM_GetFWD(){
	uint8_t tmp = COMM_fwd;
	COMM_fwd = 0;
	return tmp;
}

void COMM_SetFWD(uint8_t tmp){
	COMM_fwd = tmp;
}

uint8_t	COMM_GetREV(){
	uint8_t tmp = COMM_rev;
	COMM_rev = 0;
	return tmp;
}

void COMM_SetREV(uint8_t tmp){
	COMM_rev = tmp;
}




uint8_t COMM_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
    uint8_t value;
    uint8_t res;
    res = UTIL1_ScanDecimal8uNumber(&cmd, &value);

    res = UTIL1_strncmp((char*)cmd, " STEP", sizeof(" STEP")-1);

	//if (UTIL1_strncmp((char*)cmd, "0", 1)==0){
    if(value == 0){

	  if (UTIL1_strcmp((char*)cmd, CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "COMM help")==0) {
		*handled = TRUE;
		CLS1_SendStr((unsigned char*)"FWD, REV, STEP (xx)mm", io->stdErr);
		//return PrintHelp(io);
	  }  else if (UTIL1_strcmp((char*)cmd, " FWD")==0) {
		*handled = TRUE;
		COMM_SetFWD(1);
		//MOT_Speed(MOT_SPROC,40,MOT_FWD);
		return ERR_OK;
	  } else if (UTIL1_strcmp((char*)cmd, " REV")==0) {
		*handled = TRUE;
		COMM_SetREV(1);
		//MOT_Speed(MOT_SPROC,40,MOT_REV);
		return ERR_OK;
	  } else if (UTIL1_strncmp((char*)cmd, " STEP", sizeof(" STEP")-1)==0) {
			uint8_t value;
			uint8_t res;
			const unsigned char *p;

			p = cmd+sizeof(" STEP")-1;
			res = UTIL1_ScanDecimal8uNumber(&p, &value);

			if (*p=='m' ) { /* millimeter value && (*p-1)=='m' */
				value = (uint8_t) value;

				if(value == 1){
					CLS1_SendStr((unsigned char*)"*** Minimum Step is 2mm \r\n", io->stdErr);
					value = 2;
					return ERR_FAILED;
				}
				CLS1_SendStr((unsigned char*)"Step size changed.", io->stdOut);

			}else{
				  CLS1_SendStr((unsigned char*)"*** invalid number format! Try e.g. STEP 12mm\r\n", io->stdErr);
				  return ERR_FAILED;

			}
			*handled = TRUE;
			#if PL_ENCODER_EN
				ENC_SetStep(value);
			#endif
			return ERR_OK;
		  }
	  return ERR_OK;





	}else if(value > 0){//if (UTIL1_strncmp((char*)cmd, "0", sizeof("99")-1)>0) {
	    uint8_t ind = strlen(cmd)+5;

	    unsigned char newCMD[ind];
	    unsigned char tmpStr[ind];
	    unsigned char *p;
	    unsigned char *newAddress;

	    //UTIL1_strcpy(newCMD, ind,cmd );

	    res = UTIL1_ScanDecimal8uNumber(&cmd, &value);
	    value--;

	    UTIL1_Num8uToStr(tmpStr, ind, value);
	    UTIL1_strcpy(newCMD, ind, tmpStr);
	    UTIL1_strcat(newCMD,ind,cmd);

	    CLS1_SendStr((unsigned char*)newCMD, io->stdOut);



	}else {
		CLS1_SendStr((unsigned char*)cmd, io->stdOut);
	}



}



#endif
