/*
Includes
*/

#include <includes.h>
#include <msp430x14x.h>
#include <io.h>
#include <signal.h>
#include <iomacros.h>
#include "task_root.h"

/*
Constants
*/

int global_pb_gd = 0;
#define  TASK_STK_SIZE  64       // Size of each task's stacks (# of OS_STK entries)

// TODO: Move all this to the includes

// Leds
#define STATUS_LED_ON      P2OUT &= ~BIT1   // STATUS_LED - P2.1
#define STATUS_LED_OFF     P2OUT |= BIT1    // STATUS_LED - P2.1   

// Port Output Register 'P1OUT, P2OUT':
#define P1OUT_INIT      0                   // Init Output data of port1
#define P2OUT_INIT      0                   // Init Output data of port2

// Port Direction Register 'P1DIR, P2DIR':
#define P1DIR_INIT      0xff                // Init of Port1 Data-Direction Reg (Out=1 / Inp=0)
#define P2DIR_INIT      0xff                // Init of Port2 Data-Direction Reg (Out=1 / Inp=0)

// Selection of Port or Module -Function on the Pins 'P1SEL, P2SEL'
#define P1SEL_INIT      0                   // P1-Modules:
#define P2SEL_INIT      0                   // P2-Modules:

// Interrupt capabilities of P1 and P2
#define P1IE_INIT       0                   // Interrupt Enable (0=dis 1=enabled)
#define P2IE_INIT       0                   // Interrupt Enable (0=dis 1=enabled)
#define P1IES_INIT      0                   // Interrupt Edge Select (0=pos 1=neg)
#define P2IES_INIT      0                   // Interrupt Edge Select (0=pos 1=neg)

#define WDTCTL_INIT     WDTPW|WDTHOLD

/*
Variables
*/

OS_STK   TaskStartStk[TASK_STK_SIZE];
OS_STK   TaskStartStk2[TASK_STK_SIZE];

/*
Main
*/

int main (void)
{
    int i,j;
	
    WDTCTL = WDTCTL_INIT;       // Init watchdog timer

    P6OUT  = P1OUT_INIT;        // Init output data of port1
    P6OUT  = P1OUT_INIT;        // Init output data of port2

    P6SEL  = P1SEL_INIT;        // Select port or module -function on port1
    P6SEL  = P2SEL_INIT;        // Select port or module -function on port2

    P6DIR  = P1DIR_INIT;        // Init port direction register of port1
    P6DIR  = P2DIR_INIT;        // Init port direction register of port2

    P1IES  = P1IES_INIT;        // Init port interrupts
    P2IES  = P2IES_INIT;
    P1IE   = P1IE_INIT;
    P2IE   = P2IE_INIT;

    // TODO: What is this, do we need it?
    // changement au vue de tournier , 3 lignes
    P2SEL = 0;
    P2OUT = 0;
    P2DIR = ~BIT0;      // Only P2.0 is input

    InitPorts();
	initDisplay();
	clearDisplay();
	printString(" OT");

    for (i = 0; i <= 5; i++) {   
        for (j = 1; j <= 10000; j++);
    }
    // fin du test du démmarage

    WDTCTL = WDTPW + WDTHOLD;   // Disable the watchdog timer
 
    // TIMERA Configuration     // Configure TIMERA for the system Tick source.
    TACTL    = TASSEL1 + TACLR; // Clear the Timer and set SMCLK as the source.
    TACTL   |= 0x00C0;          // Input divider is /8
    TACCTL0  = CCIE;            // Enable the TACCR0 interrupt
    TACCR0   = 2304;            // Load the TACCR0 register

    OSInit();                   // Initialize uC/OS-II
    OSTaskCreate(TaskStart, (void *)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
    OSStart();                  // Start multitasking

	return (0);
}
