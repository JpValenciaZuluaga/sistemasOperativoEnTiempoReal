/*
 * Scheduler.c
 *
 *  Created on: Nov 1, 2023
 *      Author: diego
 */


/**********************************************************************************************************
***********************************************************************************************************
*
* INCLUSIÓN DE LIBRERÍAS
*
* **********************************************************************************************************
* * **********************************************************************************************************/
#include "types.h"
#include "Scheduler.h"
#include "Rtos.h"
#include "Queue.h"
#include "Port.h"
/* ***********************************************************************************************************
 * ***********************************************************************************************************
 *
 * DEFINICIÓN DE CONSTANTES DEL MÓDULO
 *
 * ***********************************************************************************************************
 * ***********************************************************************************************************/


/* ***********************************************************************************************************
 * ***********************************************************************************************************
 *
 * DEFINICIÓN DE ERRORES DEL MÓDULO
 *
 * ***********************************************************************************************************
 * ***********************************************************************************************************/


/* ***********************************************************************************************************
 * ***********************************************************************************************************
 *
 * DEFINICIÓN DE TIPOS DE DATO DEL MÓDULO
 *
 * ***********************************************************************************************************
 * ***********************************************************************************************************/



/************************************************************************************************************
* ***********************************************************************************************************
*
* DEFINICIÓN DE FUNCIONES DEL MODULO
*
*************************************************************************************************************
************************************************************************************************************/
/**********************************************************************************************************
***********************************************************************************************************
*
* DEFINICIÓN DE CONSTANTES INTERNAS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/

/**********************************************************************************************************
***********************************************************************************************************
*
* DEFINICIÓN DE TIPOS DE DATOS INTERNOS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/

/**********************************************************************************************************
***********************************************************************************************************
*
* DEFINICIÓN DE FUNCIONES INTERNAS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/
//xtern UInt16 Task_Create(T_TaskTCB_Ptr TCB, UInt16 Id, const char *Name,UInt8 Priority, UInt32 *Stack, UInt32 StackSize,TaskCallback Function)
extern void Task_CheckTimeBlockedTask();

UInt16 Scheduler_CreateIdleTask();
//UInt16 Scheduler_SetStackOverflowTask(T_TaskTCB_Ptr Task);
void IdleTask();

/**********************************************************************************************************
***********************************************************************************************************
*
* DEFINICIÓN DE VARIABLES GLOBALES INTERNAS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/
T_QueueHandler ReadyTaskQueue [RTOS_SYSTEM_PRIORITIES];
T_TaskTCB_Ptr ActualTaskPtr = NULL;
T_TaskTCB IdleTaskHandler;
UInt32 IdleTaskStack[RTOS_MIN_STACK_SIZE];
T_QueueHandler StackOverflowTaskQueue;

/**********************************************************************************************************
***********************************************************************************************************
*  T_QueueHandler ReadyTaskQueue [RTOS_SYSTEM_PRIORITIES];
* IMPLEMENTACIÓN DE FUNCIONES EXTERNAS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/

UInt16 Scheduler_Init()
{
	UInt16 Res = SCHEDULER_OK;
	UInt8 i;
	ActualTaskPtr = NULL;
	Res = Scheduler_CreateIdleTask();
	if(Res)
	{
		return Res;
	}
	for (i = 0; i < RTOS_SYSTEM_PRIORITIES; i++)
	{
		Res = Queue_Init(&ReadyTaskQueue[i]);
		if (Res)
		{
			break;
		}
	}
	if (Res == SCHEDULER_OK )
	{
		Res = Queue_Init(&StackOverflowTaskQueue);
	}
	return Res;
}
UInt16 Scheduler_SetReadyTask (T_TaskTCB_Ptr Task)
{
	UInt16 Res = SCHEDULER_ERR_NULL_PARAM;
		if (Task != NULL)
		{
			if (Task -> TaskPriority > RTOS_MAX_TASK_PRIORITY)
			{
				Task -> TaskPriority = RTOS_MAX_TASK_PRIORITY;
			}
			Task -> TaskStatus = TASK_READY;
			Res = Queue_Enqueue(&ReadyTaskQueue[Task -> TaskPriority],&Task->QueueElement, Task);
			if (Res)
			{
				Task -> TaskStatus = TASK_BLOCKED;
			}
		}
		return Res;
}

UInt16 Scheduler_SetStackOverflowTask(T_TaskTCB_Ptr Task)
{
	UInt16 Res = SCHEDULER_ERR_NULL_PARAM;
	if (Task != NULL)
	{
		if (Task -> TaskPriority > RTOS_MAX_TASK_PRIORITY)
		{
			Task -> TaskPriority = RTOS_MAX_TASK_PRIORITY;
		}
		Task -> TaskStatus = TASK_BLOCKED;
		Res = Queue_Enqueue(&StackOverflowTaskQueue,&Task->QueueElement, Task);

	}
	return Res;
}
/*UInt16 Scheduler_SetReadyTask(T_TaskTCB_ Ptr Task)
{
	UInt16 Res = SCHEDULER_ERR_NULL_PARAM;
	if (Task != NULL)
	{
		if (Task -> TaskPriority > RTOS_MAX_TASK_PRIORITY)
		{
			Task -> TaskPriority = RTOS_MAX_TASK_PRIORITY;
		}
		Task -> TaskStatus = TASK_READY;
		Res = Queue_Enqueue(&ReadyTaskQueue[Task -> TaskPriority],&Task->QueueElement, Task);
		if (Res)
		{
			Task -> TaskStatus = TASK_BLOCKED;
		}
	}
	return Res;
}
*/
UInt32 Scheduler_GetNextTask(UInt32 ActualSp)
{
	UInt16 Priority;
	//T_TaskTCB_Ptr * Temp = NULL; Anterior
	T_TaskTCB_Ptr Temp = NULL;
	if (ActualSp)
	{
		if(ActualTaskPtr != NULL)
		{
			ActualTaskPtr -> TaskActualSP = ActualSp;
			if(Port_CheckStackOverflow(ActualTaskPtr -> TaskStack))
			{
				ActualTaskPtr -> TaskStackOverflow = TRUE;
			}
			if(ActualTaskPtr != &IdleTaskHandler)
			{
				if (ActualTaskPtr -> TaskStatus == TASK_RUNNING)
				{
					if(!ActualTaskPtr -> TaskStackOverflow)
					{
						Scheduler_SetReadyTask(ActualTaskPtr);
					}
					else
					{
						Scheduler_SetStackOverflowTask(ActualTaskPtr);
					}
				}
			}
			else
			{
				IdleTaskHandler.TaskStatus = TASK_READY;
			}
		}
	}
	ActualTaskPtr = NULL;
	for (Priority = 0; Priority < RTOS_SYSTEM_PRIORITIES; Priority ++)
	{
		if (Queue_GetElementsCount(&ReadyTaskQueue[Priority]) > 0)
		{
			//if (Queue_Dequeue(&ReadyTaskQueue[Priority],** &Temp) == QUEUE_OK)
			if (Queue_Dequeue(&ReadyTaskQueue[Priority],&Temp) == QUEUE_OK)
			{
				ActualTaskPtr = Temp;
				break;
			}
		}
	}
	if (ActualTaskPtr == NULL)
	{
		ActualTaskPtr = &IdleTaskHandler;
	}
	ActualTaskPtr -> TaskStatus = TASK_RUNNING;
	return ActualTaskPtr -> TaskActualSP;



}
void Scheduler_Start()
{
	//UInt16 Res = SHEDULER_OK;
	UInt32 FirstSp = Scheduler_GetNextTask(0);
	Port_InitTickTimer();
	Port_RestoreFirstContext(FirstSp);

}
UInt16 Scheduler_CreateIdleTask()
{
	IdleTaskHandler.TaskId = RTOS_IDLE_TASK_ID;
	IdleTaskHandler.TaskName = RTOS_IDLE_TASK_NAME;
	IdleTaskHandler.TaskPriority = RTOS_SYSTEM_PRIORITIES;
	IdleTaskHandler.TaskStackOverflow = FALSE;
	IdleTaskHandler.TaskStack = (UInt32 * ) IdleTaskStack;
	IdleTaskHandler.TaskStackSize = RTOS_MIN_STACK_SIZE;
	IdleTaskHandler.TaskFunction = IdleTask;
	IdleTaskHandler.TaskStatus = TASK_READY;
	IdleTaskHandler.TaskActualSP = (unsigned int)Port_InitTaskStack(IdleTaskHandler.TaskStack, IdleTaskHandler.TaskStackSize, IdleTaskHandler.TaskFunction);
	IdleTaskHandler.QueueElement.DataPtr = &IdleTaskHandler;
	IdleTaskHandler.QueueElement.Next = NULL;
	return SCHEDULER_OK;
}



T_TaskTCB_Ptr Scheduler_GetActualTask()
{
	return ActualTaskPtr;
}
void IdleTask()
{
	for (; ; )
	{
		Task_CheckTimeBlockedTask();
		//GPIOC_BSRR |= GPIOC_BSRR_BS13;
		//Task_Sleep(500);
		//GPIOC_BSRR |= GPIOC_BSRR_BR13;
		//Task_Sleep(500);
	}
}
