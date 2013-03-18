#ifndef __TX_TASK__
#define __TX_TASK__

#include <includes.h>

#define QUEUE_SYNCDMA1_LENGTH 1

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
extern OS_STK TxTaskStack[TX_TASK_STACK_SIZE];

// Queue
static void* qSyncDMA1Data[QUEUE_SYNCDMA1_LENGTH];
extern OS_EVENT* qSyncDMA1; // type: INT8U

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void TxTask(void *args);

void InitializeQSyncDMA1(void);

void setupDMA (void);

// Private.

#endif