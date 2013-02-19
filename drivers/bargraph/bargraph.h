#ifndef _BARGRAPH_H_
#define _BARGRAPH_H_

#define LED_ON 		0x01
#define LED_OFF		0x00

/* Light on/off the led represented by segment parameter (8 segments in total).
*  Possible values for state are LED_ON and LED_OFF
*/
void Bargraph_Print(unsigned char segment, unsigned char state);


/* Light on all the segments one after the other (with 1ms delay between each one). 
*
*/
void Bargraph_Rise_On(void);

/* Same as Rise_On, but with only one segment switched on at a time.
*
*/
void Bargraph_Rise(void);

/* Same as Rise, but starting from the last segment.
*
*/
void Bargraph_Fall(void);

#endif
