/*
 * RtosInclude.h
 *
 *  Created on: Nov 1, 2023
 *      Author: diego
 */

#ifndef RTOSINCLUDE_H_
#define RTOSINCLUDE_H_

#include "types.h"
#include "Rtos.h"
#include "Task.h"
#include "Scheduler.h"
#include "Port.h"

#define RTOS_Start()			Scheduler_Start()
/*#define RTOS_Init()
	{
	Scheduler_Init();
	Task_Init();
	}*/
#define RTOS_Init() { \
    Scheduler_Init(); \
    Task_Init(); \
}



#endif /* RTOSINCLUDE_H_ */
