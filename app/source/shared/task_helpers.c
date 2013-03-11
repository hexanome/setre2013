#include "task_helpers.h"

/*******************************************************************************
* Method implementation.
*******************************************************************************/

void TaskStart(void (*task)(void *), int priority, OS_STK *stack, int stackSize)
{
    OSTaskCreateExt(task, 
                    (void *)0,
                    (OS_STK *)&stack[stackSize - 1],
                    priority,
                    priority,
                    (OS_STK *)&stack[0],
                    stackSize,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}