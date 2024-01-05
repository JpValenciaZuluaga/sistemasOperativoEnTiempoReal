/*
 * Port.h
 *
 *  Created on: Nov 1, 2023
 *      Author: diego
 */

#ifndef PORT_H_
#define PORT_H_

//#define "types.h"

/*
typedef void (*TaskCallback)(void);

#define portNVIC_INT_CTRL_REF             (*( volatile u32 *)0xe000ed04) )
#define portVCIC_PENDSVSET_BIT            ( 1UL << 28UL)

#define RCC_APB2ENR                       (*( (volatile u32 *)0x40021018))
#define RCC_APB2ENR_IOPC_EN               ( 1UL << 4UL)

#define GPIOC_CRL                         (*( (volatile u32 *)0x40011000))
#define GPIOC_CRH                         #define GPIOC_
#define GPIOC_CRH_MODE13                  ( 3UL << 20UL)
#define GPIOC_CRH_CNF13                   ( 3UL << 22UL)

#define GPIOC_BSRR                        (*( (volatile u32 *)0x40011010))
#define GPIOC_BSRR_BS13                   ( 1UL << 13UL)
#define GPIOC_BSRR_BR13                   ( 1UL << 29UL)
*/
/*critical section management. */
//extern void vPortEnterCritical(void);
//extern void vPortExitCritical(void);
/*
void Port_InitTickTimer();
UInt8 Port_CheckStackOverflow(UInt32 *Stack,);
void Port_InitClearStack(UInt32 *Stack, UInt32 Size);
UInt32 *Port_InitTaskStack(UInt32 Stack,UInt32 Size, TaskCallback Function);
void Port_RestoreFirstContext();
unsigned long long Port_getSystemTicks();
*/

//#define Port_SoftwareInterrupt
		//{
	/* Set a PendSV to request a context switch. */
//	portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;

	/* Barriers are normally not required but do ensure the code is completely
	 * within the specified behaviour for the architecture.  */

//	__asm volatile ("dsb" ::: "memory");
//	__asm volatile ("isb");
//		}
#define Port_SoftwareInterrupt() \
     { \
        /* Set a PendSV to request a context switch. */ \
        portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
        /* Barriers are normally not required but do ensure the code is completely \
         * within the specified behaviour for the architecture.  */ \
        __asm volatile ("dsb" ::: "memory"); \
        __asm volatile ("isb"); \
    }


#define portNVIC_INT_CTRL_REG		(*((volatile UInt32 * ) 0xe000ed04))
#define portNVIC_PENDSVSET_BIT		( 1UL <<  28UL)

#define RCC_APB2ENR					(*((volatile UInt32 * ) 0x40021018))
#define RCC_APB2ENR_IOPC_EN			( 1UL <<  4UL)

#define GPIOC_CRL 					(*((volatile UInt32 * ) 0x40011000))
#define GPIOC_CRH 					(*((volatile UInt32 * ) 0x40011004))
#define GPIOC_CRH_MODE13			( 3UL << 20UL) //(3UL << 20UL)
#define GPIOC_CRH_CNF13				( 3UL << 22UL)
#define GPIOC_BSRR					(*((volatile UInt32 * ) 0x40011010))
#define GPIOC_BSRR_BS13				(  1UL <<  13UL)
#define GPIOC_BSRR_BR13				(  1UL <<  29UL)

//#define RCC_APB2ENR_IOPC_EN			( (unsigned long) 1 <<  (unsigned long) 4)
//#define GPIOC_CRH_MODE13				( (unsigned long) 3 << (unsigned long) 20) //(3UL << 20UL)
//#define GPIOC_CRH_CNF13				( (unsigned long) 3 << (unsigned long) 22)
//#define GPIOC_BSRR_BS13				( (unsigned long) 1 << (unsigned long) 13)
//#define GPIOC_BSRR_BR13				( (unsigned long) 1 << (unsigned long) 29)


//Configuracion Puerto B
//#define RCC_APB2ENR              	(*((volatile UInt32 *)0x40021018))
#define RCC_APB2ENR_IOPB_EN         (1UL << 3UL)  // Habilitar el reloj para el puerto GPIOB

#define GPIOB_CRL                   (*((volatile UInt32 *)0x40010C00))  // Dirección base del registro de configuración de pines bajos
#define GPIOB_CRH                   (*((volatile UInt32 *)0x40010C04))  // Dirección base del registro de configuración de pines altos

// Configurar Pin 5 de GPIOB como salida
#define GPIOB_CRL_MODE5             (3UL << 20UL)  // Salida a velocidad máxima (50 MHz)
#define GPIOB_CRL_CNF5              (3UL << 22UL)  // Modo de salida push-pull

#define GPIOB_BSRR                  (*((volatile UInt32 *)0x40010C10))  // Registro de set/reset de bits

#define GPIOB_BSRR_BS5              (1UL << 5UL)  // Establecer el Pin 5 de GPIOB
#define GPIOB_BSRR_BR5              (1UL << 21UL)


#define GPIOB_CRL_MODE1             (3UL << 4UL)   // Salida a velocidad máxima (50 MHz) para el Pin 1 de GPIOB
#define GPIOB_CRL_CNF1              (3UL << 6UL)   // Modo de salida push-pull para el Pin 1 de GPIOB

#define GPIOB_BSRR_BS1              (1UL << 1UL)   // Establecer el Pin 1 de GPIOB
#define GPIOB_BSRR_BR1              (1UL << 17UL)  // Restablecer el Pin 1 de GPIOB

// Definiciones para GPIOA como entrada
#define RCC_APB2ENR_IOPA_EN     (1UL << 2UL)  // Habilitar el reloj para el puerto GPIOA

#define GPIOA_CRL               (*((volatile uint32_t *)0x40010800))  // Registro de configuración de pines bajos de GPIOA
#define GPIOA_CRH               (*((volatile uint32_t *)0x40010804))  // Registro de configuración de pines altos de GPIOA

#define GPIOA_CRL_CNF0          (2UL << 0UL)  // Modo de entrada para el pin 0 de GPIOA
#define GPIOA_CRL_CNF1          (2UL << 4UL)  // Modo de entrada para el pin 1 de GPIOA
#define GPIOA_CRL_CNF2          (2UL << 8UL)  // Modo de entrada para el pin 2 de GPIOA
#define GPIOA_CRL_CNF3          (2UL << 12UL) // Modo de entrada para el pin 3 de GPIOA
#define GPIOA_CRL_CNF4          (2UL << 16UL) // Modo de entrada para el pin 4 de GPIOA
#define GPIOA_CRL_CNF5          (2UL << 20UL) // Modo de entrada para el pin 5 de GPIOA
#define GPIOA_CRL_CNF6          (2UL << 24UL) // Modo de entrada para el pin 6 de GPIOA
#define GPIOA_CRL_CNF7          (2UL << 28UL) // Modo de entrada para el pin 7 de GPIOA

#define GPIOA_CRH_CNF8          (2UL << 0UL)  // Modo de entrada para el pin 8 de GPIOA
#define GPIOA_CRH_CNF9          (2UL << 4UL)  // Modo de entrada para el pin 9 de GPIOA


/* Critical section management */

extern void vPortEnterCritical(void);
extern void vPortExitCritical(void);

/*#define portSET_INTERRUPT_MASK_FROM_ISR()		ulPortRaiseBASEPRI()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	vPortSetBASEPRI(x)
#define portDISABLE_INTERRUPTS()				vPortRaiseBASEPRI()
#define portENABLE_INTERRUPTS()					vPortSetBASEPRI(0)
#define portENTER_CRITICAL()					vPortEnterCritical()
#define portEXIT_CRITICAL()						vPortExitCritical()
#define configMAX_SYSCALL_INTERRUPT_PRIORITY	63*/
///////////////////////////////////////////////////////////////////////////////////

/* *********************************************************************************/
/*#define portINLINE 								__inline

#ifndef portFORCE_INLINE
	#define portFORCE_INLINE 					inline __attribute ((always_inline))
#endif*/

/* *********************************************************************************/

/*portFORCE_INLINE static long xPortInsideInterrupt(void)
{
	UInt32 ulCurrentInterrupt;
	long xReturn;
*/
	/* Obtain the number of the currently executing interrupt*/
/*	__asm volatile ("mrs %0, ipsr" : "=r" (ulCurrentInterrupt) :: "memory");

	if (ulCurrentInterrupt == 0)
	{
		xReturn = FALSE;
	}
	else
	{
		xReturn = TRUE;
	}
	return xReturn;
}*/
/* *********************************************************************************/

/*portFORCE_INLINE static void vPortRaiseBASEPRI (void)
{
	UInt32 ulNewBASEPRI;

	__asm volatile
	(
		" mov %0, %1 									\n"\
		" msr basepri, %0 								\n"\
		" isb 											\n"\
		" dsb 											\n"\
		: "=r" (ulNewBASEPRI) : "i" (configMAX_SYSCALL_INTERRUPT_PRIORITY) : "memory"
	);
}*/
/* *********************************************************************************/

/*portFORCE_INLINE static UInt32 ulPortRaiseBASEPRI (void)
{
	UInt32 ulOriginalBASEPRI, ulNewBASEPRI;

	__asm volatile
	(
			" mrs %0, basepri 							\n"\
			" mov %1, %2								\n"\
			" msr basepri, %1							\n"\
			" isb 										\n"\
			" dsb										\n"\
			: "=r" (ulOriginalBASEPRI), "=r" (ulNewBASEPRI) : "i" (configMAX_SYSCALL_INTERRUPT_PRIORITY) : "memory"
	);*/
	/* This return will not be reached but is necessary to prevent compiler warnings */
//	return ulOriginalBASEPRI;
//}
/* *********************************************************************************/

/*porFORCE_INLINE static void vPortSetBASEPRI (UInt32 ulNewMaskValue)
{
	__asm volatile
	(
			"msr basepri, %0" :: "r" (ulNewMaskValue) : "memory"
	);
}
*/
/* *********************************************************************************/
//#define portMEMORY_BARRIER()		__asm volatile ("" ::: "memory")

void Port_InitTickTimer();
unsigned int Port_CheckStackOverflow(unsigned int *Stack);
void Port_InitClearStack(unsigned int *Stack, unsigned int Size);
//UInt8 Port_CheckStack();
//unsigned int  *PortInitTaskStack((unsigned int *Size), void *Function);
unsigned int *Port_InitTaskStack(unsigned int *Stack, unsigned int Size, void *Function);

void Port_RestoreFirstContext(unsigned int Sp);
unsigned long long Port_GetSystemTicks();


#endif /* PORT_H_ */
