#ifndef __MAINTASK_H__
#define __MAINTASK_H__

#include <includes.h>

#include "buttons_task.h"
#include "record_task.h"
#include "lcd_task.h"

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
static OS_STK MainTaskStack[MAIN_TASK_STACK_SIZE];

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void MainTask(void *args);

// Private.
static void TasksCreate();
static void TaskStart(void (*task)(void *), int priority, OS_STK *stack, int stackSize);

#endif