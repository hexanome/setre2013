#include "record_task.h"
#include "hal_adc.h"
#include "hal_flash.h"
#include "rx_task.h"


/*******************************************************************************
* The Record Task.
* Responsible for:
*   Record audio from the microphone when needed.
*******************************************************************************/

OS_STK RecordTaskStack[RECORD_TASK_STACK_SIZE];

unsigned char audioBuffer1[SIZE_OF_AUDIO_BUFFER];
unsigned char audioBuffer2[SIZE_OF_AUDIO_BUFFER];
//unsigned char audioBuffer3[SIZE_OF_AUDIO_BUFFER];
//unsigned char audioBuffer4[SIZE_OF_AUDIO_BUFFER];
unsigned char *audioBuffers[] = { audioBuffer1, audioBuffer2/*, 
                                  audioBuffer3, audioBuffer4*/ };

unsigned char firstTime = 1;

void InitializeQSyncDMA()
{
	// Create the DMA Synchronization queue.
	qSyncDMA = OSQCreate(qSyncDMAData, QUEUE_SYNCDMA_LENGTH);
}

void RecordTask(void *args)
{
	unsigned char index = 0;
	
  while (1) {
    
    // Start recording
    WaitOn(qToggleRecord);
    
    setupRecord();
    

    // Record the user voice
    while ( !PeekOn(qToggleRecord) )
    {
      index = index % 2;
      // Set the destination of the DMA to the start address in RAM
      __data16_write_addr((unsigned short)DMA0DA_,
                          (unsigned long)audioBuffers[index]);
      // Set the size in byte of the "page"
      DMA0SZ = SIZE_OF_AUDIO_BUFFER;

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

			// Sending addresses for entire data

			if (firstTime)
			{
				firstTime = 0;
			}
			else
			{
				halUsbSendString2 ( audioBuffers[(index+1)%2], SIZE_OF_AUDIO_BUFFER );
			}
			WaitOn(qSyncDMA);
					
      index++;
    }
    stopRecord();
  }
}

/*******************************************************************************
* Method implementations.
*******************************************************************************/

static void setupRecord(void)
{
	AUDIO_PORT_DIR |= MIC_POWER_PIN;
  AUDIO_PORT_OUT |= MIC_POWER_PIN;
  AUDIO_PORT_OUT &= ~MIC_INPUT_PIN;
  AUDIO_PORT_SEL |= MIC_INPUT_PIN;
    
  // Use SMCLK as TIMER B source
  TBCTL = TBSSEL_2;
  // Initialize the TIMER B count
  TBR = 0;
  // Set TIMER B comparison value
  TBCCR0 = 3051; // 2047
  TBCCR1= 3051 - 100;
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