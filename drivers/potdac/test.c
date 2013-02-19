#include <msp430x14x.h>
#include "platform.h"
#include "potdac.h"

int main(void)
{
	int station=1;
	int i=0;
	//WDTCTL = WDTPW + WDTHOLD;  	//Stop watchdog
	//Init platform ports	
	PF_Init_Ports();
	while (1)
	{		
		Delayx100us(10);
		if ((B1)==0)
		{
			for (i=0; i<10; i++)
				Delayx100us(10);

			station=station+1;
			if(station>5) station=1; 
			if (station==1) DAC_Print(240); // LYON 1ère
			if (station==2) DAC_Print(300); // NOSTALGIE ,  92.9 Mhz
			if (station==3) DAC_Print(400);
			if (station==4) DAC_Print(512);
			if (station==5) DAC_Print(800);
		}
	}
	return 0;
}
