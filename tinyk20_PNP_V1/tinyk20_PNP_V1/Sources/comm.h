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

volatile uint8_t COMM_fwd, COMM_rev;


uint8_t	COMM_GetFWD();
uint8_t	COMM_GetREV();

uint8_t COMM_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);

#endif /* SOURCES_COMM_H_ */
