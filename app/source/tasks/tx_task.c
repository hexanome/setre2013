#include "tx_task.h"
#include "hal_usb.h"

// Init the usb connection in the main task

// While developping the application, error code have to be checked
#define DEBUG 1

// Elsewhere (flash chunk to read)
#define SIZEOF_BLOCK 255

/*******************************************************************************
* The Transmission Task.
* Responsible for:
*   Transmitting voice packet on the serial USB link to make them analyze by the computer
*******************************************************************************/

// Temp variable for IPC
OS_EVENT* msgQBufferTx = NULL;

// Private variable
static char blockSize = SIZEOF_BLOCK;

void TxTask(void *args)
{
    // Message in the queue
    void* blockToRead = NULL;
    INT8U err;
    // Memory bounds
    void* startOffset = NULL;
    void* endOffset = NULL;
    // Flash content
    char blockToSend [SIZEOF_BLOCK] = {0};
    
    halUsbInit();
    
    while (1)
    {
        // Waits for a new block to send
        blockToRead = OSQPend(msgQBufferTx, 0, &err);
        
#if DEBUG > 0
        if ( err != OS_ERR_NONE )
          printf("Problem to withdraw information in message queue\n");
#endif

        // Read the flash in order to retrieve the string to be sent
        // TODO
  
        // Writes it on the serial line to communicate with the computer
        halUsbSendString ( blockToSend, blockSize );

        // Delay 100ms.
        //OSTimeDlyHMSM(0, 0, 0, 100);
    }
}