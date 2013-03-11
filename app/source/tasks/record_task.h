#ifndef __RECORDTASK_H__
#define __RECORDTASK_H__

#include <includes.h>  

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
static OS_STK RecordTaskStack[RECORD_TASK_STACK_SIZE];

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void RecordTask(void *args);

#endif