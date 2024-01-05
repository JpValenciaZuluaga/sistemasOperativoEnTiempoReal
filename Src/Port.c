/*
 * Port.c
 *
 *  Created on: Nov 1, 2023
 *      Author: diego
 */

//DEFINICION DE LIBRERIA DE VARIABLES, PENDIENTE DE HACERLA

#include "Port.h"
#include "types.h"
#define SYSTEM_CLOCK    8000000
#define TICK_CLOCK      1000

#define STACK_CHECK_VALUE 0xAAAAAAAA
#define STACK_CHECK_SIZE  20

//void Port_InitClearStack(UInt32 *Stack, UInt32 Size);
void Port_InitClearStack(unsigned int *Stack, unsigned int Size);
extern UInt32 Scheduler_GetNextTask(UInt32 ActualSp);
//static UInt8 xPortIsInsideInterrupt(void);

//static unsigned long long PortSystemTicks=0;
//static UInt32 uxCriticalNesting=0xaaaaaaaa;
static unsigned long long PortSystemTicks = 0;
static UInt32 uxCriticalNesting = 0xaaaaaaaa;


UInt32 PortActualSp=0;




__attribute__((always_inline))static inline void Port_SaveContex(void)
{
	__asm volatile("PUSH {R7, LR}"); //incluir si la interrución es NAKED

	__asm volatile("PUSH {R4 - R11}");
	//__asm volatile("MRS   MSP,R10");
	__asm volatile("MRS   R10,MSP"); /////////////////////////////////////////////// Pilaaaaaaaaaaaaaaaassssssssssssss
	__asm volatile("MOV  %0,R10":"=r"(PortActualSp));
}

__attribute__((always_inline))static inline void Port_RestoreContex(void)
{
	__asm volatile("MOV  R10,%0": :"r"(PortActualSp));
	__asm volatile("MSR   MSP,R10");
	//__asm volatile("MRS   R10,MSP"); ///////////////////////////////////////////////// PILAAAAAAAAAAAAAAAASSSSSSSSSSSSS
	__asm volatile("POP {R4 - R11}");
	//__asm volatile("POP {R4-R6}");
	//__asm volatile("POP {R7}");
	__asm volatile("NOP");           //incluir si la interrución es NAKED
	__asm volatile("POP {R7,PC}");   //incluir si la interrución es NAKED


}
void Port_InitTickTimer()
{

	////////////////////////////////////////////////
	//UInt32 *pSTK_CTRL = (UInt32)*0xE000E010; // es el puntero unsigned de 32 bits libreria creada por él
	volatile UInt32 *pSTK_CTRL = (volatile UInt32 *)0xE000E010;

	//UInt32 *pSTK_LOAD = (UInt32)*0xE000E014;
	volatile UInt32 *pSTK_LOAD = (volatile UInt32 *)0xE000E014;


	UInt32 TickCountValue = (SYSTEM_CLOCK/TICK_CLOCK)-1;

	*pSTK_LOAD &= ~(0x00FFFFFF);

	*pSTK_LOAD |=TickCountValue;

	*pSTK_CTRL |= (1 << 2); // fuente de Reloj de la CPU
	*pSTK_CTRL |= (1 << 1); // Habilita intrrupcion del SYSTICK
	*pSTK_CTRL |= (1 << 0); // hABULITAMOS SYSTICK

	/////////////////////////////////////////////////

	//RestoreFirstContext((pu32)Stack1);

}
unsigned int Port_CheckStackOverflow(unsigned int *Stack)
{
	UInt8 Res = FALSE;
	UInt32 Index= STACK_CHECK_SIZE;
	while(Index)
	{
		Index--;
		if(Stack[Index]!= STACK_CHECK_VALUE)
		{
			Res= TRUE;
			break;
		}
	}
	return Res;
}


void Port_InitClearStack(unsigned int *Stack, unsigned int Size)
{
	while(Size)
	{
		*Stack = STACK_CHECK_VALUE;
		Stack++;
		Size--;
	}
}

unsigned int *Port_InitTaskStack(unsigned int *Stack, unsigned int Size, void *Function)
{
	Port_InitClearStack(Stack,Size);
	Stack += (Size);
	Stack --;
	*Stack = 0x01000000; // PSR
	Stack --;
	//*Stack = (UInt32)(Function) & 0xfffffffeunsigned long;// PC
	*Stack = (UInt32)(Function) & 0xfffffffeUL; // PC

	*Stack |= 0x00000001;
	Stack --;
	//*Stack = (UInt32)(Function) & 0xfffffffeunsigned long;// LR
	*Stack = (UInt32)(Function) & 0xfffffffeUL;// LR
	Stack --;
	*Stack = 0; // R12
	Stack --;
	*Stack = 0; // R3
	Stack --;
	*Stack = 0; // R2
	Stack --;
	*Stack = 0; // R1
	Stack --;
	*Stack = 0; // R0
	Stack --;
	*Stack = 0xFFFFFFF9; //LR
	Stack --;
	*Stack = 0; // R7
	Stack --;
	*Stack = 0; // R11
	Stack --;
	*Stack = 0; // R10
	Stack --;
	*Stack = 0; // R9
	Stack --;
	*Stack = 0; // R8
	Stack --;
	*Stack = 0; // R7
	Stack --;
	*Stack = 0; // R6
	Stack --;
	*Stack = 0; // R5
	Stack --;
	*Stack = 0; // R4

	//Stack --; No se requiere el decremento final porque el stack apunta siempre a la posicion final.
	return Stack;
}

/*UInt32 SaveContext()
{
	UInt32 Sp = 0;
	__asm volatile("PUSH {R4-R11}");
	return Sp;
}

void RestoreContext(UInt32 Sp)
{

}
*/
void Port_RestoreFirstContext(UInt32 Sp)
{
	uxCriticalNesting = 0;
	vPortExitCritical();
	__asm volatile("MOV R10, %0" :: "r"(Sp));
	__asm volatile("MSR MSP, R10");
	__asm volatile("POP {R4-R11}");
	__asm volatile("POP {R7, LR}");
	//__asm volatile("POP {R7}");
	__asm volatile("POP {R0 - R3}");
	__asm volatile("POP {R12}");
	__asm volatile("POP {LR}");
	__asm volatile("POP {LR}");
	__asm volatile("POP {R10}");
	__asm volatile("MSR PSR, R10");
	__asm volatile("MOV R10, #0");
	__asm volatile("BX LR");
}

/*void Task1()
{
	for(;;);
}
void Task2()
{
	for(;;);
}
void Port_InitTiskTimer(void)
{

}*/
unsigned long long Port_GetSystemTicks()
{
	return PortSystemTicks;
}
__attribute__((naked))void SysTick_Handler (void)
//void SysTick_Handler(void)
{
	Port_SaveContex();
	PortSystemTicks ++;
	PortActualSp = Scheduler_GetNextTask(PortActualSp);
	Port_RestoreContex();



	//__asm volatile("PUSH {R4-R6}");
	//__asm volatile("PUSH {R8-R11}");

	//__asm volatile("POP {R8-R11}");
	//__asm volatile("POP {R4-R6}");

}

__attribute__((naked)) void PendSV_Handler (void)
{
	Port_SaveContex();
	//PortSystemTicks ++;
	PortActualSp = Scheduler_GetNextTask(PortActualSp);
	Port_RestoreContex();

}
/* *********************************************************************************/
void vPortEnterCritical (void)
{
	//portDISABLE_INTERRUPTS();
	//uxCriticalNesting ++;
	/* This is not the interrumpt safe version of the enter critical function so
	 * assert() if it is being called from an interrupt context Only API
	 * functions that end in "FromISR" can be used in an interrupt. Only assert if
	 * the critical nesting count is 1 to protect against recursive calls if the
	 * assert function also uses a critical selection */

	//if ( !xPortIsInsideInterrupt())
	{
		__asm volatile ("cpsid i" : : : "memory");
		uxCriticalNesting ++;
		//( portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK) == 0;
	}

}
/* *********************************************************************************/

void vPortExitCritical (void)
{
	//configASSERT (uxCriticalNesting);
	//if (!xPortInsideInterrupt())
	{
		if (uxCriticalNesting)
		{
			uxCriticalNesting --;
		}
		if (uxCriticalNesting == 0)
		{
			__asm volatile ("cpsie i" : : : "memory");
		}
	}
}
/* *********************************************************************************/

inline __attribute__((always_inline)) static UInt8 xPortIsInsideInterrupt(void)
{
	UInt32 ulCurrentInterrupt;
	UInt8 xReturn;

	/* Obtain the number of the currently executing interrumpt */
	__asm volatile ("mrs %0, ipsr" : "=r" (ulCurrentInterrupt) : : "memory");

	if (ulCurrentInterrupt == 0)
	{
		xReturn = FALSE;
	}
	else
	{
		xReturn = TRUE;
	}
}

