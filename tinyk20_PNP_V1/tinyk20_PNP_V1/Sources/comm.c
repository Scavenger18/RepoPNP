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
#include "FRTOS1.h"

#include "Application.h"

#if PL_ENCODER_EN
#include "encoder.h"
#endif

#define MAX_BUFF (64)
static unsigned char buffer[MAX_BUFF];

//static const CLS1_ConstParseCommandCallback cmdTable[] =
//{
////    CLS1_ParseCommand,
//COMM_ParseCommand,
////    LED2_ParseCommand,
////    LED3_ParseCommand,
////    MMA1_ParseCommand,
//NULL/* sentinel */
//};


void COMM_SetFWD(){
	CS1_CriticalVariable();

	CS1_EnterCritical();
	APP_Counter++;
	CS1_ExitCritical();
}

void COMM_SetREV(){
	CS1_CriticalVariable();

	CS1_EnterCritical();
	APP_Counter--;
	CS1_ExitCritical();
}

void COMM_SendError(const unsigned char *cmd){
	uint8_t maxBuf = MAX_BUFF;
	unsigned char tmpCMD[maxBuf];
	unsigned char* ptr;

	UTIL1_strcpy(tmpCMD, maxBuf, "CMD -1 ERR ");
	ptr = tmpCMD+sizeof("CMD -1 ERR ")-1;
	UTIL1_strcpy(ptr, maxBuf, cmd);

	CLS1_SendStr((unsigned char*)tmpCMD, CLS1_GetStdio()->stdOut);
}

void COMM_SendStatus(const unsigned char *cmd){
	uint8_t maxBuf = MAX_BUFF;
	unsigned char tmpCMD[maxBuf];
	unsigned char* ptr;

	UTIL1_strcpy(tmpCMD, maxBuf, "CMD -1 ");
	ptr = tmpCMD+sizeof("CMD -1 ")-1;
	UTIL1_strcpy(ptr, maxBuf, cmd);

	CLS1_SendStr((unsigned char*)tmpCMD, CLS1_GetStdio()->stdOut);
}
/*
 * CMD AND ERROR DECLARATION
 *
 * Each Message String begins with either CMD or ERR
 * This defines if the address is to be evaluated as command or
 * incremented and passed on for error evaluation in OpenPnP
 *
 * The address follows the CMD/ERR header. Then followed by the command/message
 * which is evaluated in a second step.
 *
 * CMD 0 FWD | CMD 0 REV | CMD 0 STEP xxmm |
 * ERR 0 Unknown command | ERR 0 Incorrect Step Format | ERR 0 Software Error |
 */


uint8_t COMM_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io)
{
    int8_t value;	//signed since address decrementing falls below 0
    uint8_t res;
    const unsigned char *p;
	const unsigned char *tmpCMD;

    // Check if it is command
    if (UTIL1_strncmp((char*)cmd, "CMD", sizeof("CMD")-1)==0){

    	tmpCMD = cmd+sizeof("CMD")-1;

		res = UTIL1_ScanDecimal8sNumber(&tmpCMD, &value);

		// if address is 0 execute command
		if(value == 0){
			tmpCMD = cmd+sizeof("CMD 0 ")-1;

			if (UTIL1_strncmp((char*)tmpCMD, "FWD", sizeof("FWD")-1)==0){
				p = tmpCMD+sizeof("FWD")-1;
				res = UTIL1_ScanDecimal8uNumber(&p, &value);
				*handled = TRUE;
				if((value >= 2)&&(value%2 == 0)){	// number of steps (in mm)
					//ENC_SetStep(value);
					while (value > 0){
						COMM_SetFWD();
						value = value-2;
					}
					COMM_SendStatus("FWD ok");
					return ERR_OK;
				}else {
					// wrong step size
					COMM_SendError("invalid step size");
					return ERR_VALUE;
				}


			} else if (UTIL1_strncmp((char*)tmpCMD, "REV", sizeof("REV")-1)==0){ //(UTIL1_strcmp((char*)tmpCMD, "REV")==0) {

				p = tmpCMD+sizeof("REV")-1;
				res = UTIL1_ScanDecimal8uNumber(&p, &value);
				*handled = TRUE;
				if((value >= 2)&&(value%2 == 0)){	// number of steps (in mm)
					//ENC_SetStep(value);
					while (value > 0){
						COMM_SetREV();
						value = value-2;
					}
					COMM_SendStatus("REV ok");
					return ERR_OK;
				}else {
					// wrong step size
					COMM_SendError("invalid step size");
					return ERR_VALUE;
				}
			}else {
				// catch false/ unknown command
				COMM_SendError("unknown command");
				return ERR_VALUE;
			}
			return ERR_OK;


		  // else decrement address and send command on
		}else {		//else if(value > 0){//if (UTIL1_strncmp((char*)cmd, "0", sizeof("99")-1)>0) {

			unsigned char tmpStr[sizeof("999")-1];
			unsigned char *p;
			unsigned char *newAddress;

			uint8_t ind = strlen(cmd);
			value--;

			UTIL1_Num8sToStr(tmpStr, ind, value);

			ind += strlen(tmpStr);
			ind += strlen("CMD ");

			unsigned char newCMD[ind];

			tmpCMD = cmd+(sizeof("CMD ")+strlen(tmpStr)-1);

			UTIL1_strcpy(newCMD, ind, "CMD ");
			p = newCMD+sizeof("CMD ")-1;
			UTIL1_strcpy(p, ind, tmpStr);
			p = p+strlen(tmpStr)-1;
			UTIL1_strcat(p, ind, tmpCMD);

			CLS1_SendStr((unsigned char*)newCMD, io->stdOut);

			*handled = TRUE;
			return ERR_OK;
		}
	// else generate unknown command error
	}else{
		COMM_SendError("unknown message");
		*handled = TRUE;
		return ERR_FAILED;
	}
}


uint8_t COMM_ReadAndParse(uint8_t *cmdBuf, size_t cmdBufSize, CLS1_ConstStdIOType *io)
{
  /* IMPORTANT NOTE: this function *appends* to the buffer, so the buffer needs to be initialized first! */
  uint8_t res = ERR_OK;
  size_t len;
  bool handled;


  if (io==NULL) { /* no I/O handler? */
    return ERR_FAILED;
  }
  len = UTIL1_strlen((const char*)cmdBuf);
  if (CLS1_ReadLine(cmdBuf, cmdBuf+len, cmdBufSize-len, io)) {
    len = UTIL1_strlen((const char*)cmdBuf); /* length of buffer string */
    if (len==0) { /* error case */
      return ERR_FAILED;
    } else if (len==1 && (cmdBuf[0]=='\n' || cmdBuf[0]=='\r')) { /* eat preceding newline characters */
      cmdBuf[0] = '\0';
    }
    if (len>=cmdBufSize-1) {           /* buffer overflow? Parse what we have, will be likely return an error */
      //(void)CLS1_ParseWithCommandTable(cmdBuf, io, parseCallback);
      (void)COMM_ParseCommand(cmdBuf, &handled, io);
#if ADD_NEWLINE
      CLS1_SendStr((unsigned char*)"\r\n", io->stdOut);
#endif
     cmdBuf[0] = '\0'; /* start again */
      res = ERR_OVERFLOW;
    } else if (cmdBuf[len-1]=='\n' || cmdBuf[len-1]=='\r') { /* line end: parse command */
      cmdBuf[len-1] = '\0';            /* remove line end character for parser */
      res = COMM_ParseCommand(cmdBuf, &handled, io);
#if ADD_NEWLINE
      CLS1_SendStr((unsigned char*)"\r\n", io->stdOut);
#endif
      cmdBuf[0] = '\0';                /* start again */
    } else {
      /* continue to append to buffer */
    }
  }
  return res;
}



static void COMM_task(void *param) {
   (void)param;
   for(;;) {
		 //error_res = COMM_ReadAndParse(buffer, sizeof(buffer), CLS1_GetStdio());
		(void)COMM_ReadAndParse(buffer, sizeof(buffer), CLS1_GetStdio());
		vTaskDelay(pdMS_TO_TICKS(100));
	} /* for */
 }

void COMM_Init(void){
	buffer[0] = '\0';

	if (xTaskCreate(COMM_task, "UART", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, NULL) != pdPASS) {
		for(;;){} /* error! probably out of memory */
	}
}

#endif
