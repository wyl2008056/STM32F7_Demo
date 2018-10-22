
#ifndef __APP_LUNCHER_H
#define __APP_LUNCHER_H

#include "cmsis_os2.h"
#include "GUI.h"
#include "DIALOG.h"


#define APP_BACKGROUND_COLOR  0x00FF2424                //0x00E07208

#ifdef  __cplusplus
extern "C"
{
#endif



typedef struct {
    const GUI_BITMAP *pBitmap;
    const char *pText;
    void (*callBack)(WM_MESSAGE *, int);
} ICONVIEW_ITEM;


void onCalcClicked(WM_MESSAGE *pMsg, int sel);
void onClockClicked(WM_MESSAGE *pMsg, int sel);
void onSettingsClicked(WM_MESSAGE *pMsg, int sel);
void onHomeClicked(WM_MESSAGE *pMsg, int sel);
void onVideoClicked(WM_MESSAGE *pMsg, int sel);
void onMusicClicked(WM_MESSAGE *pMsg, int sel);
void onCpuInfoClicked(WM_MESSAGE *pMsg, int sel);
void onWechatClicked(WM_MESSAGE *pMsg, int sel);


void HideLauncher();
void ShowLauncher();

extern osSemaphoreId_t semSettings;

void AppLauncherTaskCreat(osPriority_t prio);



#ifdef  __cplusplus
}
#endif

#endif