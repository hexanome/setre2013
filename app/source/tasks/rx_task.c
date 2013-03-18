#include "rx_task.h"

/************************************************************************/
void InitializeQRxBuffer(void)
{
	qRxBuffer = OSQCreate (bufferRx, QUEUE_RX_BUFFER_LENGTH);
}


/*******************************************************************************
* The Reception Task.
* Responsible for:
*   Receiving text packet received on the serial USB link from the computer
*******************************************************************************/
char textToRead [TEXT_SIZE] = {0};

void RxTask(void *args)
{
	INT8U err;
	INT8U receivedChar;
	INT8U aLength = 0;
	
	while (1)
	{
		// (Re-)Init the string's length
		aLength = 0;
		// Read the text
		do
		{
			// Wait for the message queue to be filled by the interrupt routine
			receivedChar = (INT8U) OSQPend(qRxBuffer, 0, &err);
			
			textToRead[aLength++] = receivedChar;
		}
		while ( receivedChar != '\0' );
		
		// Toggle next state
		SetNextState();
	}
}


/************************************************************************/
// Transmit and Receive interrupts are handled by the same interrupt routine vector
// One test on different bits of the UCA1IFG register will identify the interruption source
// (34.4.13 doc p921)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
	INT16U tmpUCA1IFG = UCA1IFG;
	// RX IT => UCA1IFG[0] (e.g. UCRXIFG == 1)
	if (tmpUCA1IFG & UCRXIFG)
	{
		OSQPost(qRxBuffer, (void*) UCA1RXBUF);
	}
	// TX IT => UCA1IFG[1] (e.g. UCTXIFG == 1)
	else if (tmpUCA1IFG & UCTXIFG)
	{
// TODO: If the writing were to be done using interrupt instead of pooling
	}
	__bic_SR_register_on_exit(LPM3_bits);
}