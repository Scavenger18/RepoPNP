/*
 * buttons.h
 *
 *  Created on: 25.03.2018
 *      Author: Simon
 */

#ifndef SOURCES_BUTTONS_H_
#define SOURCES_BUTTONS_H_

#include "platform.h"
//#include "stdint.h"

# define FWD_INVERT (1)
# define REV_INVERT (1)

#define LPRESS_CNT (200)

#if PL_TAPE_EN
# define MSW_INVERT	(0)
#endif

typedef enum{
#if PL_TAPE_EN
	BUT_MSW,
#endif
	BUT_FWD,
	BUT_REV
}BUT_Device;


// DO NOT ADD BEFORE BUT_PRESS!!! - Direct state acquisition from IO
typedef enum{
	BUT_IDLE,	//off state	- > do not change
	BUT_PRESS,		// on state - > do not change
	BUT_LPRESS
}BUT_State;


BUT_State BUT_GetState(BUT_Device button);

void BUT_Count(void);
void BUT_Process(void);
void BUT_Init(void);



#endif /* SOURCES_BUTTONS_H_ */
