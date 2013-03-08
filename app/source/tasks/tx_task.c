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
// A mettre sur Drive
typedef struct
{
	INT16U startAddr;
	INT16U endAddr;
} audioChunk;

/*******************************************************************************
* Private variables
*******************************************************************************/
// Each chunk has fixed size
static char blockSize = SIZEOF_BLOCK;

// Memory bounds
static INT16U memoryBegPtr = 0;
static INT16U memoryEndPtr = 0;

// Flash content read
static char blockToSend [SIZEOF_BLOCK] = {0};

/*******************************************************************************
* Private function
*******************************************************************************/
static void setTimerA (void)
{
	// Set output resolution (8 bit. Add 10 counts of headroom for loading TBCCR1
	TACCR0 = 255;
	TACCR4 = 255 >> 1;                      // Default output ~Vcc/2
	// Reset OUT1 on EQU1, set on EQU0. Load TBCCR1 when TBR counts to 0.
	TACCTL4 = OUTMOD_7 + CLLD_1;
}

/*******************************************************************************
* Task body function
*******************************************************************************/
void TxTask(void *args)
{
    // Message in the queue
    audioChunk* blockToRead = NULL;
    INT8U err;
    
    halUsbInit();
    
		setTimerA();
		
    while (1)
    {
        // Waits for a new block to read in the flash memory
        blockToRead = (audioChunk*) OSQPend(msgQBufferTx, 0, &err);
				// Keeps a local copy of memory addresses
        memoryBegPtr = blockToRead->startAddr;
				memoryEndPtr = blockToRead->endAddr;
				
#if DEBUG > 0
        if ( err != OS_ERR_NONE )
          printf("Problem to withdraw information in message queue\n");
#endif

        // Reads the flash in order to retrieve the string to be sent
				// 1 - Re-enable the timer A
				// Use SMCLK as Timer0_A source, enable overflow interrupt
				TACTL = TASSEL_2 + TAIE;
				// Start Timer_A in UP mode (counts up to TBCCR0)
				TACTL |= MC0;
        
				// 2 - Wait for the treatment's end
				// TODO
  
				// 3 - Reset timer A
  			TACTL = 0;
				
				// Writes it on the serial line to communicate with the computer
        halUsbSendString ( blockToSend, blockSize );

        // Delay 100ms.
        //OSTimeDlyHMSM(0, 0, 0, 100);
    }
}

// DMA can handle block up to 65535 bytes size
// DMADA => destinaton address
// DMASA => source address

/** NEED TIMERA VECTOR TO READ THE MEMORY USING DMA **/
#pragma vector=TIMERA1_VECTOR
__interrupt void TIMERA1_ISR(void)
{
	static int counter  = 0;
	switch (TAIV)
	{
		case 14:
			if (++counter == 8)
			{
				counter  = 0;
				TACCR4 = (*((unsigned char*)PlaybackPtr));
				PlaybackPtr++;
				if ((unsigned long) PlaybackPtr >= (lastAudioByte))
				{
					TACTL = 0;
					__bic_SR_register_on_exit(LPM0_bits);
				}
			}
		default: break;
	}
}