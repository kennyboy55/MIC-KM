/*
 * MIC-Week1.c
 *
 * Created: 2-2-2016 11:48:44
 * Author : kenny
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

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

int main(void)
{
	DDRD = 0b00000000; //input
	DDRC = 0b11111111; //output

	//Alleen voor opgave 4
	PORTC = 0x1;

	while (1)
	{
		//opgave1();
		//opgave2();
		//opgave3();
		opgave4();

		/*
		if( (PIND) != 0 )
		{
			speaker( PIND );
		}
		*/
	}

	return 1;
}

void speaker(int i)
{
	//PORTC moet hiervoor output zijn en PORTD input
	PORTC = 0b01010101;
	wait( i );
	PORTC = 0b00000000;
	wait( i );
}

void opgave1(void)
{
	PORTD = 0b10000000;
	wait( 500 );
	PORTD = 0b01000000;
	wait ( 500 );
}

void opgave4(void)
{
	wait( 50 );
	if (PORTC>= 0x80)
	{
		PORTC= 0x1;
	}
	else
	{
		PORTC = (PORTC<<1);
	}
}

