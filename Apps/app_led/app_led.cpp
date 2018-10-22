/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <stdio.h>
#include <app_led.h>
#include "GUI.h"
#include "DateTime.h"

using namespace System;

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define APP_CFG_TASK_PRIO 5u
#define APP_CFG_TASK_STK_SIZE 1024u

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
#include "DIALOG.h"


static SPINBOX_Handle spinYear;
static SPINBOX_Handle spinMonth;
static SPINBOX_Handle spinDay;
static SPINBOX_Handle spinWeekDay;
static SPINBOX_Handle spinHour;
static SPINBOX_Handle spinMinute;
static SPINBOX_Handle spinSecond;
static TEXT_Handle txtTime;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_SPINBOX_0  (GUI_ID_USER + 0x01)
#define ID_SPINBOX_1  (GUI_ID_USER + 0x02)
#define ID_SPINBOX_2  (GUI_ID_USER + 0x03)
#define ID_SPINBOX_3  (GUI_ID_USER + 0x04)
#define ID_SPINBOX_4  (GUI_ID_USER + 0x05)
#define ID_SPINBOX_5  (GUI_ID_USER + 0x06)
#define ID_SPINBOX_6  (GUI_ID_USER + 0x07)
#define ID_BUTTON_0  (GUI_ID_USER + 0x08)
#define ID_TEXT_0  (GUI_ID_USER + 0x09)

// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/




// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 1, 1, 480, 240, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 12, 15, 80, 42, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_1, 105, 16, 80, 42, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_2, 204, 17, 80, 47, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_3, 302, 15, 80, 49, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_4, 10, 78, 80, 50, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_5, 99, 78, 80, 49, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_6, 199, 80, 80, 45, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 8, 139, 338, 40, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 10, 190, 449, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE *pMsg) {
    WM_HWIN hItem;
    int NCode;
    int Id;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:

        spinYear = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
        SPINBOX_SetRange(spinYear, 0, 100);
        SPINBOX_SetValue(spinYear, 0);


        spinMonth = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1);
        SPINBOX_SetValue(spinMonth, 0);

        spinDay = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2);
        SPINBOX_SetValue(spinDay, 0);

        spinWeekDay = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_3);
        SPINBOX_SetValue(spinWeekDay, 0);

        spinHour = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_4);
        SPINBOX_SetValue(spinHour, 0);

        spinMinute = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_5);
        SPINBOX_SetValue(spinMinute, 0);

        spinSecond = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_6);
        SPINBOX_SetValue(spinSecond, 0);

        txtTime = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetFont(txtTime, GUI_FONT_16B_ASCII);
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id) {
        case ID_BUTTON_0: // Notifications sent by 'Button'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                DateTime timeSet;
                int year = SPINBOX_GetValue(spinYear);
                int month = SPINBOX_GetValue(spinMonth);
                int day = SPINBOX_GetValue(spinDay);
                int weekday = SPINBOX_GetValue(spinWeekDay);
                int hour = SPINBOX_GetValue(spinHour);
                int minute = SPINBOX_GetValue(spinMinute);
                int second = SPINBOX_GetValue(spinSecond);
                timeSet.SetDateTime(year, month, day, weekday, hour, minute, second);
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/* --------------- APPLICATION GLOBALS ---------------- */
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void AppMain(void *p_arg) {
    DateTime time;
    char timestr[30];
    (void)p_arg;
    GUI_Init();
    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog,
                        WM_HBKWIN, 0, 0);
    while (true) {
        TEXT_SetText(txtTime,
                     time.Now()->GetTimeStr("yyyy-mm-dd w hh:MM:ss mmm www", timestr));
        printf("%s\n", timestr);
        osDelay(1000);
    }
}

void AppLedTaskCreat(osPriority_t prio) {
    osThreadNewEx(AppMain, 0, "App Led", prio,
                  APP_CFG_TASK_STK_SIZE);
}
