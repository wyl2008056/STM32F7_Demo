#include "stm32f7xx_hal.h"
#include "GUI.h"
#include "GUIDRV_Lin.h"

#ifndef LCDCONF_H
#define LCDCONF_H



/* Define the possible screen orientations  */
#define ROTATION_0              0
#define ROTATION_CW             1
#define ROTATION_180            2
#define ROTATION_CCW            3

/* Define the possible color modes */
#define CMS_ARGB8888            1
#define CMS_RGB888              2
#define CMS_RGB565              3
#define CMS_ARGB1555            4
#define CMS_ARGB4444            5
#define CMS_L8                  6
#define CMS_AL44                7
#define CMS_AL88                8

/* Define physical screen sizes */
#define XSIZE_PHYS              480
#define YSIZE_PHYS              272

/* Define the number of buffers to use (minimum 1) */
#define NUM_BUFFERS             3
/* Define the number of virtual buffers to use (minimum 1) */
#define NUM_VSCREENS            1

/* Redefine number of layers to be used, must be equal or less than in GUIConf.h */
#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS          2

/* Define Layer 0 color mode, orientation and sizes */
#define COLOR_MODE_0            CMS_ARGB8888
#define ORIENTATION_0           ROTATION_CCW
#define XSIZE_0                 XSIZE_PHYS
#define YSIZE_0                 YSIZE_PHYS

/* Define Layer 1 color mode, orientation and sizes */
#define COLOR_MODE_1            CMS_ARGB1555
#define ORIENTATION_1           ROTATION_CCW
#define XSIZE_1                 XSIZE_PHYS
#define YSIZE_1                 YSIZE_PHYS

/* Define the background color shown where no layer is active */
#define BK_COLOR                GUI_DARKBLUE


#define DISPLAY_ORIENTATION GUI_ROTATION_CCW

// #undef  LCD_SWAP_XY
// #undef  LCD_MIRROR_Y
// #undef  LCD_SWAP_RB

// #define LCD_SWAP_XY  1 
// #define LCD_MIRROR_Y 1
// #define LCD_SWAP_RB  1

/* Choose Layer 0 color conversion depending of the color mode choosed */
#if   (COLOR_MODE_0 == CMS_ARGB8888)
    #define COLOR_CONVERSION_0    GUICC_M8888I
#elif (COLOR_MODE_0 == CMS_RGB888)
    #define COLOR_CONVERSION_0    GUICC_M888
#elif (COLOR_MODE_0 == CMS_RGB565)
    #define COLOR_CONVERSION_0    GUICC_M565
#elif (COLOR_MODE_0 == CMS_ARGB1555)
    #define COLOR_CONVERSION_0    GUICC_M1555I
#elif (COLOR_MODE_0 == CMS_ARGB4444)
    #define COLOR_CONVERSION_0    GUICC_M4444I
#elif (COLOR_MODE_0 == CMS_L8)
    #define COLOR_CONVERSION_0    GUICC_8666
#elif (COLOR_MODE_0 == CMS_AL44)
    #define COLOR_CONVERSION_0    GUICC_1616I
#elif (COLOR_MODE_0 == CMS_AL88)
    #define COLOR_CONVERSION_0    GUICC_88666I
#else
    #error Illegal color mode 0!
#endif

/* Choose Choose Layer 0 gui driver depending of the color mode choosed */
#if   (COLOR_MODE_0 == CMS_ARGB8888)
    #if   (ORIENTATION_0 == ROTATION_0)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_32
    #elif (ORIENTATION_0 == ROTATION_CW)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_32
    #elif (ORIENTATION_0 == ROTATION_180)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_32
    #elif (ORIENTATION_0 == ROTATION_CCW)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_32
    #endif
#elif (COLOR_MODE_0 == CMS_RGB888)
    #if   (ORIENTATION_0 == ROTATION_0)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_24
    #elif (ORIENTATION_0 == ROTATION_CW)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_24
    #elif (ORIENTATION_0 == ROTATION_180)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_24
    #elif (ORIENTATION_0 == ROTATION_CCW)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_24
    #endif
#elif (COLOR_MODE_0 == CMS_RGB565)   \
   || (COLOR_MODE_0 == CMS_ARGB1555) \
   || (COLOR_MODE_0 == CMS_ARGB4444) \
   || (COLOR_MODE_0 == CMS_AL88)
    #if   (ORIENTATION_0 == ROTATION_0)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
    #elif (ORIENTATION_0 == ROTATION_CW)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_16
    #elif (ORIENTATION_0 == ROTATION_180)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_16
    #elif (ORIENTATION_0 == ROTATION_CCW)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_16
    #endif
#elif (COLOR_MODE_0 == CMS_L8)   \
   || (COLOR_MODE_0 == CMS_AL44)
    #if   (ORIENTATION_0 == ROTATION_0)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_8
    #elif (ORIENTATION_0 == ROTATION_CW)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSX_8
    #elif (ORIENTATION_0 == ROTATION_180)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OXY_8
    #elif (ORIENTATION_0 == ROTATION_CCW)
        #define DISPLAY_DRIVER_0   GUIDRV_LIN_OSY_8
    #endif
#endif

/* Choose Layer 1 color conversion depending of the color mode//orientation choosed */
#if (GUI_NUM_LAYERS > 1)
    #if   (COLOR_MODE_1 == CMS_ARGB8888)
        #define COLOR_CONVERSION_1    GUICC_M8888I
    #elif (COLOR_MODE_1 == CMS_RGB888)
        #define COLOR_CONVERSION_1    GUICC_M888
    #elif (COLOR_MODE_1 == CMS_RGB565)
        #define COLOR_CONVERSION_1    GUICC_M565
    #elif (COLOR_MODE_1 == CMS_ARGB1555)
        #define COLOR_CONVERSION_1    GUICC_M1555I
    #elif (COLOR_MODE_1 == CMS_ARGB4444)
        #define COLOR_CONVERSION_1    GUICC_M4444I
    #elif (COLOR_MODE_1 == CMS_L8)
        #define COLOR_CONVERSION_1    GUICC_8666
    #elif (COLOR_MODE_1 == CMS_AL44)
        #define COLOR_CONVERSION_1    GUICC_1616I
    #elif (COLOR_MODE_1 == CMS_AL88)
        #define COLOR_CONVERSION_1    GUICC_88666I
    #else
        #error Illegal color mode 0!
    #endif
    /* Choose Choose Layer 1 gui driver depending of the color mode/orientation choosed */
    #if   (COLOR_MODE_1 == CMS_ARGB8888)
        #if   (ORIENTATION_1 == ROTATION_0)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_32
        #elif (ORIENTATION_1 == ROTATION_CW)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OSX_32
        #elif (ORIENTATION_1 == ROTATION_180)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OXY_32
        #elif (ORIENTATION_1 == ROTATION_CCW)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OSY_32
        #endif
    #elif (COLOR_MODE_1 == CMS_RGB888)
        #if   (ORIENTATION_1 == ROTATION_0)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_24
        #elif (ORIENTATION_1 == ROTATION_CW)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OSX_24
        #elif (ORIENTATION_1 == ROTATION_180)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OXY_24
        #elif (ORIENTATION_1 == ROTATION_CCW)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OSY_24
        #endif
    #elif (COLOR_MODE_1 == CMS_RGB565)   \
      || (COLOR_MODE_1 == CMS_ARGB1555) \
      || (COLOR_MODE_1 == CMS_ARGB4444) \
      || (COLOR_MODE_1 == CMS_AL88)
        #if   (ORIENTATION_1 == ROTATION_0)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_16
        #elif (ORIENTATION_1 == ROTATION_CW)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OSX_16
        #elif (ORIENTATION_1 == ROTATION_180)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OXY_16
        #elif (ORIENTATION_1 == ROTATION_CCW)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OSY_16
        #endif
    #elif (COLOR_MODE_1 == CMS_L8)   \
      || (COLOR_MODE_1 == CMS_AL44)
        #if   (ORIENTATION_1 == ROTATION_0)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_8
        #elif (ORIENTATION_1 == ROTATION_CW)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OSX_8
        #elif (ORIENTATION_1 == ROTATION_180)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OXY_8
        #elif (ORIENTATION_1 == ROTATION_CCW)
            #define DISPLAY_DRIVER_1    GUIDRV_LIN_OSY_8
        #endif
    #endif
#else
    /* Use complete display automatically in case of only one layer */
    #undef XSIZE_0
    #undef YSIZE_0
    #define XSIZE_0                 XSIZE_PHYS
    #define YSIZE_0                 YSIZE_PHYS
#endif

/* Check the that configuration used is OK */
#if NUM_BUFFERS > 3
    #error More than 3 buffers make no sense and are not supported in this configuration file!
#endif
#ifndef   XSIZE_PHYS
    #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
    #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_BUFFERS
    #define NUM_BUFFERS 1
#else
    #if (NUM_BUFFERS <= 0)
        #error At least one buffer needs to be defined!
    #endif
#endif
#ifndef   NUM_VSCREENS
    #define NUM_VSCREENS 1
#else
    #if (NUM_VSCREENS <= 0)
        #error At least one screeen needs to be defined!
    #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
    #error Virtual screens together with multiple buffers are not allowed!
#endif
     
/* Define the offset of the frame to be used */
#define LCD_FRAME_BUFFER ((U32)0xC0000000)
     
/**
  * @}
  */
/* Redirect bulk conversion to DMA2D routines */
#define DEFINE_DMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                        \
static void _Color2IndexBulk_##PFIX##_DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
    _DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                  \
}                                                                                                              \
static void _Index2ColorBulk_##PFIX##_DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
    _DMA_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);                                  \
}

typedef struct {
    int32_t address;
    __IO int32_t pending_buffer;
    int32_t buffer_index;
    int32_t xSize;
    int32_t ySize;
    int32_t BytesPerPixel;
    LCD_API_COLOR_CONV *pColorConvAPI;
} LCD_LayerPropTypedef;

#endif /* LCDCONF_H */

/*************************** End of file ****************************/
