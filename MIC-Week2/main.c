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
#define NELEMS(x)  (sizeof(x) / sizeof(x[0])) //Geeft het aantal elementen uit een array terug.

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int opdr;
int count = 0; //Counter voor opgave 4 om bij te houden bij welke pattern in de array hij is
int speed = 1; //De snelheid voor opgave 4 van de patronen

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

char data[16] = { // 7 segment display
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
	0b01110001  // F
};

//Struct voor een pattroon. Eerste onderdeel is welke ledjes aan moeten staan, tweede hoelang deze dan aan moeten staan
typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

//PIN 0 == SEG A
//PIN 1 == SEG B
//PIN 2 == SEG C
//PIN 3 == SEG D
//PIN 4 == SEG E
//PIN 5 == SEG F
//PIN 6 == SEG G
//PIN 7 == SEG DP

//Array van patronen die elkaar opvolgen.
PATTERN_STRUCT pattern[] = {
	{0b00000001, 100}, {0b00000010, 100}, {0b00000100, 100}, {0b00001000, 100}, {0b00010000, 100}, {0b00100000, 100}, {0b00000001, 100},
	{0b00100000, 100}, {0b00010000, 100}, {0b00001000, 100}, {0b00000100, 100}, {0b00000010, 100}, {0b00000001, 100},
	{0b00000001, 100}, {0b01000000, 100}, {0b00001000, 100}, //omlaag
	{0b00001000, 100}, {0b01000000, 100}, {0b00000001, 100}, //omhoog
	{0b00000001, 100}, {0b01000000, 100}, {0b00001000, 100}, //omlaag
	{0b00001000, 100}, {0b01001000, 100}, {0b00001001, 100}, //omhoog
	{0b00001001, 100}, {0b00010010, 100}, {0b00100100, 100}, {0b00001001, 100}, {0b00010010, 100}, {0b00100100, 100}, {0b00001001, 100},
	{0b00001001, 100}, {0b01000001, 100}, {0b00000001, 100}
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
short:			ISR INT1 / Shift omhoog
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

ISR( INT2_vect )
/*
short:			Shift omlaag
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
notes:			Toggle PORTE.0
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
	opgave2();
	//opgave3();
	//opgave4();

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
	opdr=0;					// Use of the right interrupt
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
	EIMSK |= 0x06;		//Enable INT1 & INT2
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
	DDRC = 0x00;	//Alles op input in rij c
	DDRD = 0xFF;	//Allesj op output in rij d.

	int count = 0;	//teller voor het juiste cijfer op display

	while(1)
	{
		wait(150);

		//Button C0 & C1
		if( (0x03 & PINC) == 0x03)	 
		{
			count = 0;
		}
		//Button C0
		else if( (0x01 & PINC) != 0)
		{
			count = ((count+1 > 15) ? 16 : count+1);
		}
		//Button C1
		else if( (0x02 & PINC) != 0)
		{
			count = ((count-1 < 0) ? -1 : count-1);
		}

		display(count, 0);
	}
}
void display(int d, int dot)
{
	//Digit > 15 || Digit < 0 : display E met .
	if(d > 15)
	{
		d = 14;
		dot = 1;
	}
	else if(d < 0)
	{
		d = 14;
		dot = 1;
	}

	//Met of zonder dot het getal tekenen
	if(dot)
		PORTD = data[d] | 0b10000000;
	else
		PORTD = data[d];
}

void opgave4(void)
{
	DDRD = 0xFF;
	DDRC = 0x00;

	while(1)
	{
		//Als een knop ingedrukt wordt, wordt de waarde daarvan de snelheid. Werkt niet met system interrupts, dus je moet de knop soms ingedrukt houden
		if(PINC != 0 )
		{
			speed = PINC;
		}

		//Schrijf de data van het huidige patroon naar PORTD
		PORTD = pattern[count].data;
		//Wacht het aantal milliseconde dat in het huidige patroon staat
		wait(pattern[count].delay * speed);

		count++; //patroon verder

		//als alle patronen geweest zijn, opnieuw beginnen
		if(count >= NELEMS(pattern))
		{
			count = 0;
		}
	}
}