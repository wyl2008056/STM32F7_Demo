/*
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "app_gui_ts.h"
#include "TouchConf.h"
#include "GUI.h"
#include "stdio.h"
#include "DateTime.h"

using namespace System;
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define  APP_CFG_TASK_PRIO                10u
#define  APP_CFG_TASK_STK_SIZE            512u


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void  AppMain (void *p_arg)
{
    DateTime time;
    GUI_PID_STATE pState;
    char tmp[10];
    (void)p_arg;
    while (true) { 
        GUI_TOUCH_Exec_Ex();
        // GUI_PID_GetCurrentState(&pState);
        // if(pState.Pressed)
        // {
        //     time.Now()->GetTimeStr("hh:MM:ss", tmp);
        //     printf("%s : x = %d, y = %d, layer = %d\n", tmp, pState.x, pState.y, pState.Layer);
        // }
        GUI_Exec();
        osDelay(50);
    }
}

void AppGuiTsTaskCreat(osPriority_t prio)
{
    osThreadNewEx(AppMain, 0, "App GUI TS", prio, APP_CFG_TASK_STK_SIZE);
}

