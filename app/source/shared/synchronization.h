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
static QueueData qLcdRefreshData[QUEUE_LCDREFRESH_LENGTH];
extern Queue qLcdRefresh;

static QueueData qToggleRecordData[QUEUE_TOGGLERECORD_LENGTH];
extern Queue qToggleRecord;

static void *qTxBufferData[QUEUE_TXBUFFER_LENGTH];
extern Queue qTxBuffer;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void SetupSynchronization();
void WaitOn(Queue queue);
INT8U PeekOn(Queue queue);
void Trigger(Queue queue);

#endif