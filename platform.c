#include "platform.h"

void Delay (unsigned int a)
{
  int k;
  for (k=0 ; k != a; ++k) {
    _NOP();
    _NOP();
    _NOP();
    _NOP();
  }
}

void Delayx100us(unsigned char b)
{
  int j;
  for (j=0; j!=b; ++j) Delay (_100us);
}

void PF_Init_Osc(void)
{
  WDTCTL = WDTPW | WDTHOLD;                      // stop watchdog timer

  BCSCTL1 |= XTS;                                // XT1 as high-frequency
  _BIC_SR(OSCOFF);                               // turn on XT1 oscillator

  do                                             // wait in loop until crystal is stable
    IFG1 &= ~OFIFG;
  while (IFG1 & OFIFG);

  BCSCTL1 |= DIVA0;                              // ACLK = XT1 / 2
  BCSCTL1 &= ~DIVA1;

  IE1 &= ~WDTIE;                                 // disable WDT int.
  IFG1 &= ~WDTIFG;                               // clear WDT int. flag

  WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL | WDTIS1; // use WDT as timer, flag each
                                                           // 512 pulses from ACLK

  while (!(IFG1 & WDTIFG));                      // count 1024 pulses from XT1 (until XT1's
                                                 // amplitude is OK)

  IFG1 &= ~OFIFG;                                // clear osc. fault int. flag
  BCSCTL2 |= SELM0 | SELM1;                      // set XT1 as MCLK
}

void PF_Init_Ports(void)
{
  P1SEL = 0;                                     //
  P1OUT = 0;                                     //
  P1DIR = BIT5 | BIT6;                           //enable only Relay outputs

  P2SEL = 0;
  P2OUT = 0;
  P2DIR = ~BIT0;                                //only P2.0 is input

  P3SEL |= BIT4 | BIT5;                         //enable UART0
  P3DIR |= BIT4;                                //enable TXD0 as output
  P3DIR &= ~BIT5;                               //enable RXD0 as input

  P4SEL = 0;
  P4OUT = 0;
  P4DIR = BIT2 | BIT3;                          //only buzzer pins are outputs

  //P6SEL = 0x80;
  //P6OUT = 0;
 // P6DIR = 0x00;                                  // all output
}
