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

/*
 * Initialise Communication Buffer
 * Start COMM Task
 */
void COMM_Init(void);


/*
 * Send Error Messages
 * \todo Improve to implement queueing of messages
 */
void COMM_SendError(const unsigned char *cmd);



#endif /* SOURCES_COMM_H_ */
