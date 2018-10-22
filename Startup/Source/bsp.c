
/*
*********************************************************************************************************
*
*                               OS BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*
*                                              TEMPLATE
*
* Filename      : os_bsp.c
* Version       : V1.0.0
* Programmer(s) : WYL
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    CPU_BSP_MODULE

#include "os.h"
#include "bsp.h"

#include "stm32f7xx_hal_rtc.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "stm32746g_discovery_sdram.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  CPU_BSP_REG_DEMCR             (*(CPU_REG32 *)(0xE000EDFCu))
#define  CPU_BSP_REG_DWT_CR            (*(CPU_REG32 *)(0xE0001000u))
#define  CPU_BSP_REG_DWT_CYCCNT        (*(CPU_REG32 *)(0xE0001004u))
#define  CPU_BSP_REG_DWT_LAR           (*(CPU_REG32 *)(0xE0001FB0u))
#define  CPU_BSP_REG_DWT_LSR           (*(CPU_REG32 *)(0xE0001FB4u))

#define  CPU_BSP_BIT_DWT_LSR_SLK        DEF_BIT_01
#define  CPU_BSP_BIT_DWT_LSR_SLI        DEF_BIT_00

#define  CPU_BSP_DWT_LAR_KEY            0xC5ACCE55u

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/*
*********************************************************************************************************
*                                   BSP SYSTEM CLOCK INITIALIZATION
*
* Description : Board System clock initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : none.
*
* Note(s)     : (1) The PLLISAI_R frequency is being used by the TFT LCD. the LCD configures the clock
*                   required for the LCD Clock (LTDC Clock) under the LCDConf.c file, within the
*                   HAL_LTDC_MspInit() function.
*********************************************************************************************************
*/

void  BSP_SystemClkCfg (void)
{
    RCC_OscInitTypeDef        RCC_OscInit;
    RCC_ClkInitTypeDef        RCC_ClkInit;
    RCC_PeriphCLKInitTypeDef  PeriphClkInit;
    HAL_StatusTypeDef         hal_status;

                                                                /* VCO out-freq = HSE * (PLLN / PLLM)    = 400MHz.      */
                                                                /* PLLCLK       = (VCO out-freq) / PLLP  = 200MHz.      */
    RCC_OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;        /* HSE freq     = 25MHz.                                */
    RCC_OscInit.HSEState       = RCC_HSE_ON;
    RCC_OscInit.HSIState       = RCC_HSI_OFF;
    RCC_OscInit.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInit.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInit.PLL.PLLM       = 25u;
    RCC_OscInit.PLL.PLLN       = 400u;
    RCC_OscInit.PLL.PLLP       = RCC_PLLP_DIV2;
    RCC_OscInit.PLL.PLLQ       = 8;                            /* PLL_Q out freq = (VCO out-freq) / PLLQ = 50MHz.      */

    hal_status = HAL_RCC_OscConfig(&RCC_OscInit);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }

    hal_status = HAL_PWREx_EnableOverDrive();                   /* Activate the OverDrive to reach the 200 Mhz Freq     */
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }

                                                                /* ---- CFG PLLSAI FOR OTG-FS AND LTDC CLOCK SOURCE --- */
                                                                /* VCO out-freq = HSE * (PLLSAIN / PLLM) = 192MHz.      */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
    PeriphClkInit.Clk48ClockSelection  = RCC_CLK48SOURCE_PLLSAIP;
    PeriphClkInit.PLLSAI.PLLSAIN       = 192;
    PeriphClkInit.PLLSAI.PLLSAIQ       = 4;                     /* PLLISAI_Q freq = (VCO out-freq) / PLLSAIQ = 48MHz.   */
    PeriphClkInit.PLLSAI.PLLSAIR       = 5;                     /* PLLISAI_R freq = (VCO out-freq) / PLLSAIR = 38.4MHz. */
    PeriphClkInit.PLLSAI.PLLSAIP       = RCC_PLLSAIP_DIV4;      /* PLL48CLK freq  = (VCO out-freq) / PLLSAIP = 48MHz.   */

    hal_status = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }


    RCC_ClkInit.ClockType      = RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_HCLK   |
                                 RCC_CLOCKTYPE_PCLK1  |
                                 RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInit.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInit.AHBCLKDivider  = RCC_SYSCLK_DIV1;               /* HCLK    = AHBCLK  = PLLCLK / AHBPRES(1) = 200MHz.    */
    RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;                 /* APB1CLK = AHBCLK  / APB1DIV(4)          =  50MHz.    */
    RCC_ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;                 /* APB2CLK = AHBCLK  / APB2DIV(2)          = 100MHz.    */

    hal_status = HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_6);
    if (hal_status != HAL_OK) {
        while (DEF_TRUE) {                                      /* STOP if error                                        */
            ;
        }
    }
}


/*
*********************************************************************************************************
*                                          BSP_ClkFreqGet()
*
* Description : This function is used to retrieve System clocks frequencies.
*
* Argument(s) : clk_id    System clock identifier
*                             BSP_CLK_ID_SYSCLK     System clock frequency.
*                             BSP_CLK_ID_HCLK       CPU    clock frequency.
*                             BSP_CLK_ID_PCLK1      APB1   clock frequency.
*                             BSP_CLK_ID_PCLK2      APB2   clock frequency.
*
* Return(s)   : The specific clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_ClkFreqGet (BSP_CLK_ID  clk_id)
{
    CPU_INT32U  clk_freq;


    switch (clk_id) {
        case BSP_CLK_ID_SYSCLK:
             clk_freq = HAL_RCC_GetSysClockFreq();
             break;


        case BSP_CLK_ID_HCLK:
             clk_freq = HAL_RCC_GetHCLKFreq();
             break;


        case BSP_CLK_ID_PCLK1:
             clk_freq = HAL_RCC_GetPCLK1Freq();
             break;


        case BSP_CLK_ID_PCLK2:
             clk_freq = HAL_RCC_GetPCLK2Freq();
             break;


        default:
             clk_freq = 1u;                                     /* not a valid clock frequency.                         */
             break;
    }

    return (clk_freq);
}


/*
*********************************************************************************************************
*                                    INITIALIZE OS TICK INTERRUPT
*
* Description : Initialize the tick interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none
*********************************************************************************************************
*/

void  BSP_OSTickInit (void)
{
    CPU_INT32U  cpu_clk_freq;


    cpu_clk_freq = BSP_ClkFreqGet(BSP_CLK_ID_HCLK);             /* Determine SysTick reference freq.                    */

    OS_CPU_SysTickInitFreq(cpu_clk_freq);                       /* Init uC/OS periodic time src (SysTick).              */
}


void LCD_Config(void)
{
    /* LCD Initialization */ 
    BSP_LCD_Init();

    /* LCD Initialization */ 
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS+(BSP_LCD_GetXSize()*BSP_LCD_GetYSize()*4));

    /* Enable the LCD */ 
    BSP_LCD_DisplayOn(); 

    /* Select the LCD Background Layer  */
    BSP_LCD_SelectLayer(0);

    /* Clear the Background Layer */ 
    BSP_LCD_Clear(LCD_COLOR_RED);  

    /* Select the LCD Foreground Layer  */
    BSP_LCD_SelectLayer(1);

    /* Clear the Foreground Layer */ 
    BSP_LCD_Clear(LCD_COLOR_RED);

    /* Configure the transparency for foreground and background :
        Increase the transparency */
    BSP_LCD_SetTransparency(0, 0);
    BSP_LCD_SetTransparency(1, 100);
}

RTC_HandleTypeDef rtcHandle;

void RTC_Init(void)
{
    printf("start init RTC\n");
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct);

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE| RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	HAL_RCC_OscConfig (&RCC_OscInitStruct);

	__HAL_RCC_RTC_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();


	rtcHandle.Instance = RTC;
	rtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	rtcHandle.Init.AsynchPrediv = RTC_ASYNC_PREDIV ;
	rtcHandle.Init.SynchPrediv = RTC_SYNC_PREDIV;
	rtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	rtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	rtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&rtcHandle)!= HAL_OK) {
		printf("RTC init error");
	}
}

/*
*********************************************************************************************************
*                                    INITIALIZE BSP 
*
* Description : Initialize the chip.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : mian,  first called in main().
*
* Note(s)     : none
*********************************************************************************************************
*/
void  BSP_Init (void)
{
    CPU_CACHE_Enable();

    HAL_Init();                                                 
    
    BSP_SystemClkCfg(); 
    
    BSP_OSTickInit();

    BSP_SDRAM_Init(); /* Initializes the SDRAM device */
    
    __HAL_RCC_CRC_CLK_ENABLE(); /* Enable the CRC Module */
    
    BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

    BSP_LCD_Init();
    //LCD_Config();

    RTC_Init();
}


/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time
*                       but MUST be less than the maximum measured time; otherwise, timer resolution
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  fclk_freq;
    CPU_INT32U  reg_val;

                                                                /* ---- DWT WRITE ACCESS UNLOCK (CORTEX-M7 ONLY!!) ---- */
    reg_val = CPU_BSP_REG_DWT_LSR;                              /* Read lock status register.                           */
    if ((reg_val & CPU_BSP_BIT_DWT_LSR_SLI) != 0) {             /* Check if Software lock control mecanism exits        */
        if ((reg_val & CPU_BSP_BIT_DWT_LSR_SLK) != 0) {         /* Check if DWT access needs to be unlocked             */
            CPU_BSP_REG_DWT_LAR = CPU_BSP_DWT_LAR_KEY;          /* Unlock DWT write access.                             */
        }
    }

    fclk_freq = BSP_ClkFreqGet(BSP_CLK_ID_HCLK);

    CPU_BSP_REG_DEMCR      |= DEF_BIT_24;                       /* Set DEM_CR_TRCENA                                    */
    CPU_BSP_REG_DWT_CYCCNT  = 0u;
    CPU_BSP_REG_DWT_CR     |= DEF_BIT_00;                       /* Set DWT_CR_CYCCNTENA                                 */

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif


/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer period            Timer's period in some units of
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same
*                                                                   units of (fractional) seconds
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less
*                           than the maximum measured time; otherwise, timer resolution inadequate to
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;

    ts_tmr_cnts  = (CPU_TS_TMR)CPU_BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif


/*
*********************************************************************************************************
*                                         CPU_TSxx_to_uSec()
*
* Description : Convert a 32-/64-bit CPU timestamp from timer counts to microseconds.
*
* Argument(s) : ts_cnts   CPU timestamp (in timestamp timer counts [see Note #2aA]).
*
* Return(s)   : Converted CPU timestamp (in microseconds           [see Note #2aD]).
*
* Caller(s)   : Application.
*
*               This function is an (optional) CPU module application programming interface (API)
*               function which MAY be implemented by application/BSP function(s) [see Note #1] &
*               MAY be called by application function(s).
*
* Note(s)     : (1) CPU_TS32_to_uSec()/CPU_TS64_to_uSec() are application/BSP functions that MAY be
*                   optionally defined by the developer when either of the following CPU features is
*                   enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) The amount of time measured by CPU timestamps is calculated by either of
*                       the following equations :
*
*                                                                        10^6 microseconds
*                       (1) Time measured  =   Number timer counts   *  -------------------  *  Timer period
*                                                                            1 second
*
*                                              Number timer counts       10^6 microseconds
*                       (2) Time measured  =  ---------------------  *  -------------------
*                                                Timer frequency             1 second
*
*                               where
*
*                                   (A) Number timer counts     Number of timer counts measured
*                                   (B) Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                   (C) Timer period            Timer's period in some units of
*                                                                   (fractional)  seconds
*                                   (D) Time measured           Amount of time measured,
*                                                                   in microseconds
*
*                   (b) Timer period SHOULD be less than the typical measured time but MUST be less
*                       than the maximum measured time; otherwise, timer resolution inadequate to
*                       measure desired times.
*
*                   (c) Specific implementations may convert any number of CPU_TS32 or CPU_TS64 bits
*                       -- up to 32 or 64, respectively -- into microseconds.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;

    fclk_freq = BSP_ClkFreqGet(BSP_CLK_ID_HCLK);
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;


    fclk_freq = BSP_ClkFreqGet(BSP_CLK_ID_HCLK);
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif




/*
*********************************************************************************************************
*                                            HAL_InitTick()
*
* Description : This function has been overwritten from the STM32F7xx HAL libraries because Micrium's RTOS
*               has its own Systick initialization and because it is recomended to initialize the tick after
*               multi-tasking has started.
*
* Argument(s) : TickPriority          Tick interrupt priority.
*
* Return(s)   : HAL_OK.
*
* Caller(s)   : HAL_InitTick ()) is called automatically at the beginning of the program after reset by
*               HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
*
* Note(s)     : none.
*********************************************************************************************************
*/

// HAL_StatusTypeDef  HAL_InitTick (uint32_t  TickPriority)
// {
//     HAL_NVIC_SetPriorityGrouping(0);

//     return (HAL_OK);
// }


/*
*********************************************************************************************************
*                                            HAL_GetTick()
*
* Description : This function has been overwritten from the STM32F7xx HAL libraries because Micrium's OS's
*               has their own Tick counter values.
*
* Argument(s) : None.
*
* Return(s)   : Tick current value.
*
* Caller(s)   : STM32F7xx HAL driver files.
*
* Note(s)     : (1) Please ensure that the Tick Task has a higher priority than the App Start Task.
*********************************************************************************************************
*/

// uint32_t  HAL_GetTick(void)
// {
//     CPU_INT32U    os_tick_ctr;
//     OS_ERR  os_err;
    
//     os_tick_ctr = OSTimeGet(&os_err);

//     return os_tick_ctr;
// }