#include "synchronization.h"

/*******************************************************************************
* Variable definition.
*******************************************************************************/

OS_EVENT *qLcdRefresh;
OS_EVENT *qToggleRecord;
OS_EVENT *qTxBuffer;

/*******************************************************************************
* Method implementation.
*******************************************************************************/

void SetupSynchronization()
{
	// Create the LcdRefresh queue.
	qLcdRefresh = OSQCreate(qLcdRefreshData, QUEUE_LCDREFRESH_LENGTH);
  
  // Create the ToggleRecord queue.
	qToggleRecord = OSQCreate(qToggleRecordData, QUEUE_TOGGLERECORD_LENGTH);

	// Create the TxBuffer queue.
	qTxBuffer = OSQCreate(qTxBufferData, QUEUE_TXBUFFER_LENGTH);
}

// Blocking on a message queue
void WaitOn(Queue queue)
{
	INT8U err;
	OSQPend(queue, 0, &err);
            
//    char error[1];
//    if (err == OS_ERR_NONE)
//    { 
//        error[0] = 'A';
//    }
//    else if (err == OS_ERR_Q_FULL)
//    {
//        error[0] = 'B';
//    }
//    else if (err == OS_ERR_EVENT_TYPE)
//    {
//        error[0] = 'C';
//    }
//    else if (err == OS_ERR_PEVENT_NULL)
//    {
//        error[0] = 'D';
//    }
//    else
//    {
//        error[0] = 'E';
//    }
}

// Non-blocking waiting on a message queue
bool PeekOn(Queue queue)
{
	INT8U err;
	OSQAccept(queue, &err);
	return err == OS_ERR_NONE;
}

// Simply posts a NULL-pointer message, used only for signaling	
void Trigger(Queue queue)
{
	OSQPost(queue, (void*)0);
}