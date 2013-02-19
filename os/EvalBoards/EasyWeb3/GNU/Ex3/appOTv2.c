/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                              (c) Copyright 2002, Micrium, Inc., Weston, FL
*                                           All Rights Reserved
*
*                                                TI MSP430
*********************************************************************************************************
*/

int global_pb_gd=0;
#include <includes.h>
#include <msp430x14x.h> // a voir si utile
#include "Display.h"
#include <io.h>
#include <signal.h>
#include <iomacros.h>
/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                  64       /* Size of each task's stacks (# of OS_STK entries)   */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK   TaskStartStk[TASK_STK_SIZE];
OS_STK   TaskStartStk2[TASK_STK_SIZE];

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void   TaskStart(void *data);           /* Function prototypes of Startup task                */

void   TaskStart2(void *data);           /* test task             */
void   Enable_XT2(void);                /* Enable XT2 and use it as the clock source          */        

/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

int  main (void)
{int i,j;
// début du test du démmarage

#define          STATUS_LED_ON      P2OUT &= ~BIT1    //STATUS_LED - P2.1
#define          STATUS_LED_OFF     P2OUT |= BIT1     //STATUS_LED - P2.1	

//Port Output Register 'P1OUT, P2OUT':
#define P1OUT_INIT      0                       // Init Output data of port1
#define P2OUT_INIT      0                       // Init Output data of port2

//Port Direction Register 'P1DIR, P2DIR':
#define P1DIR_INIT      0xff                    // Init of Port1 Data-Direction Reg (Out=1 / Inp=0)
#define P2DIR_INIT      0xff                    // Init of Port2 Data-Direction Reg (Out=1 / Inp=0)

//Selection of Port or Module -Function on the Pins 'P1SEL, P2SEL'
#define P1SEL_INIT      0                       // P1-Modules:
#define P2SEL_INIT      0                       // P2-Modules:


//Interrupt capabilities of P1 and P2
#define P1IE_INIT       0                       // Interrupt Enable (0=dis 1=enabled)
#define P2IE_INIT       0                       // Interrupt Enable (0=dis 1=enabled)
#define P1IES_INIT      0                       // Interrupt Edge Select (0=pos 1=neg)
#define P2IES_INIT      0                       // Interrupt Edge Select (0=pos 1=neg)

#define WDTCTL_INIT     WDTPW|WDTHOLD



	
 WDTCTL = WDTCTL_INIT;               //Init watchdog timer

    P6OUT  = P1OUT_INIT;                //Init output data of port1
    P6OUT  = P1OUT_INIT;                //Init output data of port2

    P6SEL  = P1SEL_INIT;                //Select port or module -function on port1
    P6SEL  = P2SEL_INIT;                //Select port or module -function on port2

    P6DIR  = P1DIR_INIT;                //Init port direction register of port1
    P6DIR  = P2DIR_INIT;                //Init port direction register of port2

    P1IES  = P1IES_INIT;                //init port interrupts
    P2IES  = P2IES_INIT;
    P1IE   = P1IE_INIT;
    P2IE   = P2IE_INIT;
// changement au vue de tournier , 3 lignes
  P2SEL = 0;
  P2OUT = 0;
  P2DIR = ~BIT0;                                //only P2.0 is input

  

InitPorts();
	initDisplay();
	clearDisplay();
	printString(" OT");


     for(i=0;i<=5;i++){   
            //P6OUT = 248>>i;
            for(j=1;j<=10000;j++);
     }

     

// fin du test du démmarage


	
    WDTCTL = WDTPW + WDTHOLD;           /* Disable the watchdog timer   */

  /*  P6SEL = 0x00;                       /* Port1 is set to GPIO         */
  /*   P6DIR = 0x01;                       /* P1.0 is the only output.     */
  /*  P6OUT = 0x00;                       /* P1.0 initially low.          */
 
    // TIMERA Configuration             /* Configure TIMERA for the system Tick source. */
    //
    TACTL    = TASSEL1 + TACLR;         /* Clear the Timer and set SMCLK as the source. */
    TACTL   |= 0x00C0;                  /* Input divider is /8.  */
    TACCTL0  = CCIE;                    /* Enable the TACCR0 interrupt. */
    TACCR0   = 2304;                   /* Load the TACCR0 register.    */  

    OSInit();                                              /* Initialize uC/OS-II                      */
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
	OSTaskCreate(TaskStart2, (void *)5, &TaskStartStk2[TASK_STK_SIZE - 1], 5);
  /*  P6OUT = 0;*/
    OSStart();                                             /* Start multitasking                       */

	return(0);
}

/*
*********************************************************************************************************
*                                            STARTUP TASK
*********************************************************************************************************
*/

void  TaskStart (void *pdata)
{ int i,j,k;
    pdata  = pdata;         /* Prevent compiler warning                 */

    TACTL |= MC1;           /* Start the Timer in Continuous mode. */

    while (1) 
    { for(i=0;i<=5;i++){   
            //P6OUT = 248>>i;
            for(j=1;j<10000;j++);
     }
        /* Task specific code */

      //  P6OUT ^= 0x01;      /* Toggle the port pin to show signs of life.   
    k++;
	STATUS_LED_ON;    
    OSTimeDly(OS_TICKS_PER_SEC); 
	printDecimal(1); /* Delay for a bit. */    
	STATUS_LED_OFF;
    }
}


void  TaskStart2 (void *pdata)
{ int i,j,k;
    pdata  = pdata;         /* Prevent compiler warning                 */

    while (1) 
    { for(i=0;i<=5;i++){   
            //P6OUT = 248>>i;
            for(j=1;j<10000;j++);
     }
        /* Task specific code */

      //  P6OUT ^= 0x01;      /* Toggle the port pin to show signs of life.   
    k++;
 
    OSTimeDly(OS_TICKS_PER_SEC); 
	printDecimal(2); /* Delay for a bit. */    

    }
}
/*
void Enable_XT2(void)
{
    int i;
    volatile int j;

    i=1;
    while(i)
    {
        _BIS_SR(OSCOFF);
        BCSCTL1 = 0;            // XT2 is On 
        IFG1 &= ~OFIFG;         // Clear the Oscillator Fault interrupt flag. 

        for(j=0;j<1000;j++);    // Wait for a little bit.  

        if(!(IFG1 & OFIFG))     // If OFIFG remained cleared we are ready to go. 
        {                       // Otherwise repeat the process until it stays cleared. 
            i = 0;
        }
    }

    BCSCTL2 = 0x88;             // Select XT2CLK for MCLK and SMCLK 
	}
*/
 

