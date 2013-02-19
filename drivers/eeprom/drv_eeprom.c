//==================================//
// driver i2c pour eeprom
//
// cible -> MSP430x149 (MSP430 sans module I2C)
//
//
//===================================//

#include "drv_eeprom.h"
#include <includes.h>

//P4.1  SCL
//P4.0  SDA

//* Plateform
#define SDA_1       P4OUT |=  BIT0              //SDA = 1
#define SDA_0       P4OUT &= ~BIT0              //SDA = 0
#define SCL_1       P4OUT |=  BIT1              //SCL = 1
#define SCL_0       P4OUT &= ~BIT1              //SCL = 0
#define DIR_IN      P4DIR &= ~BIT0 		// SDA as input
#define DIR_OUT     P4DIR |=  BIT0              // SDA as output
#define SDA_IN      (P4IN & 0x01)        	// Read SDA 
//*/

// P4.0 and P4.1 are set to interact with peripheral (bit = 1)
#define SEL_SDA		P4SEL |= BIT0
#define SEL_SCL		P4SEL |= BIT1
//
#define I2C_ADDR_READ	0xA1
//
#define I2C_ADDR_WRITE	0xA0
// The EEPROM model works at a maximum frequency of 400KHz, which means 1/400KHz = 
#define TIMING		4



//////////////////////////////////
// i2c frequency calibration with active delays for <a> us
static void delay_iic(unsigned int a)
{
  unsigned int k;
  for (k=0 ; k < a; ++k) {
    _NOP();
    _NOP();
    _NOP();
    _NOP();
  }
}

// <b> times 100us delay
static void delayx100us_iic(unsigned int b)
{
	  unsigned int j;
	  for (j=0; j!=b; ++j) delay_iic(100);
}

//////////////////////////////////

// From the 3rd page of the documentation concerning the EEPROM, there is a special boot sequence we will decompose
// using the following static functions
//
////
// For the first parameter
//
static void init(void)
{
	// SCL = 1 et SDA  = 1
	SCL_1;
	// 4us delay
	delay_iic(TIMING);
	SDA_1;
	// 4us delay
	delay_iic(TIMING);
}

// Cf doc slaa208 p3
static void start(void)
{
	SDA_1;
	//delay_iic(TIMING);
	SCL_1;
	delay_iic(TIMING);
	SDA_0;
	delay_iic(TIMING);
	SCL_0;
	delay_iic(TIMING);
}

// Cf doc slaa208 p3
static void stop(void)
{
	SDA_0;
	delay_iic(TIMING);
	SCL_1;
	delay_iic(TIMING);
	SDA_1;
	delay_iic(TIMING);
}


static void write_byte(unsigned char write_data)
{
	unsigned char i;
	for (i=0; i<8; i++)
	{
		SCL_0;
		delay_iic(TIMING);
		
		//recopie du bit de poids fort (au bit de poid faible) de write_data dans SDA
		if (((write_data >> 7) & 0x01) == 0x01)
		{
			SDA_1;
		}
		else
		{
			SDA_0;
		}
		delay_iic(TIMING);
		SCL_1;
		write_data = write_data << 1;
		delay_iic(TIMING);
	}
	SCL_0;
	SDA_1;
	delay_iic(TIMING);
	delay_iic(TIMING);
}

static unsigned char read_byte(void)
{
	unsigned char i;
	unsigned char TempBit  = 0; // Variables tempon
	unsigned char TempData = 0;
	SCL_0;
	delay_iic(TIMING);
	SDA_1;
	for (i=0; i<8; i++)
	{
		delay_iic(TIMING);
		SCL_1;
		delay_iic(TIMING);
		DIR_IN;
		if (SDA_IN == 0x01 /*sda==1*/)
		{
			TempBit = 1;
		}
		else
		{
			TempBit = 0;
		}
		DIR_OUT;
		TempData = (TempData << 1) | TempBit;
		SCL_0;
	}
	delay_iic(TIMING);
	return(TempData);
}

static int receive_ack(void) 
{
	unsigned char i=0;
	unsigned char flag = 0;
	SCL_1;
	delay_iic(TIMING);
	// HZ on SDA
	DIR_IN;
	while ((SDA_IN == 0x01) && (i < 255))
	{
		i++; // pour ne pas �tre bloquant en cas de NACK
	}
	if(SDA_IN == 0x00)
	{
		flag = 1;
	}
	DIR_OUT;
	SCL_0;
	delay_iic(TIMING);

	if(flag == 1)
	{
		return(0);	// ACK
	}
	else
	{
		return(1);	//NACK
	}
}

static void acknowledge(void)
{
	SCL_0;
	delay_iic(TIMING);
	DIR_OUT;
	SDA_0;
	SCL_1;
	delay_iic(TIMING);
	SCL_0;
}

// Lecture d'un octet a une adresse donnee
unsigned char eeprom_random_read(unsigned int address) 
{
	unsigned char data = 0;
	unsigned char addr_hi  = 0;
	unsigned char addr_lo = 0;

	addr_hi = (unsigned char)address & 0xff;	// get LSB 
	addr_lo = (unsigned char)address>>8;		// get MSB
	
	OS_ENTER_CRITICAL();				// Disable Interrupts

	start();					// start condition on bus
	write_byte(I2C_ADDR_WRITE); 			// send ctrl byte (write mode)
	receive_ack();					// wait for ACK

	write_byte(addr_hi);				// send addr MSB
	receive_ack();					// wait for ACK
	write_byte(addr_lo);				// send addr LSB
	receive_ack();					// wait for ACK

	// Traditionnal read
	start();					// send start condition
	write_byte(I2C_ADDR_READ);			// send ctrl byte (read mode)	
	receive_ack();					// wait for ACK
	data  = read_byte();
	
	delay_iic(15);					// NACK 
	stop();						// stop condition
	
	OS_EXIT_CRITICAL();				// Disable Interrupts
 
	return(data);
}

// Lecture d'un octet au pointeur EEPROM 
unsigned char eeprom_current_read(void)
{
	unsigned char data = 0;
	//unsigned char i = 0;
	
	OS_ENTER_CRITICAL();			// Disable Interrupts
	
	start();				// start condition on bus
	write_byte(I2C_ADDR_READ); 		// send ctrl byte (write mode)
	receive_ack();				// wait for ACK
	data  = read_byte();
	// 15us delay to simulate NACK (where did level goes to 1?)
	delay_iic(15);				// NACK
	stop();					// stop condition
	
	OS_EXIT_CRITICAL();			// Enable Interrupts
	
	return(data);
}

// Ecriture d'un octet a l'adesse specifiee
void eeprom_byte_write(	unsigned int address, unsigned char data)
{
	
	unsigned char addr_hi  = 0;
	unsigned char addr_lo = 0;
	addr_lo  = (unsigned char)address & 0xFF;	// get LSB
	addr_hi = (unsigned char)(address >> 8);	// get MSB
	
	OS_ENTER_CRITICAL();
	
	start();					// start condition
	write_byte(I2C_ADDR_WRITE); 			// send ctrl byte (write mode)
	receive_ack();					// wait for ACK
	
	write_byte(addr_hi);
	receive_ack();
	write_byte(addr_lo);
	receive_ack();
	write_byte(data);
	receive_ack();
	stop();

	// 2000us = 2ms delay ?
	delay_iic(2000);
	
	OS_EXIT_CRITICAL();
}

int eeprom_ack_polling(void)
{
	OS_ENTER_CRITICAL();
	start();			// start condition
	write_byte(I2C_ADDR_WRITE); 	// send ctrl byte (write mode)
	return(receive_ack());		// wait for ACK, 0 if ACK, 1 if NACK
	OS_EXIT_CRITICAL();
}	