#include "record_task.h"
#include "hal_adc.h"
#include "hal_flash.h"

//OS_EVENT *qSyncDMA;

/*******************************************************************************
* The Record Task.
* Responsible for:
*   Record audio from the microphone when needed.
*******************************************************************************/

unsigned char audioBuffer1[SIZE_OF_AUDIO_BUFFER];
unsigned char audioBuffer2[SIZE_OF_AUDIO_BUFFER];
unsigned char audioBuffer3[SIZE_OF_AUDIO_BUFFER];
unsigned char audioBuffer4[SIZE_OF_AUDIO_BUFFER];
unsigned char * audioBuffers[] = {audioBuffer1, audioBuffer2, 
                                  audioBuffer3, audioBuffer4 };

void RecordTask(void *args)
{   
  unsigned char index= 0;
  
  // Create the DMA Synchronization queue.
  qSyncDMA = OSQCreate(&qSyncDMAData[0], QUEUE_SYNCDMA_LENGTH);
  
  while (1) {
    
    // Start recording
    WaitOn(qToggleRecord);
    
    setupRecord();
    
    // Erase in flash the audio sample previously recorded 
    /*flashEraseBank(AUDIO_MEM_START[0]);
    flashEraseBank(AUDIO_MEM_START[1]);
    flashEraseBank(AUDIO_MEM_START[2]);
    flashErase(AUDIO_MEM_START[3], AUDIO_MEM_START[4]); */

    // Record the user voice
    // TODO : loop on the previous segments ... 1,2,3,1,2,3 ...
    while (PeekOn(qToggleRecord) == OS_ERR_Q_EMPTY)
    {
      index = index % 4;
      // Set the destination of the DMA to the start address in RAM
      __data16_write_addr((unsigned short)DMA0DA_,
                          (unsigned long)audioBuffers[index]); 
      // Set the size in byte of the "page"
      DMA0SZ = SIZE_OF_AUDIO_BUFFER;      

      record();     
   
      // Sending addresses for entire data
      audioChunk chunk = {
        .startAddr = AUDIO_MEM_START[index], 
        .endAddr = audioBuffer1 + SIZE_OF_AUDIO_BUFFER
      };    
      
      OSQPost(qTxBuffer, (void *) &chunk);
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
  AUDIO_PORT_OUT |= MIC_POWER_PIN;
  AUDIO_PORT_OUT &= ~MIC_INPUT_PIN;
  AUDIO_PORT_SEL |= MIC_INPUT_PIN;
    
  // Use SMCLK as TIMER B source
  TBCTL = TBSSEL_2;
  // Initialize the TIMER B count
  TBR = 0;
  // Set TIMER B comparison value
  TBCCR0 = 2047;
  TBCCR1= 2047- 100;
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

static void record(void)
{    
  /*// Unlock the flash for write
  FCTL3 = FWKEY; 
  // Long word write
  FCTL1 = FWKEY + BLKWRT;   */                     
  
  DMA0CTL = 
    DMADT_0 + 
    DMASRTINCR_0 +
    DMADSTINCR_3 +  
    DMADSTBYTE + 
    DMASRCBYTE + 
    DMAEN +
    DMAIE;
  
  TBCCTL1 &= ~CCIFG;
  TBCTL |= MC0;                             
  
  // Enable interrupts 
  __bis_SR_register(GIE);
  
  WaitOn(qSyncDMA);
}

static void stopRecord(void)
{   
  TBCTL &= ~MC0;
  DMA0CTL &= ~( DMAEN + DMAIE);
  
  /*FCTL3 = FWKEY + LOCK;                     // Lock the flash from write */
  
  //TBCTL &= ~MC0;
  ADC12CTL0 &= ~( ADC12ENC + ADC12ON );  

  /*// Disable Flash write
  FCTL1 = FWKEY;
  // Lock Flash memory 
  FCTL3 = FWKEY + LOCK;*/

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

/*void flashEraseBank(INT16U Flash_ptr)
{
    FCTL3 = FWKEY;
    while (FCTL3 & BUSY) ;
    FCTL1 = FWKEY + MERAS;

    __data20_write_char(Flash_ptr, 0x00);      // Dummy write to start erase

    while (FCTL3 & BUSY) ;
    FCTL1 = FWKEY;
    FCTL3 = FWKEY +  LOCK;
}

void flashErase(INT16U Mem_start, INT16U Mem_end) 
{
    uint16_t Flash_ptr = Mem_start;        // Start of record memory array
    FCTL3 = FWKEY;                          // Unlock Flash memory for write
    do {
        if ((Flash_ptr & 0xFFFF) == 0x0000)    // Use bit 12 to toggle LED
            P1OUT ^= 0x01;
        FCTL1 = FWKEY + ERASE;
        
        __data20_write_char(Flash_ptr, 0x00);  // Dummy write to activate
        
        while (FCTL3 & BUSY) ;              // Segment erase
        Flash_ptr += 0x0200;                   // Point to next segment
    } while (Flash_ptr < Mem_end);
    FCTL1 = FWKEY;
    FCTL3 = FWKEY +  LOCK;
}

static INT32U getSegmentAddress(INT8U index)
{
  if (index >= NUMBER_OF_SEGMENTS)
  {
     return 0;
  }
  return Memstart + index * SIZE_OF_SEGMENTS
}*/

/*******************************************************************************
* Interrupt routines.
*******************************************************************************/
extern OS_EVENT* qSyncDMA1;
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{
	// Interrupt source from channel 0
	if ( DMAIV & 0x02 )
	{
		DMA0CTL &= ~ DMAIFG;
	  Trigger(qSyncDMA);
	}
	// Interrupt source from channel 1
	else if ( DMAIV & 0x04 )
	{
		DMA1CTL &= ~DMAIFG;
		Trigger(qSyncDMA1);
	}
}