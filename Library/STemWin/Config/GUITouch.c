#include "GUI.h"
#include "TouchConf.h"
#include "LCDConf.h"
#include "stm32746g_discovery_ts.h"



#if (ORIENTATION_0 ==ROTATION_0)
    #define GUI_TOUCH_SWAP_XY           0
    #define GUI_TOUCH_MIRROR_X          0
    #define GUI_TOUCH_MIRROR_Y          0
#elif (ORIENTATION_0 ==ROTATION_CW)
    #define GUI_TOUCH_SWAP_XY           1
    #define GUI_TOUCH_MIRROR_X          1
    #define GUI_TOUCH_MIRROR_Y          0
#elif (ORIENTATION_0 ==ROTATION_180)
    #define GUI_TOUCH_SWAP_XY           0
    #define GUI_TOUCH_MIRROR_X          1
    #define GUI_TOUCH_MIRROR_Y          1
#elif (ORIENTATION_0 ==ROTATION_CCW)
    #define GUI_TOUCH_SWAP_XY           1
    #define GUI_TOUCH_MIRROR_X          0
    #define GUI_TOUCH_MIRROR_Y          1
#endif


void GUI_TOUCH_Exec_Ex(void)
{  
    static GUI_PID_STATE TS_State = {0, 0, 0, 0};
    __IO TS_StateTypeDef  ts;
    uint16_t xDiff, yDiff;  

    BSP_TS_GetState((TS_StateTypeDef *)&ts);

    if((ts.touchX[0] >= TS_XSIZE_PHYS) ||(ts.touchY[0] >= TS_YSIZE_PHYS) ) 
    {
        ts.touchX[0] = 0;
        ts.touchY[0] = 0;
        ts.touchDetected =0;
    }

    xDiff = (TS_State.x > ts.touchX[0]) ? (TS_State.x - ts.touchX[0]) : (ts.touchX[0] - TS_State.x);
    yDiff = (TS_State.y > ts.touchY[0]) ? (TS_State.y - ts.touchY[0]) : (ts.touchY[0] - TS_State.y);


    if((TS_State.Pressed != ts.touchDetected ) || (xDiff > 30 )|| (yDiff > 30))
    {
        TS_State.Pressed = ts.touchDetected;
        TS_State.Layer = 0;
        if(ts.touchDetected) 
        {
            TS_State.x = ts.touchX[0];
            TS_State.y = ts.touchY[0];

            if(1 == GUI_TOUCH_MIRROR_X)
            {
                TS_State.x = TS_XSIZE_PHYS - TS_State.x;
            }
            if(1 == GUI_TOUCH_MIRROR_Y)
            {
                TS_State.y = TS_YSIZE_PHYS - TS_State.y;
            }

            if(1 == GUI_TOUCH_SWAP_XY)
            {
                int x = TS_State.x;
                TS_State.x = TS_State.y;
                TS_State.y = x;
            }
            GUI_TOUCH_StoreStateEx(&TS_State);
            //printf("TouchDetected: x = %d, y = %d\n", TS_State.x, TS_State.y);
        }
        else
        {
            GUI_TOUCH_StoreStateEx(&TS_State);
            TS_State.x = 0;
            TS_State.y = 0;
        }
    }
}

void GUI_TOUCH_X_ActivateX(void) 
{
    // GUI_PID_STATE TS_State = {0, 0, 0, 0};
    // TS_StateTypeDef  ts;
    // BSP_TS_GetState((TS_StateTypeDef *)&ts);
    // if(ts.touchDetected)
    // {
    //     TS_State.Pressed = ts.touchDetected;
    //     TS_State.Layer = 0;
    //     TS_State.x = ts.touchX[0];
    //     TS_State.y = ts.touchY[0];
    //     if(1 == GUI_TOUCH_SWAP_XY)
    //     {
    //         TS_State.x = TS_XSIZE_PHYS - TS_State.x;
    //         TS_State.y = TS_YSIZE_PHYS - TS_State.y;
    //     }
    //     if(1 == GUI_TOUCH_MIRROR_X)
    //     {
    //         TS_State.x = TS_XSIZE_PHYS - TS_State.x;
    //     }
    //     if(1 == GUI_TOUCH_MIRROR_Y)
    //     {
    //         TS_State.y = TS_YSIZE_PHYS - TS_State.y;
    //     }
    // }
    // GUI_TOUCH_StoreStateEx(&TS_State);
}

void GUI_TOUCH_X_ActivateY(void) 
{  
}

int  GUI_TOUCH_X_MeasureX(void) 
{
  	return -1;		  
}

int  GUI_TOUCH_X_MeasureY(void) 
{
  	return -1;			
}

//GUI_TOUCH_Exec()

