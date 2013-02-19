#include <msp430x14x.h>

#include "platform.h"
#include "spi/spi.h"
#include "bargraph.h"


#define WDTCTL_INIT     WDTPW|WDTHOLD

#define PERIOD 1000
#define HIGH_TIME 50
#define bitset(var,bitno) ((var) |= 1 << (bitno))
#define bitclr(var,bitno) ((var) &= ~(1 << (bitno)))



#define		DISP_ON			0x0c	        //LCD control constants
#define		DISP_OFF		0x08	        //
#define		CLR_DISP		0x01    	//
#define		CUR_HOME		0x02	        //
#define		ENTRY_INC		0x06            //
#define		DD_RAM_ADDR		0x80	        //
#define		DD_RAM_ADDR2		0xc0	        //
#define		DD_RAM_ADDR3		0x28	        //
#define		CG_RAM_ADDR		0x40	        //

// Necessaire a cause d'un bug d'insight
int TMP1,TMP2;
unsigned int val=1;


/*static void delay(unsigned int minor, unsigned int major)
{
  int i,j;
  for(j=0; j < major; j++)
    {
      for (i = 0; i<minor; i++) 
	   {
	     nop();
	     nop();
	   }
    }
}
*/

void Bargraph_Print(unsigned char segment, unsigned char state)
{
	SEL_ON;				//LE signal	
	if ((segment>7) || (segment<0))
	{	segment=0;
	}
	
	if (state == LED_ON)
	{	D_ON;
	} 
	else
	{	D_OFF;
	}

	switch (segment)
	{
		case (0): 
		{	S0_OFF;		//line selection 1/8 (msb) S2 S1 S0 (lsb)
			S1_OFF;
			S2_OFF;
			break;
		}
		case (1): 
		{	S0_ON;		//line selection 1/8 (msb) S2 S1 S0 (lsb)
			S1_OFF;
			S2_OFF;
			break;
		}
		case (2): 
		{	S0_OFF;		//line selection 1/8 (msb) S2 S1 S0 (lsb)
			S1_ON;
			S2_OFF;
			break;
		}
		case (3): 
		{	S0_ON;		//line selection 1/8 (msb) S2 S1 S0 (lsb)
			S1_ON;
			S2_OFF;
			break;
		}
		case (4): 
		{	S0_OFF;		//line selection 1/8 (msb) S2 S1 S0 (lsb)
			S1_OFF;
			S2_ON;
			break;
		}
		case (5): 
		{	S0_ON;		//line selection 1/8 (msb) S2 S1 S0 (lsb)
			S1_OFF;
			S2_ON;
			break;
		}
		case (6): 
		{	S0_OFF;		//line selection 1/8 (msb) S2 S1 S0 (lsb)
			S1_ON;
			S2_ON;
			break;
		}
		case (7): 
		{	S0_ON;		//line selection 1/8 (msb) S2 S1 S0 (lsb)
			S1_ON;
			S2_ON;
			break;
		}
	
		default: break;
	}
	SEL_OFF;	//LATCH
	SEL_ON;	
}

void Barpraph_Rise_On(void)
{
	unsigned char i=0;
	
	for (i=0; i<8 ;i++)
	{
		Bargraph_Print(i,LED_ON);
		Delayx100us(10);
	}
}

void Bargraph_Rise(void)
{
	unsigned char i=0;
	
	for (i=0; i<8 ;i++)
	{
		Bargraph_Print(i,LED_ON);
		Delayx100us(10);
		Bargraph_Print(i,LED_OFF);
	}
}

void Bargraph_Fall(void)
{
	unsigned char i=0;

	for (i=7; i>0 ;i--)
	{
		Bargraph_Print(i,LED_ON);
		Delayx100us(10);
		Bargraph_Print(i,LED_OFF);
	}
}


