#include <bsp.h>
#include <cmsis_os2.h>
#include "charger.h"
#include "stm32746g_discovery_lcd.h"

int main(void)
{
    HAL_StatusTypeDef stat;
    uint8_t i=0, readData = 0;

    BSP_Init();
    //BSP_LCD_DisplayOff();
    printf("start init charger\n");
    if(Charger_Init() != HAL_OK)
    {
        printf("charger init error!!!!!!!!!!!!!!!!!!!\n");
    }
    // for(i = 0; i < 0x2F; i++)
    // {
    //     stat = Charger_Read_Reg(i, &readData);
    //     printf("stat = %d, reg = 0x%02x, value = 0x%02x\n", stat, i, readData);
    //     HAL_Delay(1000);
    // }
    stat = Charger_Read_Reg(0x90, &readData);
    printf("stat = %d, reg = 0x%02x, value = 0x%02x\n", stat, 0x90, readData);
    Charger_Enable();
    while (true)
    {
        HAL_Delay(10000);
    }
}