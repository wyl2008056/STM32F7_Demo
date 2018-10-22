/*
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include  <os.h>
#include  <stdlib.h>
#include  <stdio.h>
#include  <app_state.h>
//#include  "lcd_log.h"



/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
#define  APP_CFG_TASK_PRIO                  9u
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

static  void  DispTaskInfo()
{
	OS_TCB  *p_tcb;
    OS_ERR  err;
    
    CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
    p_tcb = OSTaskDbgListPtr;
    CPU_CRITICAL_EXIT();

    printf("TaskName                cpu StkFree StkUsed\n");
    
    while(p_tcb != (OS_TCB *) 0)
    {
        
        printf("%-21s  %3d%%     %3d     %3d\n", 
                p_tcb -> NamePtr, 
                p_tcb -> CPUUsage/100,
                p_tcb -> StkFree, 
                p_tcb -> StkUsed);

        CPU_CRITICAL_ENTER();
        p_tcb = p_tcb -> DbgNextPtr; //指向下一个任务的TCB
        CPU_CRITICAL_EXIT();
    }
}


static void  AppMain (void *p_arg)
{
    OS_ERR  err;
    (void)p_arg;

    while (DEF_TRUE) { 
        //LCD_LOG_ClearTextZone();
        DispTaskInfo();
        OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

void AppStateTaskCreat()
{
    OS_ERR  err;
    OSTaskCreate((OS_TCB    * )&AppTaskTCB,                           
                 (CPU_CHAR  * )"App Task State",
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

