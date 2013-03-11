#ifndef __SYNCHRONIZATION_H__
#define __SYNCHRONIZATION_H__

#include "includes.h"

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
static void *qLcdRefreshData[QUEUE_LCDREFRESH_LENGTH];
static Queue *qLcdRefresh;

static void *qToggleRecordData[QUEUE_TOGGLERECORD_LENGTH];
static Queue *qToggleRecord;

static void *qTxBufferData[QUEUE_TXBUFFER_LENGTH];
static Queue *qTxBuffer;

/*******************************************************************************
* Function prototypes.
*******************************************************************************/

// Public.
void SetupSynchronization();
void WaitOn(Queue *queue);
void Trigger(Queue *queue);

#endif