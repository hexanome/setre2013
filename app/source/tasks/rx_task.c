#include "rx_task.h"

/************************************************************************/
/*-------------------------------------------------------------
 *                  USB 
 * ------------------------------------------------------------*/

/**********************************************************************//**
* @brief  Initializes the serial communications peripheral and GPIO ports
*         to communicate with the TUSB3410.
*
* @param  none
*
* @return none
**************************************************************************/

void halUsbInit(void)
{
	// The USB module is plugged on port 5 :
	//  - .6 for TXD
	//  - .7 for RXD
	// and we want to activate this features instead of GPIO
	USB_PORT_SEL |= USB_PIN_RXD + USB_PIN_TXD;
  // P5DIR.6 is set to 1, meaning the TXD is output
  USB_PORT_DIR |= USB_PIN_TXD;
	// P5DIR.7 is set to 0, meaning the RXD is input
	USB_PORT_DIR &= ~USB_PIN_RXD;

	// Reset State
	UCA1CTL1 |= UCSWRST;
	UCA1CTL0 = UCMODE_0;

	// 8bit char
	UCA1CTL0 &= ~UC7BIT;
	UCA1CTL1 |= UCSSEL_2;
	// With a frequency of 16 MHz, UCA1BR register must be filled with:
	// 16MHz/57600 = 278 = 256 + 22
	// (57600 is the baud rate we should use)
	// Meaning 1 in the high-order register and 22 in the low-order
	// With a frequency of 25 MHz, this register must be filled with:
	// 25MHz/57600 = 434 = 256 + 178
	// Meaning 1 in HO register and 178 in LO
	// With a frequency of 25MHz and a baud-rate of 115200, this register must be filled with:
	// 25MHz/115200 = 217
	// Meaning 0 in HO register and 217 in LO
	UCA1BR0 = 108;//217; //178 //22;
	UCA1BR1 = 0; //1
	// Select modulation stage and disable oversampling (34.4.5 doc p916)
	UCA1MCTL = 0xE;
	// Disable software reset (34.4.2 doc p915)
	UCA1CTL1 &= ~UCSWRST;
	// Enables interrupt register on the USCI peripheral (34.4.12 doc p921)
	UCA1IE |= UCRXIE;
	
	// Enable Interrupts
	__bis_SR_register(GIE);
}

/**********************************************************************//**
* @brief  Disables the serial communications peripheral and clears the GPIO
*         settings used to communicate with the TUSB3410.
*
* @param  none
*
* @return none
**************************************************************************/

void halUsbShutDown(void)
{
	// Disable RX interrupts
	UCA1IE &= ~UCRXIE;
	//Reset State
	UCA1CTL1 = UCSWRST;                
	USB_PORT_SEL &= ~(USB_PIN_RXD + USB_PIN_TXD);
	USB_PORT_DIR |= USB_PIN_TXD;
	USB_PORT_DIR |= USB_PIN_RXD;
	USB_PORT_OUT &= ~(USB_PIN_TXD + USB_PIN_RXD);
}

/**********************************************************************//**
* @brief  Sends a character over UART to the TUSB3410.
*
* @param  character The character to be sent.
*
* @return none
**************************************************************************/

// TODO: Maybe using interrupts
void halUsbSendChar(char character)
{
	// Waits until the transitting buffer is empty (34.4.13 doc p921)
	while (!(UCA1IFG & UCTXIFG)) ;
	// Fills in the transmitting buffer
	UCA1TXBUF = character;
}

/**********************************************************************//**
* @brief  Sends a string of characters to the TUSB3410
*
* @param  string[] The array of characters to be transmit to the TUSB3410.
*
* @param  length   The length of the string.
*
* @return none
**************************************************************************/

void halUsbSendString2(unsigned char string[], INT16U length)
{
	INT16U i;

	for (i = 0; i < length; i++)
		halUsbSendChar(string[i]);
}

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
		
		//halUsbReceiveString (textToRead, &aLength);
		
		// Post the recognized text to be displayed on the LCD screen
		//err = OSQPost (qLcdRefresh, textToRead);
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