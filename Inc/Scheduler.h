/*
 * Scheduler.h
 *
 *  Created on: Nov 1, 2023
 *      Author: diego
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/**********************************************************************************************************
***********************************************************************************************************
*
* INCLUSIÓN DE LIBRERÍAS
*
* **********************************************************************************************************
* * **********************************************************************************************************/
#include "types.h"
#include "Rtos.h"
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
#ifndef SCHEDULER_BASE_ERROR
	#define SCHEDULER_BASE_ERROR   0X0300
#endif

#define SCHEDULER_OK            		0
#define SCHEDULER_ERR_UNDEF     		SCHEDULER_BASE_ERROR 		| 0x00FF
#define SCHEDULER_ERR_NULL_PARAM   		SCHEDULER_BASE_ERROR     	| 0x0FE


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

UInt16 Scheduler_Init();
UInt16 Scheduler_SetReadyTask(T_TaskTCB_Ptr Task);
//UInt16 Scheduler_SetReadyTask(T_TaskTCB_ Ptr Task);
UInt16 Scheduler_SetStackOverflowTask(T_TaskTCB_Ptr Task);
UInt32 Scheduler_GetNextTask(UInt32 ActualSp);
void Scheduler_Start();
T_TaskTCB_Ptr Scheduler_GetActualTask();

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
* * *****************************************/

#endif /* SCHEDULER_H_ */
