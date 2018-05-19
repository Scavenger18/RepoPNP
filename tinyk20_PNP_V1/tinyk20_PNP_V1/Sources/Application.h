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
	FSM_REV_PEEL,
	FSM_REV_SPROC,
	FSM_RUN,
	FSM_STOP,
	FSM_ERROR
}FSM_State;

typedef enum{
	PEEL_OFF,
	PEEL_FWD,
	PEEL_REV,
	PEEL_FWD_DONE,
	PEEL_REV_DONE
}PEEL_State;

FSM_State taskState;
PEEL_State peelState;

signed int APP_Counter;

void APP_Init(void);
void APP_Run(void);


#endif /* SOURCES_APPLICATION_H_ */
