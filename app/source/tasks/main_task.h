#ifndef __MAINTASK_H__
#define __MAINTASK_H__

#include <includes.h>

//#include "../shared/task_helpers.h"

#include "buttons_task.h"
#include "record_task.h"
#include "lcd_task.h"
#include "rx_task.h"
#include "tx_task.h"

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
extern OS_STK MainTaskStack[MAIN_TASK_STACK_SIZE];

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void MainTask(void *args);

// Private.
static void TasksCreate();

#endif