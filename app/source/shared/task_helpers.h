#ifndef __TASK_HELPERS_H__
#define __TASK_HELPERS_H__

#include <includes.h>

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void TaskStart(void (*task)(void *), int priority, OS_STK *stack, int stackSize);

#endif