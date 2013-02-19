#ifndef _BARGRAPH_H_
#define _BARGRAPH_H_

#define LED_ON 		0x01
#define LED_OFF		0x00

/* Light on/off the led represented by parameter segment. 
*  Possible values for state are LED_ON and LED_OFF
*/
void Bargraph_Print(unsigned char segment, unsigned char state);


/* Print
*
*/
void Barpraph_Rise_On(void);

/* Print
*
*/
void Bargraph_Rise(void);

/* Print
*
*/
void Bargraph_Fall(void);

#endif
