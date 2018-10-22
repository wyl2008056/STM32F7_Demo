/*
*********************************************************************************************************
*                                         INCLUDE FILES
*********************************************************************************************************
*/
#include "app_cfg.h"
#include "app_led.h"
#include "app_lcd.h"
#include "app_key.h"
#include "app_sd.h"
#include "app_gui_ts.h"
#include "app_state.h"
#include "app_launcher.h"
#include "app_charger.h"
#include "app_settings.h"

/*
*********************************************************************************************************
*                                          AppObjCreate()
*
* Description : Create Application Kernel Objects.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  AppObjCreate (void)
{

}

/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create Application Tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  AppTaskCreate (void)
{
    
    //AppChargerTaskCreat(osPriorityRealtime);
    AppKeyTaskCreat(osPriorityBelowNormal);

    //AppLedTaskCreat(); 
    //AppStateTaskCreat();
    AppGuiTsTaskCreat(osPriorityLow);
    AppLauncherTaskCreat(osPriorityLow2);
    AppSettingsTaskCreat(osPriorityLow1);
    //AppLcdTaskCreat(osPriorityLow3);
}

