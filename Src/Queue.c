/*
 * Queue.c
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
#include "Queue.h"
#include "types.h"
#include "Port.h"























































UInt16 Queue_Init(T_QueueHandlerPtr Queue)
{
	if(Queue != NULL)
	{
		vPortEnterCritical();
		Queue -> ElementsCount = 0;
		Queue -> Head = NULL;
		Queue -> Tail = NULL;
		vPortExitCritical();
		return QUEUE_OK;
	}
	return QUEUE_ERR_NULL_PARAM;
}












UInt16 Queue_GetElementsCount(T_QueueHandlerPtr Queue)
{
	if (Queue != NULL)
	{
		return Queue -> ElementsCount;
	}
	return 0;
}












UInt16 Queue_Enqueue(T_QueueHandlerPtr Queue, T_QueueElementPtr Element, void *Data)
{
	if ((Queue != NULL)&& (Element != NULL))
	{
		vPortEnterCritical();
		Element -> DataPtr = Data;
		Element -> Next = NULL;
		if (Queue -> Head == NULL)
		{
			Queue -> Head = Element;
			Queue -> Tail = Element;
			Queue -> ElementsCount = 1;
		}
		else
		{
			Queue -> Tail -> Next = Element;
			Queue -> Tail = Element;
			Queue -> ElementsCount ++;
		}
		vPortExitCritical();
		return QUEUE_OK;
	}
	return QUEUE_ERR_NULL_PARAM;
}











//UInt16 Queue_Dequeue(T_QueueHandlerPtr Queue, **Data)
UInt16 Queue_Dequeue(T_QueueHandlerPtr Queue, void ** Data)
{
	if ((Queue != NULL) && (Data != NULL))
	{
		vPortEnterCritical();
		if (Queue ->Head != NULL)
		{
			*Data = Queue -> Head -> DataPtr;
			Queue -> Head = Queue -> Head -> Next;
			Queue -> ElementsCount --;
			if (Queue -> Head == NULL)
			{
				Queue -> ElementsCount =0;
			}
			vPortExitCritical();
			return QUEUE_OK;
		}
		Queue -> Tail = NULL;
		Queue -> ElementsCount = 0;
		vPortExitCritical();
		return QUEUE_ERR_EMPTY;
	}
	return QUEUE_ERR_NULL_PARAM;
}

UInt16 Queue_Copy(T_QueueHandlerPtr Origin, T_QueueHandlerPtr Destination)
{
	UInt8 Res = QUEUE_ERR_NULL_PARAM;
	if((Origin != NULL) && (Destination != NULL))
	{
		vPortEnterCritical();
		Destination -> Head = Origin -> Head;
		Destination -> Tail = Origin -> Tail;
		Destination -> ElementsCount = Origin -> ElementsCount;
		vPortExitCritical();
		Res = QUEUE_OK;
	}
	return Res;
}







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

/**********************************************************************************************************
***********************************************************************************************************
*
* DEFINICIÓN DE VARIABLES GLOBALES INTERNAS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/


/**********************************************************************************************************
***********************************************************************************************************
*
* IMPLEMENTACIÓN DE FUNCIONES EXTERNAS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/

