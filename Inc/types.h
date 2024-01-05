/*
 * types.h
 *
 *  Created on: Nov 1, 2023
 *      Author: diego
 */

#ifndef TYPES_H_
#define TYPES_H_

#define MAIN_BASE_ERROR        0x0100
#define QUEUE_BASE_ERROR       0x0200
#define SCHEDULER_BASE_ERROR   0X0300
#define TASK_BASE_ERROR   	   0X0400

// Tipos de datos para enteros de 8 bits
//#ifndef Int8
	//typedef int8_t Int8;
//#endif

#ifndef	UInt8
	typedef unsigned char UInt8;
#endif

// Tipos de datos para enteros de 16 bits
//typedef int16_t Int16;

#ifndef	UInt16
	typedef unsigned short UInt16;
#endif


// Tipos de datos para enteros de 32 bits
//typedef int32_t Int32;

#ifndef	UInt32
	typedef unsigned int UInt32;
#endif

#ifndef UL
	typedef unsigned long UL;
#endif
//typedef uint32_t UInt32;

// Tipos de datos para caracteres de 8 bits
//typedef char Char;
//typedef unsigned char UChar;

// Tipos de datos para números en punto flotante de 32 bits
#ifndef pu32
	typedef float * pu32;
#endif

// Tipos de datos para números en punto flotante de 64 bits
typedef double Double64;

#define NULL ((void *) 0)
#define _PTR_ *

#define FALSE 0
#define TRUE  1

// Definir estructuras (si es necesario)
//typedef struct {
//    Int x;
//    Int y;
//} Point;


#endif /* TYPES_H_ */
