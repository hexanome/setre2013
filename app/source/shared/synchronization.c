#include "synchronization.h"

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

void WaitOn(Queue *queue)
{
    INT8U err;
    OSQPend(queue, 0, &err);
}
    
void Trigger(Queue *queue)
{
    int data[1] = { 0 };
    OSQPost(queue, &data[0]);
}