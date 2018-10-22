/*
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include  <os.h>
#include  <stdlib.h>
#include  <stdio.h>
#include  <stm32746g_discovery_sd.h>
#include  <app_sd.h>



/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define  APP_CFG_TASK_PRIO                  7u
#define  APP_CFG_TASK_STK_SIZE            256u

static  OS_TCB   AppTaskTCB;
static  CPU_STK  AppTaskStk[APP_CFG_TASK_STK_SIZE];

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

static void  AppMain (void *p_arg)
{
    OS_ERR  err;
    (void)p_arg;
    BSP_SD_Init();

    uint8_t sd_status = SD_NOT_PRESENT;
    while (DEF_TRUE) { 
        if(BSP_SD_IsDetected() != sd_status)
        {
            sd_status = BSP_SD_IsDetected();
            if(sd_status == SD_PRESENT)
            {
                printf("SD card detected!\n");
                HAL_SD_CardInfoTypeDef CardInfo;
                BSP_SD_GetCardInfo(&CardInfo);
                printf("SD card info:\n");
                printf("  CardType:%d\n",CardInfo.CardType);
                printf("  CardVersion:%d\n",CardInfo.CardVersion);
                printf("  Class:%d\n",CardInfo.Class);
                printf("  RelCardAdd:%d\n",CardInfo.RelCardAdd);
                printf("  BlockNbr:%d\n",CardInfo.BlockNbr);
                printf("  BlockSize:%d\n",CardInfo.BlockSize);
                printf("  LogBlockNbr:%d\n",CardInfo.LogBlockNbr);
                printf("  LogBlockSize:%d\n",CardInfo.LogBlockSize);
            }
            else
            {
                printf("SD card removed!\n");
            }

        }
        OSTimeDlyHMSM(0u, 0u, 0u, 250u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

void AppSdTaskCreat()
{
    OS_ERR  err;
    OSTaskCreate((OS_TCB    * )&AppTaskTCB,                           
                 (CPU_CHAR  * )"App Task SD",
                 (OS_TASK_PTR )AppMain,
                 (void      * )0u,
                 (OS_PRIO     )APP_CFG_TASK_PRIO,
                 (CPU_STK   * )&AppTaskStk[0u],
                 (CPU_STK_SIZE)APP_CFG_TASK_STK_SIZE / 10u,
                 (CPU_STK_SIZE)APP_CFG_TASK_STK_SIZE,
                 (OS_MSG_QTY  )0u,
                 (OS_TICK     )0u,
                 (void      * )0u,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    * )&err);
}

