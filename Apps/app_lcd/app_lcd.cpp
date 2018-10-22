/*
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <stdlib.h>
#include <stdio.h>
#include <app_lcd.h>

#include "GUI.h"
#include "DateTime.h"
#include "DIALOG.h"

using namespace System;
/*

*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define APP_CFG_TASK_PRIO 7u
#define APP_CFG_TASK_STK_SIZE 1500u

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

/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/



/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_TEXT_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_0 (GUI_ID_USER + 0x02)
#define ID_CHECKBOX_0 (GUI_ID_USER + 0x03)
#define ID_DROPDOWN_0 (GUI_ID_USER + 0x04)
#define ID_EDIT_0 (GUI_ID_USER + 0x05)
#define ID_LISTBOX_0 (GUI_ID_USER + 0x07)
#define ID_MENU_0 (GUI_ID_USER + 0x08)
#define ID_MULTIPAGE_0 (GUI_ID_USER + 0x09)
#define ID_PROGBAR_0 (GUI_ID_USER + 0x0A)
#define ID_RADIO_0 (GUI_ID_USER + 0x0B)
#define ID_SLIDER_0 (GUI_ID_USER + 0x0C)
#define ID_LISTWHEEL_0 (GUI_ID_USER + 0x0D)
#define ID_BUTTON_1 (GUI_ID_USER + 0x0E)


int clickCount = 0;
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
    {WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 272, 480, 0, 0x0, 0},
    {TEXT_CreateIndirect, "Text", ID_TEXT_0, 0, 460, 272, 20, 0, 0x64, 0},
    {BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 9, 37, 80, 23, 0, 0x0, 0},
    {CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 148, 31, 80, 20, 0,
     0x0, 0},
    {DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 8, 70, 80, 18, 0, 0x0,
     0},
    {EDIT_CreateIndirect, "Edit", ID_EDIT_0, 148, 70, 80, 20, 0, 0x64, 0},
    {LISTBOX_CreateIndirect, "Listbox", ID_LISTBOX_0, 17, 230, 80, 60, 0, 0x0,
     0},
    {MENU_CreateIndirect, "Menu", ID_MENU_0, 0, 0, 274, 21, 0, 0x0, 0},
    {MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 6, 301, 266, 105, 0,
     0x0, 0},
    {PROGBAR_CreateIndirect, "Progbar", ID_PROGBAR_0, 8, 408, 253, 20, 0, 0x0,
     0},
    {RADIO_CreateIndirect, "Radio", ID_RADIO_0, 147, 232, 80, 60, 0, 0x1403, 0},
    {SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 4, 432, 262, 20, 0, 0x0, 0},
    {LISTWHEEL_CreateIndirect, "Listwheel", ID_LISTWHEEL_0, 51, 129, 98, 39, 0,
     0x0, 0},
    {BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 166, 132, 80, 25, 0, 0x0, 0},
    // USER START (Optionally insert additional widgets)
    // USER END
};

PROGBAR_Handle progressBar;
TEXT_Handle txtTime;
EDIT_Handle editCount;

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
    MENU_ITEM_DATA ItemData;
    WM_HWIN hItem;
    int NCode;
    int Id;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        //
        // Initialization of 'Window'
        //
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(0x00FFFFFF));
        //
        // Initialization of 'Text'
        //
        txtTime = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetText(txtTime, "12:53:55");
        TEXT_SetTextAlign(txtTime, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetFont(txtTime, GUI_FONT_24B_ASCII);
        TEXT_SetTextColor(txtTime, GUI_MAKE_COLOR(0x00000000));
        //
        // Initialization of 'Checkbox'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
        CHECKBOX_SetText(hItem, "Check");
        //
        // Initialization of 'Dropdown'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
        DROPDOWN_AddString(hItem, "Item");
        DROPDOWN_AddString(hItem, "Item");
        DROPDOWN_AddString(hItem, "Item");
        DROPDOWN_AddString(hItem, "Item");
        DROPDOWN_AddString(hItem, "Item");
        DROPDOWN_AddString(hItem, "Item");
        DROPDOWN_AddString(hItem, "Item");
        DROPDOWN_AddString(hItem, "Item");
        DROPDOWN_AddString(hItem, "Item");
        DROPDOWN_SetListHeight(hItem, 50);
        //
        // Initialization of 'Edit'
        //
        editCount = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
        EDIT_SetUlongMode(editCount, 0, 0, 500);
        //
        // Initialization of 'Listbox'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
        LISTBOX_AddString(hItem, "Item 0");
        LISTBOX_AddString(hItem, "Item 1");
        LISTBOX_AddString(hItem, "Item 2");
        //
        // Initialization of 'Menu'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_MENU_0);
        ItemData.Flags = 0;
        ItemData.hSubmenu = 0;
        ItemData.Id = 0;
        ItemData.pText = "Item";
        MENU_AddItem(hItem, &ItemData);
        ItemData.Flags = 0;
        ItemData.hSubmenu = 0;
        ItemData.Id = 0;
        ItemData.pText = "Item";
        MENU_AddItem(hItem, &ItemData);
        ItemData.Flags = 0;
        ItemData.hSubmenu = 0;
        ItemData.Id = 0;
        ItemData.pText = "Item";
        MENU_AddItem(hItem, &ItemData);
        ItemData.Flags = 0;
        ItemData.hSubmenu = 0;
        ItemData.Id = 0;
        ItemData.pText = "Item";
        MENU_AddItem(hItem, &ItemData);
        ItemData.Flags = 0;
        ItemData.hSubmenu = 0;
        ItemData.Id = 0;
        ItemData.pText = "Item";
        MENU_AddItem(hItem, &ItemData);
        ItemData.Flags = 0;
        ItemData.hSubmenu = 0;
        ItemData.Id = 0;
        ItemData.pText = "Item";
        MENU_AddItem(hItem, &ItemData);
        ItemData.Flags = 0;
        ItemData.hSubmenu = 0;
        ItemData.Id = 0;
        ItemData.pText = "Item";
        MENU_AddItem(hItem, &ItemData);
        ItemData.Flags = 0;
        ItemData.hSubmenu = 0;
        ItemData.Id = 0;
        ItemData.pText = "Item";
        MENU_AddItem(hItem, &ItemData);
        ItemData.Flags = 0;
        ItemData.hSubmenu = 0;
        ItemData.Id = 0;
        ItemData.pText = "Item";
        MENU_AddItem(hItem, &ItemData);
        //
        // Initialization of 'Multipage'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
        MULTIPAGE_AddEmptyPage(hItem, 0, "Page");
        MULTIPAGE_AddEmptyPage(hItem, 0, "Page");
        MULTIPAGE_AddEmptyPage(hItem, 0, "Page");
        MULTIPAGE_AddEmptyPage(hItem, 0, "Page");
        MULTIPAGE_AddEmptyPage(hItem, 0, "Page");
        //
        // Initialization of 'Radio'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
        RADIO_SetText(hItem, "Option", 0);
        RADIO_SetText(hItem, "Option", 1);
        RADIO_SetText(hItem, "Option", 2);
        //
        // Initialization of 'Listwheel'
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTWHEEL_0);
        LISTWHEEL_AddString(hItem, "String1");
        LISTWHEEL_AddString(hItem, "String2");
        LISTWHEEL_AddString(hItem, "String3");
        LISTWHEEL_AddString(hItem, "String4");
        LISTWHEEL_AddString(hItem, "String5");
        // USER START (Optionally insert additional code for further widget
        // initialization)
        // USER END
        progressBar = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id) {
        case ID_BUTTON_0: // Notifications sent by 'Button'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                clickCount++;
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further
                // notification handling)
                // USER END
            }
            break;
        case ID_CHECKBOX_0: // Notifications sent by 'Checkbox'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                clickCount++;
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further
                // notification handling)
                // USER END
            }
            break;
        case ID_DROPDOWN_0: // Notifications sent by 'Dropdown'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                clickCount++;
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further
                // notification handling)
                // USER END
            }
            break;
        case ID_EDIT_0: // Notifications sent by 'Edit'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further
                // notification handling)
                // USER END
            }
            break;
        case ID_LISTBOX_0: // Notifications sent by 'Listbox'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further
                // notification handling)
                // USER END
            }
            break;
        case ID_MENU_0: // Notifications sent by 'Menu'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further
                // notification handling)
                // USER END
            }
            break;
        case ID_MULTIPAGE_0: // Notifications sent by 'Multipage'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further
                // notification handling)
                // USER END
            }
            break;
        case ID_RADIO_0: // Notifications sent by 'Radio'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further
                // notification handling)
                // USER END
            }
            break;
        case ID_SLIDER_0: // Notifications sent by 'Slider'
            switch (NCode) {
            case WM_NOTIFICATION_CLICKED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_RELEASED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                // USER START (Optionally insert code for reacting on
                // notification
                // message)
                // USER END
                break;
                // USER START (Optionally insert additional code for further
                // notification handling)
                // USER END
            }
            break;
            // USER START (Optionally insert additional code for further Ids)
            // USER END
        }
        break;
    // USER START (Optionally insert additional message handling)
    // USER END
    default:
        WM_DefaultProc(pMsg);
        break;
    }
    EDIT_SetUlongMode(editCount, clickCount, 0, 500);
}

/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
void AppMain(void *pargs) {
    DateTime time;
    int progress = 0;
    int ticks = 0;
    char timestr[25];
    GUI_Init();
    WM_SetDesktopColor(GUI_DARKBLUE); /* Automacally update desktop window */
    WM_SetCreateFlags(
        WM_CF_MEMDEV); /* Use memory devices on all windows to avoid flicker */

    GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog,
                        WM_HBKWIN, 0, 0);
    while (true) {
        PROGBAR_SetValue(progressBar, progress);
        progress++;
        if (progress >= 100) {
            progress = 0;
        }
        TEXT_SetText(txtTime, time.Now()->GetTimeStr("yyyy-mm-dd hh:MM:ss www", timestr));
        //printf("%s\n", timestr);
        osDelay(1000);
    }
}

void AppLcdTaskCreat(osPriority_t prio) {
    printf("AppLcdTaskCreat\n");
    osThreadNewEx(AppMain, 0, "LCD Task", prio,
                  APP_CFG_TASK_STK_SIZE);
}
