#include <msp430x14x.h>
#include "platform.h"
#include "potdac.h"

int main(void)
{
	int station=1;
	int volume=0;
	int i=0;
	//WDTCTL = WDTPW + WDTHOLD;  	//Stop watchdog
	InitPorts();
	//initDisplay();
	//clearDisplay();
	/*
	printString(" ** OT-SETRE **");
		STATUS_LED_ON;  
		RISE_BAR();
		gotoSecondLine();
		printString(" ** INSA-2007 **");
		STATUS_LED_OFF;
		FALL_BAR();
		Delayx100us(10000);
	    clearDisplay();
		printString("  COMMANDE RADIO");
		gotoSecondLine();
		printString(">FIXER VOLUME");
		
		Delayx100us(10000);
		print_DAC(240); // LYON 1ère 240
		PRINT_POT (31,128);	 //cmd write , value
	*/
	while (1)
	{		
		Delayx100us(10);
		if ((B1)==0)
		{
			PRINT_POT (31,0);	 //cmd write , value
			STATUS_LED_OFF;
		}
		if ((B2)==0)
		{
			PRINT_POT (31,volume++);	 //cmd write , value
			RISE_BAR();
			STATUS_LED_ON;
		}
		if ((B3)==0)
		{
			PRINT_POT (31,volume--);	 //cmd write , value
			FALL_BAR();
			STATUS_LED_ON;
		}
		if ((B4)==0)
		{
			PRINT_BAR(7,1);
			for (i=0; i<10; i++) Delayx100us(10);
			station=station+1;
			if(station>5) station=1; 
			if (station==1) print_DAC(240); // LYON 1ère
			if (station==2) print_DAC(300); // NOSTALGIE ,  92.9 Mhz
			if (station==3) print_DAC(400);
			if (station==4) print_DAC(512);
			if (station==5) print_DAC(800);
			PRINT_BAR(8,0);
		}
	}
	return 0;
}
