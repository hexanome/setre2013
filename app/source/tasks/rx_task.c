#include "rx_task.h"

/*******************************************************************************
* The Reception Task.
* Responsible for:
*   Receiving text packet received on the serial USB link from the computer
*******************************************************************************/

// Temp variable for IPC
OS_EVENT* msgQBufferRx = NULL;
OS_EVENT* qLcdRefresh = NULL;


void RxTask(void *args)
{
	INT8U err;
	void* textToRead = NULL;
		
	while (1)
	{
		// Read the message queue filled by the interrupt routine
		textToRead = OSQPend(msgQBufferRx, 0, &err);
		
		// Post the recognized text to be displayed on the LCD screen
		err = OSQPost (qLcdRefresh, textToRead);
		
		// Delay 100ms.
		//OSTimeDlyHMSM(0, 0, 0, 100);
	}
}

/************************************************************************/
