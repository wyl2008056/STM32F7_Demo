
#ifndef __APP_CHARGER_H
#define __APP_CHARGER_H

#include "cmsis_os2.h"
#include "stm32f7xx_hal.h"
#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

#define APP_BACKGROUND_COLOR  0x00FF2424                //0x00E07208

#ifdef  __cplusplus
extern "C"
{
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmbattery_charging0_96px;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery_charging1_96px;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery_charging2_96px;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery_charging3_96px;
extern GUI_CONST_STORAGE GUI_BITMAP bmbattery_charging4_96px;

static GUI_CONST_STORAGE GUI_BITMAP* chargeIcons[] = 
{
    &bmbattery_charging0_96px,
    &bmbattery_charging1_96px,
    &bmbattery_charging2_96px,
    &bmbattery_charging3_96px,
    &bmbattery_charging4_96px
};

void AppChargerTaskCreat(osPriority_t prio);

#ifdef  __cplusplus
}
#endif


#endif