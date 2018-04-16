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

#include "Application.h"

#if PL_ENCODER_EN
#include "encoder.h"
#endif


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
	uint8_t maxBuf = 64;
	unsigned char tmpCMD[maxBuf];
	unsigned char* ptr;

	UTIL1_strcpy(tmpCMD, maxBuf, "ERR 0 ");
	ptr = tmpCMD+sizeof("ERR 0 ")-1;
	UTIL1_strcpy(ptr, maxBuf, cmd);

	CLS1_SendStr((unsigned char*)tmpCMD, CLS1_GetStdio()->stdOut);
}

void COMM_SendStatus(const unsigned char *cmd){
	uint8_t maxBuf = 64;
	unsigned char tmpCMD[maxBuf];
	unsigned char* ptr;

	UTIL1_strcpy(tmpCMD, maxBuf, "STS 0 ");
	ptr = tmpCMD+sizeof("STS 0 ")-1;
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
    int8_t value;	//signed since address dekrementing falls below 0
    uint8_t res;
	const unsigned char *tmpCMD;

    // Check if it is command
    if (UTIL1_strncmp((char*)cmd, "CMD", sizeof("CMD")-1)==0){


    	tmpCMD = cmd+sizeof("CMD")-1;


		res = UTIL1_ScanDecimal8sNumber(&tmpCMD, &value);

		// if address is 0 execute command
		if(value == 0){
			tmpCMD = cmd+sizeof("CMD 0 ")-1;
//		  if (UTIL1_strcmp((char*)cmd, CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "COMM help")==0) {
//			*handled = TRUE;
//			CLS1_SendStr((unsigned char*)"FWD, REV, STEP (xx)mm", io->stdErr);
//			//return PrintHelp(io);
//		  }  else

			if (UTIL1_strcmp((char*)tmpCMD, "FWD")==0) {
				//CLS1_SendStr((unsigned char*)"STS 0 FWD OK", io->stdOut);
				COMM_SendStatus("FWD OK");
			*handled = TRUE;
			COMM_SetFWD();
			return ERR_OK;

			} else if (UTIL1_strcmp((char*)tmpCMD, "REV")==0) {
				//CLS1_SendStr((unsigned char*)"STS 0 REV OK", io->stdOut);
				COMM_SendStatus("REV OK");
			*handled = TRUE;
			COMM_SetREV();
			return ERR_OK;

			} else if (UTIL1_strncmp((char*)tmpCMD, "STEP", sizeof("STEP")-1)==0) {
				uint8_t value;
				uint8_t res;
				const unsigned char *p;

				p = tmpCMD+sizeof("STEP")-1;
				res = UTIL1_ScanDecimal8uNumber(&p, &value);

				if (*p=='m' ) { /* millimeter value && (*p-1)=='m' */
					value = (uint8_t) value;

					if(value == 1){
						//CLS1_SendStr((unsigned char*)"ERR 0 minimal step is 2mm", io->stdOut);
						COMM_SendError("minimal step is 2mm");
						value = 2;
						return ERR_VALUE;
					}
				//	CLS1_SendStr((unsigned char*)"Step size changed.", io->stdOut);
				//	CLS1_SendStr((unsigned char*)"STS 0 STEP OK", io->stdOut);
					COMM_SendStatus("STEP OK");
				}else{
					  //CLS1_SendStr((unsigned char*)"ERR 0 invalid number format", io->stdOut);
					COMM_SendError("invalid number format");
					return ERR_VALUE;

				}
				*handled = TRUE;
				#if PL_ENCODER_EN
					ENC_SetStep(value);
				#endif
				return ERR_OK;
			}
			return ERR_OK;


		  // else decrement address and send command on
		}else if(value > 0){//if (UTIL1_strncmp((char*)cmd, "0", sizeof("99")-1)>0) {

			unsigned char tmpStr[sizeof("99")-1];
			unsigned char *p;
			unsigned char *newAddress;

			uint8_t ind = strlen(cmd);

//			res = UTIL1_ScanDecimal8uNumber(&tmpCMD, &value);
			value--;

			UTIL1_Num8sToStr(tmpStr, ind, value);

			ind += strlen(tmpStr);
			ind += strlen("CMD ");

			unsigned char newCMD[ind];
//			newCMD[0] = '/0';

			tmpCMD = cmd+(sizeof("CMD ")+strlen(tmpStr)-1);

			UTIL1_strcpy(newCMD, ind, "CMD ");
			p = newCMD+sizeof("CMD ")-1;
			UTIL1_strcpy(p, ind, tmpStr);
			p = p+strlen(tmpStr)-1;
			UTIL1_strcat(p, ind, tmpCMD);

			CLS1_SendStr((unsigned char*)newCMD, io->stdOut);

			*handled = TRUE;
			return ERR_OK;

		}else {
		//	CLS1_SendStr((unsigned char*)cmd, io->stdOut);
		// negative address ?!?
			CLS1_SendStr((unsigned char*)"ERR 0 Negative CMD Address", io->stdOut);
			*handled = TRUE;
			return ERR_FAILED;

		}
	// else if it is an error to be passed on
	}else if (UTIL1_strncmp((char*)cmd, "ERR", sizeof("ERR")-1)==0){

		unsigned char tmpStr[sizeof("99")-1];
		unsigned char *p;
		unsigned char *newAddress;

    	tmpCMD = cmd+sizeof("ERR")-1;
		res = UTIL1_ScanDecimal8sNumber(&tmpCMD, &value);
		value--;

		uint8_t ind = strlen(cmd);
		UTIL1_Num8sToStr(tmpStr, ind, value);

		ind += strlen(tmpStr);
		ind += strlen("ERR ");

		unsigned char newCMD[ind];

		UTIL1_strcpy(newCMD, ind, "ERR ");
		p = newCMD+sizeof("ERR ")-1;
		UTIL1_strcpy(p, ind, tmpStr);
		p = p+strlen(tmpStr)-1;
		UTIL1_strcat(p, ind, tmpCMD);

		CLS1_SendStr((unsigned char*)newCMD, io->stdOut);
		*handled = TRUE;
		return ERR_OK;

	// else if status?
	}else if (UTIL1_strncmp((char*)cmd, "STS", sizeof("STS")-1)==0){

		unsigned char tmpStr[sizeof("99")-1];
		unsigned char *p;
		unsigned char *newAddress;

    	tmpCMD = cmd+sizeof("STS")-1;
		res = UTIL1_ScanDecimal8sNumber(&tmpCMD, &value);
		value--;

		uint8_t ind = strlen(cmd);
		UTIL1_Num8sToStr(tmpStr, ind, value);

		ind += strlen(tmpStr);
		ind += strlen("STS ");

		unsigned char newCMD[ind];

		UTIL1_strcpy(newCMD, ind, "STS ");
		p = newCMD+sizeof("STS ")-1;
		UTIL1_strcpy(p, ind, tmpStr);
		p = p+strlen(tmpStr)-1;
		UTIL1_strcat(p, ind, tmpCMD);

		CLS1_SendStr((unsigned char*)newCMD, io->stdOut);
		*handled = TRUE;
		return ERR_OK;

	// else generate unknown command error
	}else{
		CLS1_SendStr((unsigned char*)"ERR 0 unknown command", io->stdOut);
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





#endif
