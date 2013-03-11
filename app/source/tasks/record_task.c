#include "record_task.h"

/*******************************************************************************
* The Record Task.
* Responsible for:
*   Record audio from the microphone when needed.
*******************************************************************************/

void RecordTask(void *args)
{
    // TODO: Do something.
    for (;;)
    {
        WaitOn(qLcdRefresh);
        
        // Delay 100ms.
        OSTimeDlyHMSM(0, 0, 0, 100);
    }   
}