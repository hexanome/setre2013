#include "rx_task.h"

/*******************************************************************************
* The Reception Task.
* Responsible for:
*   Receiving text packet received on the serial USB link from the computer
*******************************************************************************/

void RXTask(void *args)
{
    // TODO: Do something.
    
    while (1) {
        // Delay 100ms.
        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}