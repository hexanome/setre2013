#ifndef __RECORDTASK_H__
#define __RECORDTASK_H__

#include <includes.h>

/*******************************************************************************
* Defines.
*******************************************************************************/

#define QUEUE_SYNCDMA_LENGTH            1
#define SIZE_OF_AUDIO_BUFFER            512

/*******************************************************************************
* Variables.
*******************************************************************************/

// Tasks.
extern OS_STK RecordTaskStack[RECORD_TASK_STACK_SIZE];

static unsigned long lastAudioByte = 0;

// Queues.
static void* qSyncDMAData[QUEUE_SYNCDMA_LENGTH];
static OS_EVENT* qSyncDMA;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void RecordTask(void *args);
void InitializeQSyncDMA(void);

// Private.
static void setupRecord(void);
static void stopRecord(void);

#endif