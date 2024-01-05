/*
 * Task.c
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
#include "Port.h"
#include "Task.h"
#include "Rtos.h"
#include "Scheduler.h"

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

void Task_CheckTimeBlockedTask();
/**********************************************************************************************************
***********************************************************************************************************
*
* DEFINICIÓN DE VARIABLES GLOBALES INTERNAS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/

T_QueueHandler TaskTimeBlocked;

/**********************************************************************************************************
***********************************************************************************************************
*
* IMPLEMENTACIÓN DE FUNCIONES EXTERNAS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/












/******************************************************************************************************************/
UInt16 Task_Init()
{
	return Queue_Init(&TaskTimeBlocked);
}


UInt16 Task_Create(T_TaskTCB_Ptr TCB, UInt16 Id, const char *Name,UInt8 Priority, UInt32 *Stack, UInt32 StackSize,TaskCallback Function)
{
	UInt16 Res = TASK_ERR_NULL_PARAM;
	if ((TCB != NULL) && (Stack != NULL) && (Function != NULL))// && ) PILAAAAAAAAAAAAAAASSSSSSSSSSSSSSS
	{
		Res = TASK_ERR_PARAM_OUT_RANGE;
		vPortEnterCritical();
		if (StackSize >= RTOS_MIN_STACK_SIZE)
		{
			TCB -> TaskId = Id;
			TCB -> TaskName = Name;
			if (Priority > RTOS_MAX_TASK_PRIORITY)
			{
				TCB -> TaskPriority = RTOS_MAX_TASK_PRIORITY;
			}
			else
			{
				TCB -> TaskPriority = Priority;
			}
			TCB -> TaskStackOverflow = FALSE;
			TCB -> TaskPriority = Priority;
			TCB -> TaskStack = Stack;
			TCB -> TaskStackSize = StackSize;
			TCB -> TaskFunction = Function;
			TCB -> TaskStatus = TASK_READY;
			TCB -> TaskActualSP = (unsigned int) Port_InitTaskStack(TCB -> TaskStack, TCB -> TaskStackSize, TCB -> TaskFunction);
			TCB -> QueueElement.DataPtr = TCB;
			TCB -> QueueElement.Next = NULL;
			TCB -> TaskSleepTime = 0;
			Res = Scheduler_SetReadyTask(TCB);
		}
		vPortExitCritical();
	}
	return Res;
}

UInt16 Task_Sleep (UInt32 SleepTimeTicks)
{
	UInt8 CallScheduler = FALSE;
	UInt16 Res = TASK_ERR_PARAM_OUT_RANGE;
	if (SleepTimeTicks)
	{
		vPortEnterCritical();
		Res = TASK_ERR_NULL_PARAM;
		T_TaskTCB_Ptr TaskPtr = Scheduler_GetActualTask();
		if (TaskPtr != NULL)
		{
			if((TaskPtr -> TaskId != RTOS_IDLE_TASK_ID) && (TaskPtr -> TaskPriority < RTOS_SYSTEM_PRIORITIES))
			{
				if(!Port_CheckStackOverflow(TaskPtr -> TaskStack))
				{

					TaskPtr -> TaskSleepTime = Port_GetSystemTicks();
					TaskPtr -> TaskSleepTime += SleepTimeTicks;
					TaskPtr -> TaskStatus = TASK_BLOCKED;
					TaskPtr -> QueueElement.DataPtr = TaskPtr;
					TaskPtr -> QueueElement.Next = NULL;
					Res = Queue_Enqueue(&TaskTimeBlocked, &TaskPtr -> QueueElement, TaskPtr);
					if (Res == QUEUE_OK)
					{
						CallScheduler = TRUE;
					}
					else
					{
						TaskPtr -> TaskStatus = TASK_RUNNING;
					}
				}
				else
				{
					//Scheduler_SetstackOverflowTask(ActualTaskPtr);
					TaskPtr -> TaskStackOverflow = TRUE;
					Res = Scheduler_SetStackOverflowTask(TaskPtr);
					CallScheduler = TRUE;


				}
			}
			else
			{
				Res = TASK_ERR_UNDEF;
			}
		}
		vPortExitCritical();
		if (CallScheduler)
		{
			Port_SoftwareInterrupt();
		}
	}
	return Res;
}

void Task_CheckTimeBlockedTask()
{
	UInt16 Res = 0;
	unsigned long long Time;
	T_QueueHandler TempQueue;
	vPortEnterCritical();
	Time = Port_GetSystemTicks();
	Res = Queue_Init(&TempQueue);
	if(Res == QUEUE_OK)
	{
		T_TaskTCB_Ptr TaskPtr;
		while (Queue_Dequeue(&TaskTimeBlocked, (void **)& TaskPtr) == QUEUE_OK)
		{
			if (TaskPtr != NULL)
			{
				if (TaskPtr -> TaskSleepTime > Time)
				{
					Res = Queue_Enqueue(&TempQueue, &TaskPtr -> QueueElement, TaskPtr);
				}
				else
				{
					Scheduler_SetReadyTask(TaskPtr);
				}
			}
		}
		Res = Queue_Copy(&TempQueue, &TaskTimeBlocked);
	}
	vPortExitCritical();
}









/**********************************************************************************************************
***********************************************************************************************************
*
* IMPLEMENTACIÓN DE FUNCIONES INTERNAS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/

