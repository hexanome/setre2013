/* Emulated SPI
*
*/

#include "platform.h"
#include "spi.h"

/* Init function fo the SPI peripheral
*  NOTE: Not implemented for the Emulated SPI
*/
void SPI_Init()
{
	return;
}

/* Send function: a word "word" of length "wlen" is sent
*  along with the CLK on the MOSI pin (MSB first).
*/
int SPI_Send_Word(unsigned int wlen, unsigned int txWord, unsigned int *rxWord)
{
	unsigned int i, mask=1;
	
	/* Compute the mask for the MSB*/
	for(i=0; i<wlen-1; i++)
		mask <<= 1;

	for(i=0; i<wlen;i++)
	{
		if (txWord & mask) 
		{ 	/* If the i-th bit from the MSB is 1 */
			DIN_ON;		// DIN=0
			SCLK_ON;	//SCLK=1
			SCLK_OFF;	// SCLK=0
			if (rxWord)
				*rxWord |= mask; //FIXME
		}
		else
		{	/* If zero instead */
			DIN_OFF;	// DIN=1
			SCLK_ON;	//SCLK=1
			SCLK_OFF;	// SCLK=0
			if (rxWord)
				*rxWord &= ~mask; //FIXME
		}
		/* Shift the mask right by one */
		mask >>= 1;
	}
	return i;
}

