/*
 * MIC-Week1.c
 *
 * Created: 2-2-2016 11:48:44
 * Author : kenny
 */ 

#define F_CPU 8000000UL
#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int reset = 0;

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
		if(reset == 1)
		{
			reset = 0;
			return;
		}

		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = {
	{0x00, 100}, {0x01, 100}, {0x02, 100}, {0x04, 100}, {0x10, 100}, {0x20, 100}, {0x40, 100}, {0x80, 100},
	{0x00, 100},
	{0xAA,  50}, {0x55,  50},
	{0xAA,  50}, {0x55,  50},
	{0xAA,  50}, {0x55,  50},
	{0x00, 100},
	{0x81, 100}, {0x42, 100}, {0x24, 100}, {0x18, 100}, {0x0F, 200}, {0xF0, 200}, {0x0F, 200}, {0xF0, 200},
	{0x00, 0x00}
};

int count = 0;
int speed = 1;
int blink = 1000;
int b1 = 0;
int b2 = 0;
int direction = 0;

ISR(TIMER0_OVF_vect) {

		if( (0b00000001 & PINC) != 0  && b1 == 0)
		{
			blink = blink*2;
			reset = 1;
			b1 = 1;
		}

		if( (0b00000010 & PINC) != 0 && b2 == 0 )
		{
			blink = blink/2;
			reset = 1;
			b2 = 1;
		}

		if( PINC == 0)
		{
			b1 = 0;
			b2 = 0;
		}

		if(blink < 20)
		{
			blink = 20;
		}
		else if(blink > 4000)
		{
			blink = 4000;
		}
}

int main(void)
{
	DDRC = 0b00000000; //input
	DDRD = 0b11111111; //output

	 // Prescaler
	 TCCR0 |= (1 << CS01) | (1 << CS00);
	 //Enable Overflow Interrupt Enable
	 TIMSK|=(1<<TOIE0);
	 //Initialize Counter
	 TCNT0=0;

	sei();

	//Alleen voor opgave 4
	PORTD = 0x1;

	while (1)
	{
		//opgave1();
		//opgave2();
		//opgave3();
		opgave4();
		//opgave5();
		//opgave6();

		//charlieplexing();

		//extraopgave();
	}

	return 1;
}

void extraopgave(void)
{
	if( (PINC) != 0 )
	{
		speaker( PINC );
	}
}

void speaker(int i)
{
	//PORTD moet hiervoor output zijn en PORTC input
	PORTD = 0b01010101;
	wait( i );
	PORTD = 0b00000000;
	wait( i );
}

void opgave1(void)
{
	PORTD = 0b10000000;
	wait( 500 );
	PORTD = 0b01000000;
	wait ( 500 );
}

void opgave3(void)
{
	if( (0b00000001 & PINC) != 0  )
	{
	   PORTD = 0b10000000;
	   wait( 100 );
	   PORTD = 0b00000000;
	   wait( 100 );  
	}
}

void opgave4(void)
{
	wait( 50 );
	if (PORTD>= 0x80)
	{
		direction = 1;
	}
	else if (PORTD <= 0x1)
	{
		direction = 0;
	}

	if(direction)
	PORTD = (PORTD>>1);
	else
	PORTD = (PORTD<<1);

}

void opgave5(void)
{
	if(PINC != 0 )
	{
		speed = PINC;
	}

	// Write data to PORTD
	PORTD = pattern[count].data;
	// wait
	wait(pattern[count].delay * speed);

	count++;
	if(count >= NELEMS(pattern))
	{
		count = 0;
	}
}

void opgave6(void)
{
	PORTD = 0b10000000;
	wait( blink );
	PORTD = 0b00000000;
	wait( blink );
}

void charlieplexing(void)
{
	//Button C0
	if( (0b00000001 & PINC) != 0)
	{
		DDRD =  0b00000011; //PIN 1 & 2 as output. Pin 3 as input (TRI-STATE) 
		PORTD = 0b00000001; //PIN 1 High, PIN 2 Low
	}

	//Button C1
	else if( (0b00000010 & PINC) != 0)
	{
		DDRD =  0b00000011; //PIN 1 & 2 as output. Pin 3 as input (TRI-STATE)
		PORTD = 0b00000010; //PIN 2 High, PIN 1 Low
	}

	//Button C2
	else if( (0b00000100 & PINC) != 0)
	{
		DDRD =  0b00000110; //PIN 2 & 3 as output. Pin 1 as input (TRI-STATE)
		PORTD = 0b00000010; //PIN 2 High, PIN 3 Low
	}

	//Button C3
	else if( (0b00001000 & PINC) != 0)
	{
		DDRD =  0b00000110; //PIN 2 & 3 as output. Pin 1 as input (TRI-STATE)
		PORTD = 0b00000100; //PIN 3 High, PIN 2 Low
	}

	//Button C4
	else if( (0b00010000 & PINC) != 0)
	{
		DDRD =  0b00000101; //PIN 3 & 1 as output. Pin 2 as input (TRI-STATE)
		PORTD = 0b00000100; //PIN 3 High, PIN 1 Low
	}

	//Button C5
	else if( (0b00100000 & PINC) != 0)
	{
		DDRD =  0b00000101; //PIN 3 & 1 as output. Pin 2 as input (TRI-STATE)
		PORTD = 0b00000001; //PIN 1 High, PIN 3 Low
	}
}
