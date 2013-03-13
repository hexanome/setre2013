#ifndef __SYNCHRONIZATION_H__
#define __SYNCHRONIZATION_H__

#include <includes.h>

/*******************************************************************************
* Defines.
*******************************************************************************/

#define QUEUE_LCDREFRESH_LENGTH         1
#define QUEUE_TOGGLERECORD_LENGTH       1
#define QUEUE_TXBUFFER_LENGTH           10

/*******************************************************************************
* Variables.
*******************************************************************************/

// Queues.
static void* qLcdRefreshData[QUEUE_LCDREFRESH_LENGTH];
extern OS_EVENT* qLcdRefresh;

static void* qToggleRecordData[QUEUE_TOGGLERECORD_LENGTH];
extern OS_EVENT* qToggleRecord;

static void* qTxBufferData[QUEUE_TXBUFFER_LENGTH];
extern OS_EVENT* qTxBuffer;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void SetupSynchronization();
void WaitOn(Queue queue);
bool PeekOn(Queue queue);
void Trigger(Queue queue);

#endif