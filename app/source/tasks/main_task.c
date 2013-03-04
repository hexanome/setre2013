#include "main_task.h"

/*******************************************************************************
* The Main Task.
* Responsible for:
*   Hardware initialization.
*   Starting other tasks.
*******************************************************************************/

void MainTask(void *args)
{    
    
    
    // Continue running but do nothing.
    while (1) {
        // Delay 100s.
        OSTimeDlyHMSM(0, 0, 100, 0);
    }
}