#include <msp430x14x.h>

#include "platform.h"
#include "bargraph.h"

void InitPorts(void){
  P1SEL = 0;                                     //
  P1OUT = 0;                                     //
  P1DIR = BIT5 | BIT6;                           //enable only Relay outputs

  P2SEL = 0;
  P2OUT = 0;
  P2DIR = ~BIT0;                                // enable only P2.0 as output

  P3SEL |= BIT4 | BIT5;                         //enable UART0
  P3DIR |= BIT4;                                //enable TXD0 as output
  P3DIR &= ~BIT5;                               //enable RXD0 as input

  P4SEL = 0;
  P4OUT = 0;
  P4DIR = BIT2 | BIT3;                          //only buzzer pins are outputs

  P6SEL = 0;
  P6OUT = 0;
  P6DIR = 0xff;                                  // all output
}


int main(void)
{
	//WDTCTL = WDTPW + WDTHOLD;  	//Stop watchdog
	InitPorts();
	
	while (1)
	{		
		Delayx100us(10);
		if ((B1)==0)
		{
			STATUS_LED_OFF;
		}
		if ((B2)==0)
		{
			Bargraph_Rise();
			STATUS_LED_ON;
		}
		if ((B3)==0)
		{
			Bargraph_Fall();
			STATUS_LED_ON;
		}
		if ((B4)==0)
		{
			Bargraph_Print(7,1);
		}
	}
	return 0;
}



