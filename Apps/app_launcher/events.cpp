#include <stdio.h>
#include "app_launcher.h"
#include "stm32746g_discovery_qspi.h"
#include "pictures.h"
#include "DateTime.h"

using namespace System;

extern WM_HWIN hMotion;

void onCalcClicked(WM_MESSAGE *pMsg, int sel)
{
    printf("Start to write flash\n");
    //BSP_QSPI_Write((uint8_t*)bmbackground3.pData, BMBACKGROUND3_ADDRESS, 272*480*3);
    printf("write flash success\n");
}

void onClockClicked(WM_MESSAGE *pMsg, int sel)
{
    uint8_t pData[20], i;
    BSP_QSPI_Read(pData, 0, 20);
    for(i=0; i<20; i++)
    {
        printf("%02x ", pData[i]);
    }
    printf("onClockClicked\n");
    BSP_QSPI_Read(pData, bmCalculator_48px.XSize * bmCalculator_48px.YSize * 4 - 20, 20);
    for(i=0; i<20; i++)
    {
        printf("%02x ", pData[i]);
    }
    printf("onClockClicked\n");
}

void onSettingsClicked(WM_MESSAGE *pMsg, int sel)
{
    printf("onSettingsClicked\n");
    HideLauncher();
    osSemaphoreRelease(semSettings);
    
}

void onHomeClicked(WM_MESSAGE *pMsg, int sel)
{
    printf("onHomeClicked\n");
}

void onVideoClicked(WM_MESSAGE *pMsg, int sel)
{
    printf("onVideoClicked\n");
}

void onMusicClicked(WM_MESSAGE *pMsg, int sel)
{
    DateTime time;
    printf("onMusicClicked\n");
    time.SetDate(2018, 10, 20, 6);
}

void onCpuInfoClicked(WM_MESSAGE *pMsg, int sel)
{
    DateTime time;
    printf("onCpuInfoClicked\n");
    time.SetTime(1, 38, 0);
}

void onWechatClicked(WM_MESSAGE *pMsg, int sel)
{
    printf("onWechatClicked\n");
}