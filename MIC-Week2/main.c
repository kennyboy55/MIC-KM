/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ioisr.c
**
** Beschrijving:	ISR on PORTD demonstrattion  
** Target:			AVR mcu
** Build:			avr-gcc -std=c99 -Wall -O3 -mmcu=atmega128 -D F_CPU=8000000UL -c ioisr.c
**					avr-gcc -g -mmcu=atmega128 -o ioisr.elf ioisr.o
**					avr-objcopy -O ihex ioisr.elf ioisr.hex 
**					or type 'make'
** Author: 			dkroeske@gmail.com
** -------------------------------------------------------------------------*/

#define F_CPU 8000000UL //CPU op 8MHz
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int opdr;
/******************************************************************/
void wait( int ms )
/* 
short:			Busy wait number of millisecs
inputs:			int ms (Number of millisecs to busy wait)
outputs:	
notes:			Busy wait, not very accurate. Make sure (external)
				clock value is set. This is used by _delay_ms inside
				util/delay.h
Version :    	DMK, Initial code
*******************************************************************/
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

char data[16] = {		//segment display
	0b00111111, // 0
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
	0b00000111, // 7
	0b01111111, // 8
	0b01101111, // 9
	0b01110111, // A
	0b01111100, // B
	0b00111001, // C
	0b01011110, // D
	0b01111001, // E
	0b01110001 // F
};

/******************************************************************/
ISR( INT0_vect ) //powerpointopg
/* 
short:			ISR INT0
inputs:			
outputs:	
notes:			Set PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
{
    PORTD |= (1<<5);		
}

/******************************************************************/
ISR( INT1_vect )
{ 
/* 
short:			ISR INT1
inputs:			
outputs:	
notes:			Clear PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
	if(opdr==0)
	{
		PORTD &= ~(1<<5);		
	}
	else if(opdr==1)
	{
		shift(1);
	}
}

ISR( INT2_vect ) //powerpointopg
/*
short:			ISR INT4
inputs:
outputs:
notes:			Clear PORTD.3
Version :    	DMK, Initial code
*******************************************************************/
{
	shift(0);
}

ISR( INT4_vect ) //powerpointopg
/*
short:			ISR INT4
inputs:
outputs:
notes:			Clear PORTD.3
Version :    	DMK, Initial code
*******************************************************************/
{
	PORTE ^= 0x01; //PE0
}

/******************************************************************/
int main( void )
/*
short:			main() loop, entry point of executable
inputs:			
outputs:	
notes:			Slow background task after init ISR
Version :    	DMK, Initial code
*******************************************************************/
{
	//powerpointOpg();
	//opgave2();
	opgave3();

	return 1;
}

void powerpointOpg(void)
{
	// Init I/O
	DDRD = 0xF0;			// PORTD(7:4) output, PORTD(3:0) input
	DDRE = 0x0F;			// PORTE(7:4) input, PORTE(3:0) output

	// Init Interrupt hardware
	EICRA |= 0x0B;			// INT1 falling edge, INT0 rising edge
	EICRB |= 0x02;			// INT4 falling edge
	EIMSK |= 0x13;			// Enable INT1 & INT0 & INT4


	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	opdr=0;
	sei();
	

	while (1)
	{
		PORTD ^= (1<<7);	// Toggle PORTD.7
		wait( 500 );
	}
}

void opgave2(void)
{
	DDRD = 0xF0;	//PORTD(7:4) output, PORTD(3:0) input
	DDRC = 0xFF;	//PORTC(7:0) output;
	PORTC= 0x1;

	EICRA |= 0x28;		//INT1 en INT2 falling edge 
	EIMSK |= 0x06;
	opdr=1;
	sei();

	while(1)
	{
		wait(500);
	}
}

void shift(int i)
{

	if(i)
		PORTC = (PORTC>>1);
	else
		PORTC = (PORTC<<1);
		
	//Als het looplicht aan het einde is
	if (PORTC >= 0x80)
	{
		PORTC= 0x01;
	}
	//Als het looplicht aan het begin is
	else if (PORTC < 0x1)
	{
		PORTC= 0x80;
	}
}

void opgave3()
{
	DDRC = 0x00;
	DDRD = 0xFF;

	int count = 0;

	while(1)
	{
		if(0b00000001 & PINC == 0b00000001)
		{
			count++;
		}
		else if(0b00000010 & PINC == 0b00000010)
		{
			count--;
		}

		if(count > 15)
			display(14);
		else if(count < 0)
			display(14);
		else
			display(count);
	}
}
void display(int d)
{
	PORTD = data[d];
}