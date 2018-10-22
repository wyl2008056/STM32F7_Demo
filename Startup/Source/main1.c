#include <bsp.h>
#include <cmsis_os2.h>
#include "pictures.h"
#include "stm32746g_discovery_qspi.h"
#include "GUI.h"


#define MSK  0x000FFFFF

int main(void)
{
    BSP_Init();
	BSP_QSPI_Init();
	//BSP_QSPI_EnableMemoryMappedMode();
    GUI_Init();
	printf("Start to write flash\n");
    BSP_QSPI_Write((uint8_t*)bmCalculator_48px.pData, BMCALCULATOR_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmCalendar_48px.pData, BMCALENDAR_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmCamera_48px.pData, BMCAMERA_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmChart_48px.pData, BMCHART_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmChip_48px.pData, BMCHIP_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmClock_48px.pData, BMCLOCK_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmContacts_48px.pData, BMCONTACTS_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmDiccionary_48px.pData, BMDICCIONARY_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmFacetime_48px.pData, BMFACETIME_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmGame_48px.pData, BMGAME_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmHome_48px.pData, BMHOME_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmMaps_48px.pData, BMMAPS_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmMusic_48px.pData, BMMUSIC_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmNotes_48px.pData, BMNOTES_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmPhotos_48px.pData, BMPHOTOS_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmReminders_48px.pData, BMREMINDERS_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmSettings_48px.pData, BMSETTINGS_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmStocks_48px.pData, BMSTOCKS_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmVideo_48px.pData, BMVIDEO_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmWeather_48px.pData, BMWEATHER_48PX_ADDRESS & MSK, 48 * 48 * 4);
    BSP_QSPI_Write((uint8_t*)bmWechat_48px.pData, BMWECHAT_48PX_ADDRESS & MSK, 48 * 48 * 4);
    printf("write flash success\n");
	
    while (true); // Should Never Get Here.
}