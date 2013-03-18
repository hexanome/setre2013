#include "record_task.h"
#include "hal_adc.h"
#include "hal_flash.h"


/*******************************************************************************
* The Record Task.
* Responsible for:
*   Record audio from the microphone when needed.
*******************************************************************************/

OS_STK RecordTaskStack[RECORD_TASK_STACK_SIZE];

unsigned char audioBuffer1[SIZE_OF_AUDIO_BUFFER];
unsigned char audioBuffer2[SIZE_OF_AUDIO_BUFFER];
unsigned char *audioBuffers[] = { audioBuffer1, audioBuffer2};

unsigned char firstTime = 1;

void InitializeQSyncDMA()
{
	// Create the DMA Synchronization queue.
	qSyncDMA = OSQCreate(qSyncDMAData, QUEUE_SYNCDMA_LENGTH);
}

void RecordTask(void *args)
{
	unsigned char index = 0;
	
  while (1) 
	{    
    // Start recording
    WaitOn(qToggleRecord);
    
    setupRecord();
    
    // Record the user voice
    while ( !PeekOn(qToggleRecord) )
    {
      index = index % NUMBER_OF_BUFFER;
      // Set the destination of the DMA to the start address in RAM
      __data16_write_addr((unsigned short)DMA0DA_,
                          (unsigned long)audioBuffers[index]);
      // Set the size in byte of the "page"
      DMA0SZ = SIZE_OF_AUDIO_BUFFER;

			// Configure DMA chan 0
			DMA0CTL = 
				DMADT_0 + 
				DMASRCINCR_0 +
				DMADSTINCR_3 +  
				DMADSTBYTE + 
				DMASRCBYTE + 
				DMAEN +
				DMAIE;

			TBCCTL1 &= ~CCIFG;
			TBCTL |= MC0;                             

			// Enable interrupts 
			__bis_SR_register(GIE);

			// Ignore the first step in the loop
			if (firstTime)
			{
				firstTime = 0;
			}
			else
			{
				// Send the buffer of the previous index
				halUsbSendStringINT16U ( audioBuffers[(index+1)%NUMBER_OF_BUFFER], SIZE_OF_AUDIO_BUFFER );
			}
			// Waiting DMA finishes its work
			WaitOn(qSyncDMA);
					
      index++;
    }
		
		// Send the the last buffer
// TODO : Size may differ from SIZE_OF_AUDIO_BUFFER
		halUsbSendStringINT16U ( audioBuffers[(index+1)%NUMBER_OF_BUFFER], SIZE_OF_AUDIO_BUFFER );
		
    stopRecord();
  }
}

/*******************************************************************************
* Method implementations.
*******************************************************************************/

static void setupRecord(void)
{
	// Setup mic
	AUDIO_PORT_DIR |= MIC_POWER_PIN;
  AUDIO_PORT_OUT |= MIC_POWER_PIN;
  AUDIO_PORT_OUT &= ~MIC_INPUT_PIN;
  AUDIO_PORT_SEL |= MIC_INPUT_PIN;
    
  // Use SMCLK as TIMER B source
  TBCTL = TBSSEL_2;
  // Initialize the TIMER B count
  TBR = 0;
  // Set TIMER B comparison value
  TBCCR0 = TIMERB_COMP_VALUE;
  TBCCR1= TIMERB_COMP_VALUE - 100;
  // Set ouput mode to reset/set
  TBCCTL1 = OUTMOD_7;   

  // Enabling MODOSC requests
  UCSCTL8 |= MODOSCREQEN;
  // Select 8-bit resolution
  ADC12CTL2 = ADC12RES_0;
  // Turn ON the ADC and set the sampling period to 16 ADC12CLK cycles
  ADC12CTL0 = ADC12ON + ADC12SHT02 ;  
  // SAMPCON signal is sourced from the sampling timer
  // Conversion sequence mode = Repeat-single-channel
  // Source clock selected : MCLK
  // Sample-and-hold source : Timer source
  ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2 + ADC12SSEL_2 + ADC12SHS_3;     
  // Input channel selected : A5, Indicates the last conversion in a sequence
  ADC12MCTL0 = MIC_INPUT_CHAN | ADC12EOS  ; 
  // Enable conversion
  ADC12CTL0 |= ADC12ENC;
  //Start conversion
  ADC12CTL0 |= ADC12SC;                     
  
  // ADC12IFGx triggers DMA0
  DMACTL0 = DMA0TSEL_24;
  // Src address = ADC12 module
  __data16_write_addr((unsigned short)DMA0SA_,(unsigned long)ADC12MEM0_);
}

static void stopRecord(void)
{   
  TBCTL &= ~MC0;
  DMA0CTL &= ~( DMAEN + DMAIE);
  
  //TBCTL &= ~MC0;
  ADC12CTL0 &= ~( ADC12ENC + ADC12ON );

  // Stop conversion immediately
  ADC12CTL1 &= ~ADC12CONSEQ_2;
  // Disable ADC12 conversion
  ADC12CTL0 &= ~ADC12ENC;
  // Switch off ADC12 & ref voltage 
  ADC12CTL0 = 0;

  // Disable Timer_B
  TBCTL = 0;                                

  // Turn of MIC 
  AUDIO_PORT_OUT &= ~MIC_POWER_PIN;           
  AUDIO_PORT_SEL &= ~MIC_INPUT_PIN;                            
}

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
void halUsbSendStringINT16U(unsigned char string[], INT16U length)
{
	INT16U i;

	for (i = 0; i < length; i++)
		halUsbSendChar(string[i]);
}

/*******************************************************************************
* Interrupt routines.
*******************************************************************************/
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{
	INT16U tmpDMAIV = DMAIV;
	// Interrupt source from channel 0
	if ( tmpDMAIV & 0x02 )
	{
		DMA0CTL &= ~ DMAIFG;
	  Trigger(qSyncDMA);
	}
}