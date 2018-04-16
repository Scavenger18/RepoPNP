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

unsigned int BUT_GetState(BUT_Device button);

void BUT_Init(void);


#endif /* SOURCES_BUTTONS_H_ */
