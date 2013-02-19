#include <msp430x14x.h>
#include "Display.h"


int DelayCounter;

#define MAX (PERIOD-20) << 2
#define MIN (20) << 2
#define STEPSIZE 1
#define UP 1
#define DOWN 0
int Count;
int Direction;
int main(void)
{
	
	InitPorts();
	initDisplay();
	clearDisplay();
	printString(" OT");
	while (1)
	{
	STATUS_LED_ON;
	//BUZ1_ON;
	Delayx100us(1000);
	//BUZ1_OFF;
	printString("     FABv1");
	//BUZ1_ON;
	STATUS_LED_OFF;
	Delayx100us(1000);
	//BUZ1_OFF;
	
	}
}



