/*
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "stdio.h"
#include "DateTime.h"
#include "app_charger.h"
#include "charger.h"
#include "DIALOG.h"


using namespace System;
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  APP_CFG_TASK_STK_SIZE            1024*10


#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
#define ID_IMAGE_0      (GUI_ID_USER + 0x01)
#define ID_GRAPH_0      (GUI_ID_USER + 0x02)
#define ID_CHECKBOX_0   (GUI_ID_USER + 0x03)
#define ID_TEXT_0       (GUI_ID_USER + 0x04)
#define ID_TEXT_1       (GUI_ID_USER + 0x06)
#define ID_SLIDER_0     (GUI_ID_USER + 0x07)
#define ID_TEXT_2       (GUI_ID_USER + 0x08)

IMAGE_Handle chargeIcon;
CHECKBOX_Handle chkEnableChg;
TEXT_Handle  txtChgEnabled;
TEXT_Handle  txtChgState;
TEXT_Handle  txtBattVolt;
SLIDER_Handle sliderIchg;

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    " ", ID_WINDOW_0, 0, 0, 272, 480, 0, 0x0, 0 },
    { IMAGE_CreateIndirect,     " ", ID_IMAGE_0, 88, 13, 96, 96, 0, 0, 0 },
    { GRAPH_CreateIndirect,     " ", ID_GRAPH_0, 12, 159, 250, 112, 0, 0x0, 0 },
    { CHECKBOX_CreateIndirect,  " ", ID_CHECKBOX_0, 23, 286, 125, 20, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      " ", ID_TEXT_0, 175, 289, 80, 20, 0, 0x64, 0 },
    { TEXT_CreateIndirect,      " ", ID_TEXT_1, 12, 121, 247, 20, 0, 0x64, 0 },
    { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 12, 377, 236, 20, 0, 0x0, 0 },
    { TEXT_CreateIndirect, "Text", ID_TEXT_2, 18, 443, 80, 20, 0, 0x64, 0 },
};

static void _cbDialog(WM_MESSAGE *pMsg) {
    const void *pData;
    WM_HWIN hItem;
    U32 FileSize;
    int value;
    char tmp[5];
    int NCode;
    int Id;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, APP_BACKGROUND_COLOR);
        //
        // Initialization of 'chargeIcon'
        //
        chargeIcon = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
        IMAGE_SetBitmap(chargeIcon, chargeIcons[0]);
        //
        // Initialization of 'chkEnableCharger'
        //
        chkEnableChg = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
        CHECKBOX_SetText(chkEnableChg, "EnableCharger");
        CHECKBOX_SetFont(chkEnableChg, GUI_FONT_16B_ASCII);
        //
        // Initialization of 'txtChargeEnabledState'
        //
        txtChgEnabled = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetText(txtChgEnabled, "Disabled");
        TEXT_SetTextColor(txtChgEnabled, GUI_MAKE_COLOR(0x0000FF00));
        //
        // Initialization of 'txtChargeState'
        //
        txtChgState = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetText(txtChgState, "Chargeing");
        TEXT_SetTextAlign(txtChgState, GUI_TA_HCENTER | GUI_TA_VCENTER);
        // USER START (Optionally insert additional code for further widget
        // initialization) USER END
        sliderIchg = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
        SLIDER_SetRange(sliderIchg, 100, 4000);
        SLIDER_SetNumTicks(sliderIchg, 20);
        SLIDER_SetValue(sliderIchg, 2000);

        txtBattVolt = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id) {
        case ID_CHECKBOX_0: // Notifications sent by 'chkEnableCharger'
            switch (NCode) {
                case WM_NOTIFICATION_VALUE_CHANGED:
                    if(CHECKBOX_IsChecked(chkEnableChg))
                    {
                        Charger_Enable();
                    }
                    else
                    {
                        Charger_Disable();
                    }
                    break;
            }
            break;
        case ID_SLIDER_0: // Notifications sent by 'Slider'
            switch (NCode) {
                case WM_NOTIFICATION_VALUE_CHANGED:
                    value = SLIDER_GetValue(sliderIchg);
                    sprintf(tmp, "%d", value);
                    TEXT_SetText(txtChgEnabled, tmp);
                    Charger_Set_Ichg(value);
                    break;
            }
            break;
        }
        break;
    // USER START (Optionally insert additional message handling)
    // USER END
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

static void  AppMain (void *p_arg)
{
    ChargeState state;
    WM_HWIN hWin;
    uint8_t i=0;
    DateTime time;
    int battVolt;
    char tmp[20];
    (void)p_arg;

    GUI_Init();
    //WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_SetBkColor(APP_BACKGROUND_COLOR);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_16B_ASCII);
    GUI_Clear();

    if(Charger_Init() != 0)
    {
        printf("charger init error!!!!!!!!!!!!!!!!!!!\n");
    }

    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);

  
    while (true) { 
        time.Now()->GetTimeStr("mm/dd hh:MM:ss www", tmp);
        printf("%s\n", tmp);
        battVolt = Charger_Get_Batt_VoltageEx();
        sprintf(tmp,"vbat = %d mV", battVolt);
        TEXT_SetText(txtBattVolt, tmp);
        printf("%s\n", tmp);
        if(Charger_Get_Enabled())
        {
            TEXT_SetText(txtChgEnabled, "Enabled");
        }
        else
        {
            TEXT_SetText(txtChgEnabled, "Disabled");
        }

        state = Charger_Get_ChargeState();
        printf("charge State:%d\n", state);
        switch(state)
        {
            case NotCharging:
                TEXT_SetText(txtChgState, "NotCharging");
                break;
            case PreCharging:
                TEXT_SetText(txtChgState, "PreCharging");
                break;
            case FastCharging:
                TEXT_SetText(txtChgState, "FastCharging");
                break;
            case ChargeDone:
                TEXT_SetText(txtChgState, "ChargeDone");
                break;
            case UnKnow:
                TEXT_SetText(txtChgState, "ChargeUnKnow");
                break;
        }

        osDelay(1000);
        i++;
        if(i > 4) 
        {
            i = 0;
        }
        IMAGE_SetBitmap(chargeIcon, chargeIcons[i]);

    }
}

void AppChargerTaskCreat(osPriority_t prio)
{
    osThreadNewEx(AppMain, 0, "App Charger", prio, APP_CFG_TASK_STK_SIZE);
}