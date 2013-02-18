#include <msp430x14x.h>

#include "platform.h"
#include "spi/spi.h"
#include "potdac.h"

void DAC_Print(int nb)
{
	char bin[11];

	SEL_OFF;		// select DAC path
	CS_ON;
	//delay (20,1);	// (10,1) ->82µs
	Delayx100us(1);
	CS_OFF;
	
	if (nb>255)
		nb=255;

	if (nb<0)
		nb=0;
	
	nb <<= 2;		//shift nb left by two in order to add 2 dummy bits

	/* Send the number to the DAC */
	SPI_Send_Word(13, nb, NULL);
		
	CS_ON;	
	delay (200,10);
	SEL_OFF;		// deselect POT path


}
void POT_Print(int cmd, int nb)
{
	char bin[8];
	
	SEL_ON;				// select POT-NUM path
	CS_ON;
	//delay (20,1);	// (10,1) ->82us
	Delayx100us(1);
	CS_OFF;
	
	// cmde byte for POT :8 higher 16bits  
	// 0b 0001.0001 > write data on POT0
	// 0b 0010.0001 > shutdown POT0
	// 0b 0011.000x > no command executed
	if (cmd>255)
		cmd=255;

	if (cmd<0)
		cmd=0;
	
	/* Send the command over the SPI line to the POT */
	SPI_Send_Word(8, cmd, NULL);
	
	if (nb>255)
		nb=255;

	if (nb<0)
		nb=0;
	
	/* Send the number to the POT */
	SPI_Send_Word(8, nb, NULL);
		
	CS_ON;	
	delay (200,10);
	SEL_OFF;		// deselect POT path
}		


