/*
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <stdlib.h>
#include <stdio.h>
#include <app_key.h>
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define  APP_CFG_TASK_PRIO                 9u
#define  APP_CFG_TASK_STK_SIZE            512u


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void  AppMain (void *p_arg)
{
    (void)p_arg; 
    bool displayOn = false;
    BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
    while (true) { 
        osDelay(200);
        if(BSP_PB_GetState(BUTTON_KEY) == 1)
        {
            if(displayOn)
            {
                displayOn = false;
                BSP_LCD_DisplayOff();
            }
            else
            {
                displayOn = true;
                BSP_LCD_DisplayOn();
            }
        }
    }
}


void AppKeyTaskCreat(osPriority_t prio) {
    osThreadNewEx(AppMain, 0, "Key Task", prio, APP_CFG_TASK_STK_SIZE);
}