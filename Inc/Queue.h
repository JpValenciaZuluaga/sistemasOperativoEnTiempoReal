/*
 * Queue.h
 *
 *  Created on: Nov 1, 2023
 *      Author: diego
 */

#ifndef QUEUE_H_
	#define QUEUE_H_

#include "types.h"

#ifndef QUEUE_BASE_ERROR
#define QUEUE_BASE_ERROR			0x0200
#endif

#define QUEUE_OK 					0x00
#define QUEUE_ERR_UNDEFF 			QUEUE_BASE_ERROR | 0x00FF
#define QUEUE_ERR_NULL_PARAM   		QUEUE_BASE_ERROR | 0x00FE

//#define QUEUE_ERR_NULL_PARAM		QUEUE_BASE_ERROR | 0x00FE
#define QUEUE_ERR_PARAM_OUT_RANGE	QUEUE_BASE_ERROR | 0x00FD
#define QUEUE_ERR_EMPTY 			QUEUE_BASE_ERROR | 0x00FC







/**********************************************************************************************************
***********************************************************************************************************
*
**DEFINICIÓN DE ERRORES DEL MODULO
***/



//#ifndef QUEUE_BASE_ERROR
//#define QUEUE_BASE_ERROR 0x0200
//#endif

//#define QUEUE_OK                   0
//#define QUEUE_ERR_UNDEF            QUEUE_BASE_ERROR | 0x00FF
//#define QUEUE_ERR_NULL_PARAM       QUEUE_BASE_ERROR | 0x00FE
//#define QUEUE_ERR_PARAM_OUT_RANGE  QUEUE_BASE_ERROR | 0x00FD
//#define QUEUE_ERR_EMPTY            QUEUE_BASE_ERROR | 0x00FC


/**********************************************************************************************************
***********************************************************************************************************
*
* DEFINICIÓN DE TIPOS DE DATOS DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/
typedef struct _queue_element
{
	void *DataPtr;
	struct _queue_element * Next;
}T_QueueElement, *T_QueueElementPtr;

typedef struct
{
	T_QueueElementPtr Head;
	T_QueueElementPtr Tail;
	UInt16 ElementsCount;
}T_QueueHandler, *T_QueueHandlerPtr;

/**********************************************************************************************************
***********************************************************************************************************
*
* DEFINICIÓN DE FUNCIONES DEL MÓDULO
*
* **********************************************************************************************************
* * **********************************************************************************************************/

UInt16 Queue_Init(T_QueueHandlerPtr Queue);
UInt16 Queue_GetElementsCount(T_QueueHandlerPtr Queue);
//UInt16 Queue_Enqueue(T_QueueHandlerPtr Queue, T_QueueElementPtr Element, Data);
UInt16 Queue_Enqueue(T_QueueHandlerPtr Queue, T_QueueElementPtr Element, void *Data);
//UInt16 Queue_Dequeue(T_QueueHandlerPtr Queue, Data);
UInt16 Queue_Dequeue(T_QueueHandlerPtr Queue, void ** Data);
UInt16 Queue_Copy(T_QueueHandlerPtr Origin, T_QueueHandlerPtr Destination);



#endif /* QUEUE_H_ */
