#include "tx_task.h"
#include "rx_task.h"
#include "record_task.h"
#include "lcd_task.h"

// Init the usb connection in the main task

// While developping the application, error code have to be checked
#define DEBUG 0

// Test without DMA
#define WO_DMA 1

/*******************************************************************************
* The Transmission Task.
* Responsible for:
*   Transmitting voice packet on the serial USB link to make them analyze by the computer
*******************************************************************************/

OS_STK TxTaskStack[TX_TASK_STACK_SIZE];

OS_EVENT* qSyncDMA1;

/*******************************************************************************
* Private variables
*******************************************************************************/
// Each chunk has fixed size
//static char blockSize = BUFFER_SIZE;

// Memory bounds
static INT16U memoryBegPtr = 0;
static INT16U memoryEndPtr = 0;

#if WO_DMA > 0
// Flash content read
static char blockToSend [SIZE_OF_AUDIO_BUFFER] = {0};
#endif

/*******************************************************************************
* Private function
*******************************************************************************/
// We'll use the DMA to transfer directly from the flash to the transmit register
void setupDMA (void)
{
	// Round-robin DMA
	DMACTL4 |= ROUNDROBIN;
	
	// DMA channel 1 trigger is set to USCIA1 transmit event
	DMACTL1 = DMA1TSEL_21;
	
	// The destination of the DMA's channel 1 is the TX register of the USB
	// DMAxDA => destinaton address (11.3.7 doc p402)
	//__data16_write_addr((unsigned long)&DMA1DA & 0xffff, (unsigned long)&UCA1TXBUF);
	__data16_write_addr((unsigned short)DMA1DA_, (unsigned long)&UCA1TXBUF);
}

void InitializeQSyncDMA1(void)
{
	qSyncDMA1 = OSQCreate(qSyncDMA1Data, QUEUE_SYNCDMA1_LENGTH);
}

/*******************************************************************************
* Task body function
*******************************************************************************/
void TxTask(void *args)
{
	// Message in the queue
	audioChunk* blockToRead = NULL;
	INT8U err;
	INT8U resultDMA;
#if WO_DMA > 0
	// Chunk size
	INT16U blockSize = QUEUE_RX_BUFFER_LENGTH;
	INT8U currentIdx = 0;
	INT16U currentPtr = 0;
#endif
	
	while (1)
	{
		// Waits for a new block to read in the flash memory
		blockToRead = (audioChunk*) OSQPend(qTxBuffer, 0, &err);
		// Keeps a local copy of memory addresses
		memoryBegPtr = blockToRead->startAddr;
		memoryEndPtr = blockToRead->endAddr;
				
#if DEBUG > 0
		if ( err != OS_ERR_NONE )
			DrawText(10, 30, "Pb qTxBuffer", PIXEL_ON);
		else
			DrawText(10, 50, "Ready", PIXEL_ON);
#endif

#if WO_DMA == 0
		// Reads the flash in order to retrieve the string to be sent		
		////
		// 1 - Set up the DMA controller
		// The source of the DMA's channel 1 is the pointer on the flash stored in the flash
		// DMAxSA => source address (11.3.8 doc p403)
		__data16_write_addr((unsigned short)DMA1SA_, memoryBegPtr);
			
		// Define the amount of information to be transferred (counts downwards to 0)
		// (11.3.9 doc p404)
		DMA1SZ = memoryEndPtr - memoryBegPtr;
		//DMA1SZ = SIZE_OF_AUDIO_BUFFER;
		
		// Enable Long-Word write, all 32 bits will be written once
		// 4 bytes are loaded
		DMA1CTL = DMADT_0 +
							DMASRCINCR_3 +
							DMADSTINCR_0 +
							DMADSTBYTE +
							DMASRCBYTE +
							DMAEN +
							DMAIE;
		
		////
		// 2 - Wait for the treatment's end
		// Initiate the transfer with a dummy character
		//halUsbSendChar ('S');
		WaitOn (qSyncDMA1);
		halUsbSendChar ('E');
#else
		// Read the memory block
		currentIdx = 0;
		currentPtr = memoryBegPtr;
		while ( currentPtr != memoryEndPtr )
		{
			blockToSend [currentIdx++] = *( (char*) currentPtr);
			currentPtr++;
		}
		//resultDMA = (INT8U) OSQPend(qSyncDMA1, 0, &err);
#endif
		
#if WO_DMA == 0 && DEBUG > 0
		if ( err != OS_ERR_NONE )
			DrawText(10, 70, "syncDMA", PIXEL_ON);
		else
			DrawText(10, 90, resultDMA, PIXEL_ON);
		// For now, only one bock is transferred
		if (DMA1SZ != SIZE_OF_AUDIO_BUFFER)
			DrawText(10, 110, "DMA error", PIXEL_ON);
#endif
		
#if WO_DMA == 0
		////
		// 3 - Reset/disable
		// For the DMA
		DMA1CTL &= ~(DMAEN + DMAIE);
#else		
		// Writes it on the USB line to communicate with the computer
		//printf("%s\n", blockToSend);
		//halUsbSendChar ('S');
		halUsbSendString2 ( blockToSend, blockSize );
		LED1_Blink();
		//halUsbSendChar ('E');
#endif
	}
}