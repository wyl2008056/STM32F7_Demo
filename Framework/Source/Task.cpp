/*
*********************************************************************************************************
*                                               MODULE
*********************************************************************************************************
*/
#include "os.h"
#include "task.h"

namespace OS
{
	class Task
	{
		#define 	AppStkSize		 256u
		public:
			OS_TCB    	    AppTaskTCB;
			CPU_CHAR 	   *AppTaskName;
			OS_PRIO   		AppTaskPrio;
			
			CPU_STK  	    AppTaskStk[AppStkSize];

			static void AppMian(void *p_args)
			{

			}

			void CreateTask(void *p_args)
			{
				OS_ERR  err;
				OSTaskCreate((OS_TCB    * )&AppTaskTCB,                           
							 (CPU_CHAR  * )AppTaskName,
							 (OS_TASK_PTR )AppMian,
							 (void      * )p_args,
							 (OS_PRIO     )AppTaskPrio,
							 (CPU_STK   * )&AppTaskStk[0u],
							 (CPU_STK_SIZE)AppStkSize / 10u,
							 (CPU_STK_SIZE)AppStkSize,
							 (OS_MSG_QTY  )0u,
							 (OS_TICK     )0u,
							 (void      * )0u,
							 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							 (OS_ERR    * )&err);
			}

			void CreateTask()
			{
				CreateTask(0u);
			}
	};
}