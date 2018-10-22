/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include <stdio.h>
#include "app_launcher.h"
#include "DateTime.h"
#include "pictures.h"
#include "stm32746g_discovery_qspi.h"
#include "charger.h"

using namespace System;
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define  APP_CFG_TASK_STK_SIZE            1024*12


#define SCREEN_Width        270    /* 显示屏的宽度 */  
#define SCREEN_Height       480     /* 显示屏高度   */ 

#define TITLE_Height        24 


#define ICON_Width          48      /* 控件ICONVIEW的图标的宽度 */  
#define ICON_Height         64       /* 控件ICONVIEW的图标的高度, 含图标下面的文本，一起的高度 */ 

#define ICONVIEW_YSpace     25    /* 控件ICONVIEW的Y方向间距 */
#define ICONVIEW_XSpace     16    /* 控件ICONVIEW的Y方向间距 */

#define ICONVIEW_TBorder    16   /* 控件ICONVIEW的上边距 */
#define ICONVIEW_LBorder    16   /* 控件ICONVIEW的左边距 */  



#define ICONVIEW_Width      ICON_Width*4+ICONVIEW_XSpace*3             /* 控件ICONVIEW的宽 */  
#define ICONVIEW_Height     ICON_Height*4+ICONVIEW_YSpace*3    /* 控件ICONVIEW的高 */ 

#define B_ICON_Width        48      /* 控件ICONVIEW的图标的宽度 */  
#define B_ICON_Height       48       /* 控件ICONVIEW的图标的高度, 含图标下面的文本，一起的高度 */ 
#define B_ICONVIEW_XSpace   5    /* 控件ICONVIEW的Y方向间距 */

#define ICONVIEW_BBorder    8   /* 控件ICONVIEW的左边距 */ 

#define B_ICONVIEW_Width    B_ICON_Width*5+B_ICONVIEW_XSpace*4        /* 控件ICONVIEW的宽 */  
#define B_ICONVIEW_Height   B_ICON_Height                             /* 控件ICONVIEW的高 */

#define ID_TEXT_0           (GUI_ID_USER + 0x01)
#define ID_TEXT_1           (GUI_ID_USER + 0x02)

static WM_HWIN hIcon1, hIcon2;
static TEXT_Handle txtTime;
static TEXT_Handle txtVoltage;
static uint8_t s_ucIconSwitch = 0;
WM_HWIN hMotion;
GUI_RECT timeRect={5, 0, 130, TITLE_Height};
osSemaphoreId_t semSettings;
DateTime time;
char tmp[35];

void HideLauncher()
{
    WM_HideWindow(hMotion);
}

void ShowLauncher()
{
    WM_ShowWindow(hMotion);
}


/*
*********************************************************************************************************
*                               创建ICONVIEW控件所需变量
*********************************************************************************************************
*/ 


/* 用于第一屏桌面上ICONVIEW图标的创建 */
static const ICONVIEW_ITEM _aIconItem1[] = 
{
    {&bmCalculator_48px,"Calc",       &onCalcClicked},
    {&bmCalendar_48px,  "Calendar",   NULL},
    {&bmCamera_48px,    "Camera",     NULL},
    {&bmChart_48px,     "Chart",      NULL},

    {&bmChip_48px,      "CpuInfo",    &onCpuInfoClicked},
    {&bmClock_48px,     "Clock",      &onClockClicked},
    {&bmContacts_48px,  "Contacts",   NULL},
    {&bmDiccionary_48px,"Diccionary", NULL},

    {&bmFacetime_48px,  "Facetime",   NULL},
    {&bmGame_48px,      "Game",       NULL},
    {&bmMaps_48px,      "Maps",       NULL},
    {&bmMusic_48px,     "Music",      &onMusicClicked},

    {&bmNotes_48px,     "Notes",      NULL},
    {&bmPhotos_48px,    "Photos",     NULL},
    {&bmReminders_48px, "Reminder",   NULL},
    {&bmVideo_48px,     "Video",      &onVideoClicked},
};

static const ICONVIEW_ITEM _aIconItem2[] = 
{
    {&bmStocks_48px,    "",           NULL},
    {&bmWeather_48px,   "",           NULL},
    {&bmHome_48px,      "",           &onHomeClicked},
    {&bmSettings_48px,  "",           &onSettingsClicked},
    {&bmWechat_48px,    "",           &onWechatClicked},

};

void _paintDialog(WM_MESSAGE * pMsg)
{
    int x, i;
    GUI_SetBkColor(APP_BACKGROUND_COLOR);
    GUI_Clear();
    GUI_DrawBitmap(&bmbackground3, 0 , 0);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_16B_ASCII);
    time.Now()->GetTimeStr("mm/dd hh:MM:ss www", tmp);
    TEXT_SetText(txtTime, tmp);

    x = LCD_GetXSize();

    x = x - 31;
    GUI_DrawBitmap(&bmBatteryEmpty_27x14, x,5);
    GUI_DrawBitmap(apbmCharge[4], x, 5);

    x =  x - 20;
    GUI_DrawBitmap(&bmAlarm_16x16, x, 4);

    x = x - 22;
    GUI_SetColor(GUI_WHITE);
    for (i = 0; i < 5; i++) 
    {
        GUI_DrawVLine(x + i * 4 + 0, 14 - i * 2, 17);
        GUI_DrawVLine(x + i * 4 + 1, 14 - i * 2, 17);
    }

    x = x - 18;
    GUI_DrawBitmap(&bmsd_insert3_16px, x, 4);
    
    // x = x - 16;
    // GUI_DrawBitmap(&bmsd_removed3_16px, x, 4);
}

static void _cbMotion(WM_MESSAGE * pMsg) 
{
	WM_MOTION_INFO * pInfo;
	WM_HWIN          hWin = pMsg->hWin;
	int NCode, Id, sel;
	static uint32_t  tStart, tEnd;
	switch (pMsg->MsgId) 
	{
		case WM_PRE_PAINT:
			//GUI_MULTIBUF_Begin();
			break;
		
		case WM_POST_PAINT:
			//GUI_MULTIBUF_End();
			break;

        case WM_PAINT:
            _paintDialog(pMsg);
            break;
		
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);     
			NCode = pMsg->Data.v;                 
			switch (Id) 
			{
				/* 第一个界面上的图标 */
				case GUI_ID_ICONVIEW0:
                    switch (NCode) 
                    {
                        /* ICON控件点击消息 */
                        case WM_NOTIFICATION_CLICKED:
                            sel = ICONVIEW_GetSel(pMsg->hWinSrc);
                            if(sel < GUI_COUNTOF(_aIconItem1))
                            {
                                //printf("WM_NOTIFICATION_CLICKED:%d %s\n", sel, _aIconItem1[sel].pText);
                                if(_aIconItem1[sel].callBack != NULL)
                                {
                                    _aIconItem1[sel].callBack(pMsg, sel);
                                }
                            }
                            break;
                        case WM_NOTIFICATION_RELEASED: 
                            ICONVIEW_SetSel(pMsg->hWinSrc, -1);
                            break;
                    }
                    break;
					
				/* 第二个界面上的图标 */
				case GUI_ID_ICONVIEW1:
                    switch (NCode) 
                    {
                        /* ICON控件点击消息 */
                        case WM_NOTIFICATION_CLICKED:
                            sel = ICONVIEW_GetSel(pMsg->hWinSrc);
                            if(sel < GUI_COUNTOF(_aIconItem2))
                            {
                                //printf("WM_NOTIFICATION_CLICKED:%d %s\n", sel, _aIconItem2[sel].pText);
                                if(_aIconItem2[sel].callBack != NULL)
                                {
                                    _aIconItem2[sel].callBack(pMsg, sel);
                                }
                            }
                            break;
                        case WM_NOTIFICATION_RELEASED: 
                            ICONVIEW_SetSel(pMsg->hWinSrc, -1);
                            break;
                    }
                    break;
			}
			break;	
        case WM_MOTION:
            break;
  }
}

static WM_HWIN _CreateICONVIEW(WM_HWIN hParent, const ICONVIEW_ITEM *pBm,
                               int BitmapNum, int Id, int x, int y, int w,int h, 
                               int icoWith, int icoHeight, int xSpace, int ySpace, int align) {
    WM_HWIN hIcon;
    int i;

    hIcon = ICONVIEW_CreateEx(x,       /* 小工具的最左像素（在父坐标中）*/
                              y,       /* 小工具的最上像素（在父坐标中）*/
                              w,       /* 小工具的水平尺寸（单位：像素）*/
                              h,       /* 小工具的垂直尺寸（单位：像素）*/
                              hParent, /* 父窗口的句柄。如果为0，则新小工具将成为桌面（顶级窗口）的子窗口 */
                              WM_CF_SHOW | WM_CF_HASTRANS, /* 窗口创建标记。为使小工具立即可见，通常使用WM_CF_SHOW */
                              0,            /* ICONVIEW_CF_AUTOSCROLLBAR_V, 	默认是0，如果不够现实可设置增减垂直滚动条 */
                              Id,           /* 小工具的窗口ID */
                              icoWith,   /* 图标的水平尺寸 */
                              icoHeight); /* 图标的垂直尺寸,图标和文件都包含在里面，不要大于ICONVIEW的高度，导致Text显示不完整*/

    for (i = 0; i < BitmapNum; i++) {
        ICONVIEW_AddBitmapItem(hIcon, pBm[i].pBitmap, pBm[i].pText);
    }

    ICONVIEW_SetFont(hIcon, GUI_FONT_13_ASCII);

    /* 设置小工具的背景色 32 位颜色值的前8 位可用于alpha混合处理效果*/
    ICONVIEW_SetBkColor(hIcon, ICONVIEW_CI_SEL, GUI_WHITE | 0x80000000);

    /* 设置X方向的边界值为0，默认不是0,
     * Y方向默认是0，这里我们也进行一下设置，方便以后修改 */
    ICONVIEW_SetFrame(hIcon, GUI_COORD_X, 0);
    ICONVIEW_SetFrame(hIcon, GUI_COORD_Y, 0);

    /* 设置图标在x 或y 方向上的间距。*/
    ICONVIEW_SetSpace(hIcon, GUI_COORD_X, xSpace);
    ICONVIEW_SetSpace(hIcon, GUI_COORD_Y, ySpace);

    ICONVIEW_SetIconAlign(hIcon, align);

    // ICONVIEW_SetTextColor(hIcon, ICONVIEW_CI_UNSEL, GUI_WHITE);
    // ICONVIEW_SetTextColor(hIcon, ICONVIEW_CI_SEL, GUI_RED);
    
    ICONVIEW_SetSel(hIcon, -1);
    return hIcon;
}


static void  AppMain (void *p_arg)
{
    int batVol;
    Charger_Init();
    Charger_Enable();
    Charger_Set_Ichg(2000);
    BSP_QSPI_Init();
    //BSP_QSPI_Write((uint8_t*)bmbackground3.pData, BMBACKGROUND3_ADDRESS & 0x00fffff, 272*480*3);
    BSP_QSPI_EnableMemoryMappedMode();
    GUI_Init();
    WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_SetBkColor(APP_BACKGROUND_COLOR);
    GUI_SetColor(GUI_WHITE);
    GUI_SetFont(GUI_FONT_16B_ASCII);
    GUI_Clear();
    //GUI_SetTextMode(GUI_TEXTMODE_TRANS);

    //WM_MULTIBUF_Enable(1);

    // hMempic = GUI_MEMDEV_CreateFixed(0, 
	//                                  0, 
	//                                  LCD_GetXSize(), 
	//                                  LCD_GetYSize(), 
	// 								 GUI_MEMDEV_HASTRANS, 
	// 								 GUI_MEMDEV_APILIST_16, 
	// 								 GUICC_M565);
									 
	//GUI_MEMDEV_Select(hMempic);
    GUI_DrawBitmap(&bmbackground3, 0 , 0);
	//GUI_MEMDEV_Select(0);

    hMotion = WM_CreateWindowAsChild( 0, 
                                      0, 
                                      SCREEN_Width, 
                                      SCREEN_Height, 
                                      WM_HBKWIN,  
                                      WM_CF_MOTION_X | WM_CF_SHOW | WM_CF_HASTRANS, 
                                      _cbMotion, 
                                      0);

    txtTime = TEXT_CreateAsChild(0, 0, 220, TITLE_Height, hMotion, ID_TEXT_0, WM_CF_SHOW, "", 0);
    TEXT_SetFont(txtTime, GUI_FONT_16B_ASCII);
    TEXT_SetTextColor(txtTime, GUI_WHITE);
    TEXT_SetTextAlign(txtTime, GUI_TA_LEFT | GUI_TA_VCENTER);

    txtVoltage = TEXT_CreateAsChild(0, 20, 220, 16, hMotion, ID_TEXT_1, WM_CF_SHOW, "", 0);
    TEXT_SetFont(txtVoltage, GUI_FONT_16B_ASCII);
    TEXT_SetTextColor(txtVoltage, GUI_WHITE);
    TEXT_SetTextAlign(txtVoltage, GUI_TA_LEFT | GUI_TA_VCENTER);

    _CreateICONVIEW(hMotion, 
                    _aIconItem1, 
                    GUI_COUNTOF(_aIconItem1), 
                    GUI_ID_ICONVIEW0, 
                    ICONVIEW_LBorder, 
                    ICONVIEW_TBorder + TITLE_Height, 
                    ICONVIEW_Width, 
                    ICONVIEW_Height,
                    ICON_Width,
                    ICON_Height,
                    ICONVIEW_XSpace,
                    ICONVIEW_YSpace,
                    ICONVIEW_IA_HCENTER | ICONVIEW_IA_TOP);

    _CreateICONVIEW(hMotion, 
                    _aIconItem2, 
                    GUI_COUNTOF(_aIconItem2), 
                    GUI_ID_ICONVIEW1, 
                    B_ICONVIEW_XSpace, 
                    SCREEN_Height - B_ICON_Height - ICONVIEW_BBorder, 
                    B_ICONVIEW_Width, 
                    B_ICONVIEW_Height,
                    B_ICON_Width,
                    B_ICON_Height,
                    B_ICONVIEW_XSpace,
                    0,
                    ICONVIEW_IA_HCENTER | ICONVIEW_IA_TOP);

    while (true) { 
        if(WM_IsVisible(hMotion))
        {
            time.Now()->GetTimeStr("mm/dd hh:MM:ss www", tmp);
            batVol = Charger_Get_Batt_Voltage();
            TEXT_SetText(txtTime, tmp);
            sprintf(tmp, "Battery vol: %d mV", batVol);
            TEXT_SetText(txtVoltage, tmp);
        }
        osDelay(1000);
    }
}

void AppLauncherTaskCreat(osPriority_t prio)
{
    semSettings = osSemaphoreNewEx(0, "semSettings");
    osThreadNewEx(AppMain, 0, "App luncher", prio, APP_CFG_TASK_STK_SIZE);
}

