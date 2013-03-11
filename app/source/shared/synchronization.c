#include "synchronization.h"

/*******************************************************************************
* Variable declaration.
*******************************************************************************/

OS_EVENT *qLcdRefresh;
OS_EVENT *qToggleRecord;

/*******************************************************************************
* Method implementation.
*******************************************************************************/

void SetupSynchronization()
{
    // Create the LcdRefresh queue.
    qLcdRefresh = OSQCreate(&qLcdRefreshData[0], QUEUE_LCDREFRESH_LENGTH);
    
    // Create the ToggleRecord queue.
    qToggleRecord = OSQCreate(&qToggleRecordData[0], QUEUE_TOGGLERECORD_LENGTH);
}

void WaitOn(Queue queue)
{
    INT8U err;
    OSQPend(queue, 0, &err);
            
    char error[1];
    if (err == OS_ERR_NONE)
    { 
        error[0] = 'A';
    }
    else if (err == OS_ERR_Q_FULL)
    {
        error[0] = 'B';
    }
    else if (err == OS_ERR_EVENT_TYPE)
    {
        error[0] = 'C';
    }
    else if (err == OS_ERR_PEVENT_NULL)
    {
        error[0] = 'D';
    }
    else
    {
        error[0] = 'E';
    }
}
    
void Trigger(Queue queue)
{    
    int data[1] = { 0 };
    OSQPost(queue, &data[0]);
}