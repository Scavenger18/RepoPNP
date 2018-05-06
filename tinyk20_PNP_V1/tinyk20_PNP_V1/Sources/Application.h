/*
 * Application.h
 *
 *  Created on: 17.02.2018
 *      Author: Erich Styger
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

typedef enum{
	FSM_INIT,
	FSM_IDLE,
	FSM_FWD,
	FSM_REV,
	FSM_RUN,
	FSM_STOP,
	FSM_ERROR
}FSM_State;

FSM_State taskState;

signed int APP_Counter;

void APP_Init(void);
void APP_Run(void);


#endif /* SOURCES_APPLICATION_H_ */
