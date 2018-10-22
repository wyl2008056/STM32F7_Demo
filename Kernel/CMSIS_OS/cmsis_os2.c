
 
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "cmsis_os2.h"

#include <os.h>
#include <cpu.h>
#include <lib_math.h>
#include <lib_mem.h>
#include <lib_str.h>
#include <os_app_hooks.h>
#include <app_cfg.h>
#include "GUI.h"



#ifdef  __cplusplus
extern "C"
{
#endif



osStatus_t errToStatus(OS_ERR err)
{
    if(OS_ERR_NONE != err)
    {
        return osError;
    }
    return osOK;
}


//  ==== Kernel Management Functions ====
 
/// Initialize the RTOS Kernel.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelInitialize (void)
{
    OS_ERR err;
    APP_TRACE_DBG(("Start to Initialize the kernel.\n"));

    APP_TRACE_DBG(("Initialize the uC/CPU services\n"));
    CPU_Init(); 

    APP_TRACE_DBG(("Initialize Memory Managment Module\n"));
    Mem_Init(); 
    
    APP_TRACE_DBG(("Initialize Mathematical Module\n"));
    Math_Init(); 
    
    APP_TRACE_DBG(("Disable all Interrupts.\n"));
    CPU_IntDis(); 
    
    APP_TRACE_DBG(("Init uC/OS-III.\n"));
    OSInit(&err);  

    APP_TRACE_DBG(("Set os hooks.\n"));
    App_OS_SetAllHooks();

#if OS_CFG_STAT_TASK_EN > 0u
    APP_TRACE_DBG(("init Compute CPU capacity with no task running \n"));   
    OSStatTaskCPUUsageInit(&err);     
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    APP_TRACE_DBG(("Creating Application kernel objects\n"));
    AppObjCreate();                                          

    APP_TRACE_DBG(("Creating Application Tasks\n"));
    AppTaskCreate();                                         
    
    APP_TRACE_DBG(("Kernel Initialize success.\n"));
    return errToStatus(err);
}
 
///  Get RTOS Kernel Information.
/// \param[out]    version       pointer to buffer for retrieving version information.
/// \param[out]    id_buf        pointer to buffer for retrieving kernel identification string.
/// \param[in]     id_size       size of buffer for kernel identification string.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelGetInfo (osVersion_t *version, char *id_buf, uint32_t id_size)
{
    version->api = osCMSIS;
    version->kernel = osCMSIS_KERNEL;
    Str_Copy_N(id_buf, osKernelSystemId, id_size);
    return osOK;
}	
/// Get the current RTOS Kernel state.
/// \return current RTOS Kernel state.
osKernelState_t osKernelGetState (void)
{
    osKernelState_t state = osKernelInactive;
    if(OS_STATE_OS_RUNNING == OSRunning)
    {
        state = osKernelRunning;
    }
    return state;
}
 
/// Start the RTOS Kernel scheduler.
/// \return status code that indicates the execution status of the function.
osStatus_t osKernelStart (void)
{
    OS_ERR err;
    OSStart(&err);  
    if(OS_ERR_NONE != err)
    {
        return osKernelError;
    }
    return osKernelInactive;
}
 
int32_t lastLockState = 0;
/// Lock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelLock (void)
{
    OS_ERR err;
    int32_t ret = lastLockState;
    OSSchedLock(&err);
    lastLockState = 1;
    if(OS_ERR_NONE != err)
    {
        return err;
    }
    return ret;
}
 
/// Unlock the RTOS Kernel scheduler.
/// \return previous lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelUnlock (void)
{
    OS_ERR err;
    int32_t ret = lastLockState;
    OSSchedUnlock(&err);
    lastLockState = 0;
    if(OS_ERR_NONE != err)
    {
        return err;
    }
    return ret;
}
 
/// Restore the RTOS Kernel scheduler lock state.
/// \param[in]     lock          lock state obtained by \ref osKernelLock or \ref osKernelUnlock.
/// \return new lock state (1 - locked, 0 - not locked, error code if negative).
int32_t osKernelRestoreLock (int32_t lock)
{
    if(1 == lock)
    {
        return osKernelUnlock();
    }
    else
    {
        return osKernelLock();
    }
}
 
/// Suspend the RTOS Kernel scheduler.
/// \return time in ticks, for how long the system can sleep or power-down.
uint32_t osKernelSuspend (void)
{
    OS_ERR err;
    return OSTimeGet(&err);
}
 
/// Resume the RTOS Kernel scheduler.
/// \param[in]     sleep_ticks   time in ticks for how long the system was in sleep or power-down mode.
void osKernelResume (uint32_t sleep_ticks)
{
    
}
 
/// Get the RTOS kernel tick count.
/// \return RTOS kernel current tick count.
uint32_t osKernelGetTickCount (void)
{
    return OSTickCtr;
}
 
/// Get the RTOS kernel tick frequency.
/// \return frequency of the kernel tick in hertz, i.e. kernel ticks per second.
uint32_t osKernelGetTickFreq (void)
{
    return OS_CFG_TICK_RATE_HZ;
}
 
/// Get the RTOS kernel system timer count.
/// \return RTOS kernel current system timer count as 32-bit value.
uint32_t osKernelGetSysTimerCount (void)
{
    OS_ERR err; 
    return OSTimeGet(&err);
}
 
/// Get the RTOS kernel system timer frequency.
/// \return frequency of the system timer in hertz, i.e. timer ticks per second.
uint32_t osKernelGetSysTimerFreq (void)
{
    return OS_CFG_TICK_RATE_HZ;
}
 
 


//  ==== Thread Management Functions ====
 
/// Create a thread and add it to Active Threads.
/// \param[in]     func          thread function.
/// \param[in]     argument      pointer that is passed to the thread function as start argument.
/// \param[in]     attr          thread attributes; NULL: default values.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
    OS_ERR err; 
    OSTaskCreate((OS_TCB    * )attr->cb_mem,                           
                 (CPU_CHAR  * )attr->name,
                 (OS_TASK_PTR )func,
                 (void      * )argument,
                 (OS_PRIO     )attr->priority,
                 (CPU_STK   * )attr->stack_mem,
                 (CPU_STK_SIZE)attr->stack_size / 10u,
                 (CPU_STK_SIZE)attr->stack_size,
                 (OS_MSG_QTY  )0u,
                 (OS_TICK     )0u,
                 (void      * )0u,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    * )&err);
    return (osThreadId_t)attr->cb_mem;
}


osThreadId_t osThreadNewEx (osThreadFunc_t func, void *argument, const char *name, osPriority_t priority, uint32_t stack_size)
{
    OS_ERR err; 
    OS_TCB *p_tcb = (OS_TCB *)malloc(sizeof(OS_TCB));
    if(NULL == p_tcb)
    {
        printf("error to create task tcb\n");
    }
    CPU_STK *p_stk = (CPU_STK *)malloc(sizeof(CPU_STK) * stack_size);
    if(NULL == p_stk)
    {
        printf("error to create task stk\n");
    }
    OSTaskCreate((OS_TCB    * )p_tcb,                           
                 (CPU_CHAR  * )name,
                 (OS_TASK_PTR )func,
                 (void      * )argument,
                 (OS_PRIO     )priority,
                 (CPU_STK   * )p_stk,
                 (CPU_STK_SIZE)stack_size / 10u,
                 (CPU_STK_SIZE)stack_size,
                 (OS_MSG_QTY  )0u,
                 (OS_TICK     )0u,
                 (void      * )0u,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR    * )&err);
    return (osThreadId_t)p_tcb;
}
 
/// Get name of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return name as NULL terminated string.
const char *osThreadGetName (osThreadId_t thread_id)
{
    OS_TCB *p_tcb = (OS_TCB *)thread_id;
    return p_tcb->NamePtr;
}
 
/// Return the thread ID of the current running thread.
/// \return thread ID for reference by other functions or NULL in case of error.
osThreadId_t osThreadGetId (void)
{
    return (osThreadId_t)OSTCBCurPtr;
}
 
/// Get current thread state of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current thread state of the specified thread.
osThreadState_t osThreadGetState (osThreadId_t thread_id)
{
    OS_TCB *p_tcb = (OS_TCB *)thread_id;
    switch (p_tcb->TaskState) {
        case OS_TASK_STATE_RDY:
            return osThreadReady;

        case OS_TASK_STATE_SUSPENDED:
            return osThreadBlocked;

        case OS_TASK_STATE_DLY:                                 /* Task is only delayed, not on any wait list           */
        case OS_TASK_STATE_DLY_SUSPENDED:
        case OS_TASK_STATE_PEND:
        case OS_TASK_STATE_PEND_SUSPENDED:
        case OS_TASK_STATE_PEND_TIMEOUT:
        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
            return osThreadRunning;

        case OS_TASK_STATE_DEL:
            return osThreadTerminated;

        default:
            return osThreadError;
    }
    return osThreadInactive;
}
 
/// Get stack size of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return stack size in bytes.
uint32_t osThreadGetStackSize (osThreadId_t thread_id)
{
    OS_TCB *p_tcb = (OS_TCB *)thread_id;
    return p_tcb->StkSize;
}
 
/// Get available stack space of a thread based on stack watermark recording during execution.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return remaining stack space in bytes.
uint32_t osThreadGetStackSpace (osThreadId_t thread_id)
{
    OS_TCB *p_tcb = (OS_TCB *)thread_id;
    return p_tcb->StkFree;
}
 
/// Change priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     priority      new priority value for the thread function.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSetPriority (osThreadId_t thread_id, osPriority_t priority)
{
    OS_ERR err; 
    OS_TCB *p_tcb = (OS_TCB *)thread_id;
    OSTaskChangePrio(p_tcb, priority, &err);
    return errToStatus(err);
}
 
/// Get current priority of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return current priority value of the specified thread.
osPriority_t osThreadGetPriority (osThreadId_t thread_id)
{
    OS_TCB *p_tcb = (OS_TCB *)thread_id;
    return (osPriority_t)p_tcb->Prio;
}
 
/// Pass control to next thread that is in state \b READY.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadYield (void)
{
    OSSched();
    return osOK;
}
 
/// Suspend execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadSuspend (osThreadId_t thread_id)
{
    OS_ERR err; 
    OS_TCB *p_tcb = (OS_TCB *)thread_id;
    OSTaskSuspend(p_tcb, &err);
    return errToStatus(err);
}
 
/// Resume execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadResume (osThreadId_t thread_id)
{
    OS_ERR err; 
    OS_TCB *p_tcb = (OS_TCB *)thread_id;
    OSTaskResume(p_tcb, &err);
    return errToStatus(err);
}
 
/// Detach a thread (thread storage can be reclaimed when thread terminates).
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadDetach (osThreadId_t thread_id)
{
    
}
 
/// Wait for specified thread to terminate.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadJoin (osThreadId_t thread_id)
{
    
}
 
/// Terminate execution of current running thread.
__NO_RETURN void osThreadExit (void)
{
    abort();
}
 
/// Terminate execution of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \return status code that indicates the execution status of the function.
osStatus_t osThreadTerminate (osThreadId_t thread_id)
{
#if (OS_CFG_TASK_DEL_EN == DEF_ENABLED)
    OS_ERR err; 
    OS_TCB *p_tcb = (OS_TCB *)thread_id;
    OSTaskDel(p_tcb, &err);
    return errToStatus(err);
#endif
}
 
/// Get number of active threads.
/// \return number of active threads.
uint32_t osThreadGetCount (void)
{
    OS_TCB  *p_tcb;
    uint32_t  count;
    
    CPU_SR_ALLOC();
    
    CPU_CRITICAL_ENTER();
    p_tcb = OSTaskDbgListPtr;
    CPU_CRITICAL_EXIT();

    while(p_tcb != (OS_TCB *) 0)
    {
        count++;
        CPU_CRITICAL_ENTER();
        p_tcb = p_tcb -> DbgNextPtr; //指向下一个任务的TCB
        CPU_CRITICAL_EXIT();
    }
    return count;
}
 
/// Enumerate active threads.
/// \param[out]    thread_array  pointer to array for retrieving thread IDs.
/// \param[in]     array_items   maximum number of items in array for retrieving thread IDs.
/// \return number of enumerated threads.
uint32_t osThreadEnumerate (osThreadId_t *thread_array, uint32_t array_items)
{
    thread_array = (osThreadId_t *)OSTaskDbgListPtr;
    return osThreadGetCount();
}
 
 
//  ==== Thread Flags Functions ====
 
/// Set the specified Thread Flags of a thread.
/// \param[in]     thread_id     thread ID obtained by \ref osThreadNew or \ref osThreadGetId.
/// \param[in]     flags         specifies the flags of the thread that shall be set.
/// \return thread flags after setting or error code if highest bit set.
uint32_t osThreadFlagsSet (osThreadId_t thread_id, uint32_t flags)
{

}
 
/// Clear the specified Thread Flags of current running thread.
/// \param[in]     flags         specifies the flags of the thread that shall be cleared.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsClear (uint32_t flags)
{

}
 
/// Get the current Thread Flags of current running thread.
/// \return current thread flags.
uint32_t osThreadFlagsGet (void)
{

}
 
/// Wait for one or more Thread Flags of the current running thread to become signaled.
/// \param[in]     flags         specifies the flags to wait for.
/// \param[in]     options       specifies flags options (osFlagsXxxx).
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return thread flags before clearing or error code if highest bit set.
uint32_t osThreadFlagsWait (uint32_t flags, uint32_t options, uint32_t timeout)
{

}
 
 
//  ==== Generic Wait Functions ====
 
/// Wait for Timeout (Time Delay).
/// \param[in]     ticks         \ref CMSIS_RTOS_TimeOutValue "time ticks" value
/// \return status code that indicates the execution status of the function.
osStatus_t osDelay (uint32_t ticks)
{
    OS_ERR err; 
    OSTimeDly(ticks, OS_OPT_TIME_DLY, &err);
    return errToStatus(err);
}
 
/// Wait until specified time.
/// \param[in]     ticks         absolute time in ticks
/// \return status code that indicates the execution status of the function.
osStatus_t osDelayUntil (uint32_t ticks)
{
    OS_ERR err; 
    OS_TICK curTicks = OSTimeGet(&err);
    OSTimeDly(ticks - curTicks, OS_OPT_TIME_DLY, &err);
    return errToStatus(err);
}
 
 
//  ==== Timer Management Functions ====
 
/// Create and Initialize a timer.
/// \param[in]     func          function pointer to callback function.
/// \param[in]     type          \ref osTimerOnce for one-shot or \ref osTimerPeriodic for periodic behavior.
/// \param[in]     argument      argument to the timer callback function.
/// \param[in]     attr          timer attributes; NULL: default values.
/// \return timer ID for reference by other functions or NULL in case of error.
osTimerId_t osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
{
    OS_ERR err; 
    OS_OPT opt = OS_OPT_TMR_ONE_SHOT;
    if(type == osTimerPeriodic){
        opt = OS_OPT_TMR_PERIODIC;
    }
    OSTmrCreate((OS_TMR            *) attr->cb_mem,
                (CPU_CHAR          *) attr->name,
                (OS_TICK            ) attr->dly,
                (OS_TICK            ) attr->period,
                (OS_OPT             ) opt,
                (OS_TMR_CALLBACK_PTR) func,
                (void              *) argument,
                (OS_ERR            *) &err);
    
    return (osTimerId_t)(attr->cb_mem);
}

osTimerId_t osTimerNewEx (osTimerFunc_t func, osTimerType_t type, void *argument, const char *name, uint32_t dly, uint32_t period)
{
    OS_ERR err; 
    OS_OPT opt = OS_OPT_TMR_ONE_SHOT;
    if(type == osTimerPeriodic){
        opt = OS_OPT_TMR_PERIODIC;
    }
    OS_TMR *p_tmr = (OS_TMR *) malloc(sizeof(OS_TMR));
    
    OSTmrCreate((OS_TMR            *) p_tmr,
                (CPU_CHAR          *) name,
                (OS_TICK            ) dly,
                (OS_TICK            ) period,
                (OS_OPT             ) opt,
                (OS_TMR_CALLBACK_PTR) func,
                (void              *) argument,
                (OS_ERR            *) &err);
    
    return (osTimerId_t)(p_tmr);
}
 
/// Get name of a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return name as NULL terminated string.
const char *osTimerGetName (osTimerId_t timer_id)
{
    OS_TMR *p_tmr = (OS_TMR *)timer_id;
    return (const char *)(p_tmr->NamePtr);
}
 
/// Start or restart a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \param[in]     ticks         \ref CMSIS_RTOS_TimeOutValue "time ticks" value of the timer.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerStart (osTimerId_t timer_id, uint32_t ticks)
{
    OS_ERR err; 
    OS_TMR *p_tmr = (OS_TMR *)timer_id;
    OSTmrStart(p_tmr, &err);
    return errToStatus(err);
}
 
/// Stop a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerStop (osTimerId_t timer_id)
{
    OS_ERR err; 
    OS_TMR *p_tmr = (OS_TMR *)timer_id;
    OSTmrStop(p_tmr, OS_OPT_TMR_NONE, NULL, &err);
    return errToStatus(err);
}
 
/// Check if a timer is running.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return 0 not running, 1 running.
uint32_t osTimerIsRunning (osTimerId_t timer_id)
{
    OS_ERR err; 
    OS_TMR *p_tmr = (OS_TMR *)timer_id;
    OS_STATE state = OSTmrStateGet(p_tmr, &err);
    if(OS_TMR_STATE_RUNNING == state)
    {
        return 1;
    }
    return 0;
}
 
/// Delete a timer.
/// \param[in]     timer_id      timer ID obtained by \ref osTimerNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osTimerDelete (osTimerId_t timer_id)
{
    OS_ERR err; 
    OS_TMR *p_tmr = (OS_TMR *)timer_id;
    OSTmrDel(p_tmr, &err);
    return errToStatus(err);
}
 
 
//  ==== Event Flags Management Functions ====
 
/// Create and Initialize an Event Flags object.
/// \param[in]     attr          event flags attributes; NULL: default values.
/// \return event flags ID for reference by other functions or NULL in case of error.
osEventFlagsId_t osEventFlagsNew (const osEventFlagsAttr_t *attr)
{
    OS_ERR err; 
    OSFlagCreate((OS_FLAG_GRP *) attr->cb_mem,
                 (CPU_CHAR    *) attr->name,
                 (OS_FLAGS     ) 0,
                 (OS_ERR      *) &err);
    return (osEventFlagsId_t)attr->cb_mem;
}

osEventFlagsId_t osEventFlagsNewEx (const char *name, uint32_t flags)
{
    OS_ERR err; 
    OS_FLAG_GRP *p_flag = (OS_FLAG_GRP *)malloc(sizeof(OS_FLAG_GRP));
    OSFlagCreate((OS_FLAG_GRP *) p_flag,
                 (CPU_CHAR    *) name,
                 (OS_FLAGS     ) flags,
                 (OS_ERR      *) &err);
    return (osEventFlagsId_t)p_flag;
}
 
/// Get name of an Event Flags object.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return name as NULL terminated string.
const char *osEventFlagsGetName (osEventFlagsId_t ef_id)
{
    OS_FLAG_GRP *p_flag = (OS_FLAG_GRP *)ef_id;
    return p_flag->NamePtr;
}
 
/// Set the specified Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags that shall be set.
/// \return event flags after setting or error code if highest bit set.
uint32_t osEventFlagsSet (osEventFlagsId_t ef_id, uint32_t flags)
{

}
 
/// Clear the specified Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags that shall be cleared.
/// \return event flags before clearing or error code if highest bit set.
uint32_t osEventFlagsClear (osEventFlagsId_t ef_id, uint32_t flags)
{

}
 
/// Get the current Event Flags.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return current event flags.
uint32_t osEventFlagsGet (osEventFlagsId_t ef_id)
{

}
 
/// Wait for one or more Event Flags to become signaled.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \param[in]     flags         specifies the flags to wait for.
/// \param[in]     options       specifies flags options (osFlagsXxxx).
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return event flags before clearing or error code if highest bit set.
uint32_t osEventFlagsWait (osEventFlagsId_t ef_id, uint32_t flags, uint32_t options, uint32_t timeout)
{

}
 
/// Delete an Event Flags object.
/// \param[in]     ef_id         event flags ID obtained by \ref osEventFlagsNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osEventFlagsDelete (osEventFlagsId_t ef_id)
{

}
 
 
//  ==== Mutex Management Functions ====
 
/// Create and Initialize a Mutex object.
/// \param[in]     attr          mutex attributes; NULL: default values.
/// \return mutex ID for reference by other functions or NULL in case of error.
osMutexId_t osMutexNew (const osMutexAttr_t *attr)
{

}

osMutexId_t osMutexNewEx (const char *name)
{
    OS_ERR err; 
    OS_MUTEX *p_mutex = (OS_MUTEX *)malloc(sizeof(OS_MUTEX));
    OSMutexCreate((OS_MUTEX *) p_mutex,
                  (CPU_CHAR *) name,
                  (OS_ERR   *) &err);
    return (osMutexId_t)p_mutex;
}
 
/// Get name of a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return name as NULL terminated string.
const char *osMutexGetName (osMutexId_t mutex_id)
{
    OS_MUTEX *p_mutex = (OS_MUTEX *)mutex_id;
    return p_mutex->NamePtr;
}
 
/// Acquire a Mutex or timeout if it is locked.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexAcquire (osMutexId_t mutex_id, uint32_t timeout)
{
    OS_ERR err; 
    OS_MUTEX *p_mutex = (OS_MUTEX *)mutex_id;
    OSMutexPend(p_mutex, timeout, OS_OPT_PEND_BLOCKING, NULL, &err);
    return errToStatus(err);
}
 
/// Release a Mutex that was acquired by \ref osMutexAcquire.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexRelease (osMutexId_t mutex_id)
{
    OS_ERR err; 
    OS_MUTEX *p_mutex = (OS_MUTEX *)mutex_id;
    OSMutexPost(p_mutex, OS_OPT_POST_NONE, &err);
    return errToStatus(err);
}
 
/// Get Thread which owns a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return thread ID of owner thread or NULL when mutex was not acquired.
osThreadId_t osMutexGetOwner (osMutexId_t mutex_id)
{

}
 
/// Delete a Mutex object.
/// \param[in]     mutex_id      mutex ID obtained by \ref osMutexNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMutexDelete (osMutexId_t mutex_id)
{
#if (OS_CFG_MUTEX_DEL_EN == DEF_ENABLED)
    OS_ERR err; 
    OS_MUTEX *p_mutex = (OS_MUTEX *)mutex_id;
    OSMutexDel(p_mutex, OS_OPT_DEL_ALWAYS, &err);
    return errToStatus(err);
#endif
}
 
 
//  ==== Semaphore Management Functions ====
 
/// Create and Initialize a Semaphore object.
/// \param[in]     max_count     maximum number of available tokens.
/// \param[in]     initial_count initial number of available tokens.
/// \param[in]     attr          semaphore attributes; NULL: default values.
/// \return semaphore ID for reference by other functions or NULL in case of error.
osSemaphoreId_t osSemaphoreNew (uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t *attr)
{

}

osSemaphoreId_t osSemaphoreNewEx (uint32_t initial_count, const char *name)
{
    OS_ERR err; 
    OS_SEM *p_sem = (OS_SEM *)malloc(sizeof(OS_SEM));
    OSSemCreate((OS_SEM   *) p_sem,
                (CPU_CHAR *) name,
                (OS_SEM_CTR) initial_count,
                (OS_ERR   *) &err);
    return (osSemaphoreId_t)p_sem;
}
 
/// Get name of a Semaphore object.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return name as NULL terminated string.
const char *osSemaphoreGetName (osSemaphoreId_t semaphore_id)
{
    OS_SEM *p_sem = (OS_SEM *)semaphore_id;
    return p_sem->NamePtr;
}
 
/// Acquire a Semaphore token or timeout if no tokens are available.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreAcquire (osSemaphoreId_t semaphore_id, uint32_t timeout)
{
    OS_ERR err; 
    OS_SEM *p_sem = (OS_SEM *)semaphore_id;
    OSSemPend(p_sem, timeout, OS_OPT_PEND_BLOCKING, NULL, &err);
    return errToStatus(err);
}
 
/// Release a Semaphore token up to the initial maximum count.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreRelease (osSemaphoreId_t semaphore_id)
{
    OS_ERR err; 
    OS_SEM *p_sem = (OS_SEM *)semaphore_id;
    OSSemPost(p_sem, OS_OPT_POST_ALL, &err);
    return errToStatus(err);
}
 
/// Get current Semaphore token count.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return number of tokens available.
uint32_t osSemaphoreGetCount (osSemaphoreId_t semaphore_id)
{
    OS_SEM *p_sem = (OS_SEM *)semaphore_id;
    return p_sem->Ctr;
}
 
/// Delete a Semaphore object.
/// \param[in]     semaphore_id  semaphore ID obtained by \ref osSemaphoreNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osSemaphoreDelete (osSemaphoreId_t semaphore_id)
{
#if (OS_CFG_SEM_DEL_EN == DEF_ENABLED)
    OS_ERR err; 
    OS_SEM *p_sem = (OS_SEM *)semaphore_id;
    OSSemDel(p_sem, OS_OPT_DEL_ALWAYS, &err);
    return errToStatus(err);
#endif
}
 
 
//  ==== Memory Pool Management Functions ====
 
/// Create and Initialize a Memory Pool object.
/// \param[in]     block_count   maximum number of memory blocks in memory pool.
/// \param[in]     block_size    memory block size in bytes.
/// \param[in]     attr          memory pool attributes; NULL: default values.
/// \return memory pool ID for reference by other functions or NULL in case of error.
osMemoryPoolId_t osMemoryPoolNew (uint32_t block_count, uint32_t block_size, const osMemoryPoolAttr_t *attr)
{

}
 
/// Get name of a Memory Pool object.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return name as NULL terminated string.
const char *osMemoryPoolGetName (osMemoryPoolId_t mp_id)
{

}
 
/// Allocate a memory block from a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return address of the allocated memory block or NULL in case of no memory is available.
void *osMemoryPoolAlloc (osMemoryPoolId_t mp_id, uint32_t timeout)
{

}
 
/// Return an allocated memory block back to a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \param[in]     block         address of the allocated memory block to be returned to the memory pool.
/// \return status code that indicates the execution status of the function.
osStatus_t osMemoryPoolFree (osMemoryPoolId_t mp_id, void *block)
{

}
 
/// Get maximum number of memory blocks in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return maximum number of memory blocks.
uint32_t osMemoryPoolGetCapacity (osMemoryPoolId_t mp_id)
{

}
 
/// Get memory block size in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return memory block size in bytes.
uint32_t osMemoryPoolGetBlockSize (osMemoryPoolId_t mp_id)
{

}
 
/// Get number of memory blocks used in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return number of memory blocks used.
uint32_t osMemoryPoolGetCount (osMemoryPoolId_t mp_id)
{

}
 
/// Get number of memory blocks available in a Memory Pool.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return number of memory blocks available.
uint32_t osMemoryPoolGetSpace (osMemoryPoolId_t mp_id)
{

}
 
/// Delete a Memory Pool object.
/// \param[in]     mp_id         memory pool ID obtained by \ref osMemoryPoolNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMemoryPoolDelete (osMemoryPoolId_t mp_id)
{

}
 
 
//  ==== Message Queue Management Functions ====
 
/// Create and Initialize a Message Queue object.
/// \param[in]     msg_count     maximum number of messages in queue.
/// \param[in]     msg_size      maximum message size in bytes.
/// \param[in]     attr          message queue attributes; NULL: default values.
/// \return message queue ID for reference by other functions or NULL in case of error.
osMessageQueueId_t osMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr)
{

}
 
/// Get name of a Message Queue object.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return name as NULL terminated string.
const char *osMessageQueueGetName (osMessageQueueId_t mq_id)
{

}
 
/// Put a Message into a Queue or timeout if Queue is full.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \param[in]     msg_ptr       pointer to buffer with message to put into a queue.
/// \param[in]     msg_prio      message priority.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{

}
 
/// Get a Message from a Queue or timeout if Queue is empty.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \param[out]    msg_ptr       pointer to buffer for message to get from a queue.
/// \param[out]    msg_prio      pointer to buffer for message priority or NULL.
/// \param[in]     timeout       \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{

}
 
/// Get maximum number of messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return maximum number of messages.
uint32_t osMessageQueueGetCapacity (osMessageQueueId_t mq_id)
{

}
 
/// Get maximum message size in a Memory Pool.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return maximum message size in bytes.
uint32_t osMessageQueueGetMsgSize (osMessageQueueId_t mq_id)
{

}
 
/// Get number of queued messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return number of queued messages.
uint32_t osMessageQueueGetCount (osMessageQueueId_t mq_id)
{

}
 
/// Get number of available slots for messages in a Message Queue.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return number of available slots for messages.
uint32_t osMessageQueueGetSpace (osMessageQueueId_t mq_id)
{

}
 
/// Reset a Message Queue to initial empty state.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueReset (osMessageQueueId_t mq_id)
{

}
 
/// Delete a Message Queue object.
/// \param[in]     mq_id         message queue ID obtained by \ref osMessageQueueNew.
/// \return status code that indicates the execution status of the function.
osStatus_t osMessageQueueDelete (osMessageQueueId_t mq_id)
{

}
 
void LogI(const char * msg)
{
    uint32_t time = osKernelGetTickCount();
    int sec, min, hour,ms;
    int seconds = time/1000;
    ms = time % 1000;
    hour = seconds/3600;
    min = (seconds % 3600)/60;
    sec = seconds % 60;
    
    printf("[%02d:%02d:%02d.%03d] I %s\n", hour, min, sec, ms, msg);
    
}

void LogD(const char * msg)
{
    uint32_t time = osKernelGetTickCount();
    int sec, min, hour,ms;
    int seconds = time/1000;
    ms = time % 1000;
    hour = seconds/3600;
    min = (seconds % 3600)/60;
    sec = seconds % 60;
    
    printf("[%02d:%02d:%02d.%03d] D %s\n", hour, min, sec, ms, msg);
}
 
#ifdef  __cplusplus
}
#endif
