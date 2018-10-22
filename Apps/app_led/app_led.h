
#ifndef __APP_LED_H
#define __APP_LED_H


#include "cmsis_os2.h"

#ifdef  __cplusplus
extern "C"
{
#endif


void AppLedTaskCreat(osPriority_t prio);

#ifdef  __cplusplus
}
#endif

#endif