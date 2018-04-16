/*
 * comm.h
 *
 *  Created on: 04.04.2018
 *      Author: Simon
 */

#ifndef SOURCES_COMM_H_
#define SOURCES_COMM_H_
#include "platform.h"
//#include "stdint.h"
#include "CLS1.h"

#define ADD_NEWLINE (1)

void COMM_SendError();

uint8_t COMM_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);
uint8_t COMM_ReadAndParse(uint8_t *cmdBuf, size_t cmdBufSize, CLS1_ConstStdIOType *io);

#endif /* SOURCES_COMM_H_ */
