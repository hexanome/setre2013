#include "tx_task.h"

/*******************************************************************************
* The Transmission Task.
* Responsible for:
*   Transmitting voice packet on the serial USB link to make them analyze by the computer
*******************************************************************************/

void TXTask(void *args)
{
    // TODO: Do something.
    
    while (1) {
        // Delay 100ms.
        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}