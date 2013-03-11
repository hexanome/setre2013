#ifndef __SYNCHRONIZATION_H__
#define __SYNCHRONIZATION_H__

#include <includes.h>

/*******************************************************************************
* Defines.
*******************************************************************************/

#define QUEUE_LCDREFRESH_LENGTH         1
#define QUEUE_TOGGLERECORD_LENGTH       1

/*******************************************************************************
* Variables.
*******************************************************************************/

// Queues.
static QueueData qLcdRefreshData[QUEUE_LCDREFRESH_LENGTH];
extern Queue qLcdRefresh;

static QueueData qToggleRecordData[QUEUE_TOGGLERECORD_LENGTH];
extern Queue qToggleRecord;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void SetupSynchronization();
void WaitOn(Queue queue);
void Trigger(Queue queue);

#endif