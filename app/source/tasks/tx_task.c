#include "tx_task.h"
#include "rx_task.h"

// Init the usb connection in the main task

// While developping the application, error code have to be checked
#define DEBUG 1

// Make sure the timer code is not here (removed for now)
#undef TIM_

/*******************************************************************************
* The Transmission Task.
* Responsible for:
*   Transmitting voice packet on the serial USB link to make them analyze by the computer
*******************************************************************************/

OS_EVENT* qSyncDMA1;

/*******************************************************************************
* Private variables
*******************************************************************************/
// Each chunk has fixed size
//static char blockSize = BUFFER_SIZE;

// Memory bounds
static INT16U memoryBegPtr = 0;
static INT16U memoryEndPtr = 0;

// Flash content read
//static char blockToSend [BUFFER_SIZE] = {0};

/*******************************************************************************
* Private function
*******************************************************************************/
#if TIM_
static void setupTimerA (void)
{
	// Set output resolution (8 bit. Add 10 counts of headroom for loading TBCCR1
	TA0CCR0 = 255;
	TA0CCR4 = 255 >> 1;                      // Default output ~Vcc/2
	// Reset OUT1 on EQU1, set on EQU0. Load TBCCR1 when TBR counts to 0.
	TA0CCTL4 = OUTMOD_7 + CLLD_1;
}
#endif

// We'll use the DMA to transfer directly from the flash to the transmit register
static void setupDMA (void)
{
	// DMA channel 1 trigger is set to USCIA0 transmit event (may be useless/false)
	DMACTL1 = DMA1TSEL_17;
	
	// The destination of the DMA's channel 1 is the TX register of the USB
	// DMAxDA => destinaton address (11.3.7 doc p402)
	__data16_write_addr((unsigned long)&DMA1DA & 0xffff, (unsigned long)&UCA1TXBUF);
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
    
	halUsbInit();

#if TIM_
	setupTimerA();
#endif

	setupDMA();
		
	while (1)
	{
		// Waits for a new block to read in the flash memory
		blockToRead = (audioChunk*) OSQPend(qTxBuffer, 0, &err);
		// Keeps a local copy of memory addresses
		memoryBegPtr = blockToRead->startAddr;
		memoryEndPtr = blockToRead->endAddr;
				
#if DEBUG > 0
		if ( err != OS_ERR_NONE )
			printf("Problem to withdraw information in msgQBufferTx\n");
		else
			printf("Ready to read from 0x%x to 0x%x\n", memoryBegPtr, memoryEndPtr);
#endif

		// Reads the flash in order to retrieve the string to be sent
#if TIM_
		////
		// 1 - Re-enable the timer A
		// Use SMCLK as Timer0_A source, enable overflow interrupt
		TA0CTL = TASSEL_2 + TAIE;
		// Start Timer_A in UP mode (counts up to TBCCR0)
		TA0CTL |= MC0;
#endif
		
		////
		// 2 - Set up the DMA controller
		// The source of the DMA's channel 1 is the pointer on the flash stored in the flash
		// DMAxSA => source address (11.3.8 doc p403)
		__data16_write_addr((unsigned long)&DMA1SA & 0xffff, memoryBegPtr);
			
		// Define the amount of information to be transferred (counts downwards to 0)
		// (11.3.9 doc p404)
		DMA1SZ = SIZE_OF_AUDIO_BUFFER;
		
		// Enable Long-Word write, all 32 bits will be written once
		// 4 bytes are loaded
		DMA1CTL = DMADSTINCR_3 + DMAEN + DMADSTBYTE +  DMASRCBYTE + DMAIE;
		
		////
		// 3 - Wait for the treatment's end
		resultDMA = (INT8U) OSQPend(qSyncDMA1, 0, &err);

#if DEBUG > 0
		if ( err != OS_ERR_NONE )
			printf("Problem to withdraw information in syncDMA\n");
		else
			printf("DMA sent back %c\n", resultDMA);
		// For now, only one bock is transferred
		if (DMA1SZ != BUFFER_SIZE)
			printf("DMA transfert error\n");
#endif
		
		////
		// 4 - Reset/disable
		// For the DMA
		DMA1CTL &= ~(DMAEN + DMAIE);

#if TIM_
		// For the TimerA_0
		TA0CTL = 0;
#endif
		
		// Writes it on the USB line to communicate with the computer
		//halUsbSendString ( blockToSend, blockSize );
	}
}

#if TIM_
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMERA1_ISR(void)
{
	static int counter  = 0;
	switch (TA0IV)
	{
		case 14:
			if (++counter == 8)
			{
				counter  = 0;
				TA0CCR4 = (*((unsigned char*)PlaybackPtr));
				PlaybackPtr++;
				if ((unsigned long) PlaybackPtr >= (lastAudioByte))
				{
					TA0CTL = 0;
					__bic_SR_register_on_exit(LPM0_bits);
				}
			}
		default: break;
	}
}
#endif