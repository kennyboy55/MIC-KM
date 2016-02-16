/*
 * MIC-Week1.c
 *
 * Created: 2-2-2016 11:48:44
 * Author : Kenneth en Martijn
 */ 

#define F_CPU 8000000UL //CPU op 8MHz
#define NELEMS(x)  (sizeof(x) / sizeof(x[0])) //Geeft het aantal elementen uit een array terug.

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

int reset = 0; //Reset voor opgave 6 (wordt gebruikt in de wait methode)
int count = 0; //Counter voor opgave 5 om bij te houden bij welke pattern in de array hij is
int speed = 1; //De snelheid voor opgave 5 van de patronen
int blink = 1000; //De snelheid voor opgave 6 van het ledje
int b1 = 0; //Houdt bij voor opgave 6 of de knop al weer losgelaten is
int b2 = 0; //Houdt bij voor opgave 6 of de knop al weer losgelaten is
int direction = 0; //De richting van het looplicht in opgave 4

void wait( int ms ) {
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

//Struct voor een pattroon. Eerste onderdeel is welke ledjes aan moeten staan, tweede hoelang deze dan aan moeten staan
typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

//Array van patronen die elkaar opvolgen.
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

//System timer interrupt.
ISR(TIMER0_OVF_vect) {

		//C0 ingedrukt
		if( (0b00000001 & PINC) != 0  && b1 == 0)
		{
			blink = blink*2; //Slomer knipperen
			reset = 1; //Reset de timer
			b1 = 1;
		}

		//C1 ingedrukt
		if( (0b00000010 & PINC) != 0 && b2 == 0 )
		{
			blink = blink/2; //Sneller knipperen
			reset = 1; //Reset de timer
			b2 = 1;
		}

		//Knoppen losgelaten
		if( PINC == 0)
		{
			b1 = 0;
			b2 = 0;
		}

		//Lower limit
		if(blink < 20)
		{
			blink = 20;
		}
		//Upper limit
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
	 //Het overflow event aanzetten
	 TIMSK|=(1<<TOIE0);
	 //Counter op waarde 0 zetten
	 TCNT0=0;

	//Luister naar System interrupts
	sei();

	//Alleen voor opgave 4
	PORTD = 0x1;

	while (1)
	{
		//opgave2();
		//opgave3();
		//opgave4();
		opgave5();
		//opgave6();

		//charlieplexing();

		//extraopgave();
	}

	return 1;
}

void extraopgave(void)
{
	//Een knop op PORTC is ingedrukt
	if( (PINC) != 0 )
	{
		//Verschillende toonhoogtes door de waarde van de PORT te pakken.
		speaker( PINC );
	}
}

void speaker(int i)
{
	PORTD = 0b01010101;
	wait( i );
	PORTD = 0b00000000;
	wait( i );
}

void opgave2(void)
{
	PORTD = 0b10000000; //Led D7 aan
	wait( 500 );
	PORTD = 0b01000000; //Led D6 aan
	wait ( 500 );
}

void opgave3(void)
{
	//Knop C0 ingedrukt
	if( (0b00000001 & PINC) != 0  )
	{
	   //knipperen
	   PORTD = 0b10000000;
	   wait( 100 );
	   PORTD = 0b00000000;
	   wait( 100 );  
	}
}

void opgave4(void)
{
	wait( 50 );

	//Als het looplicht aan het einde is
	if (PORTD>= 0x80)
	{
		direction = 1;
	}
	//Als het looplicht aan het begin is
	else if (PORTD <= 0x1)
	{
		direction = 0;
	}

	if(direction)
	PORTD = (PORTD>>1); //Naar een hogere waarde shiften
	else
	PORTD = (PORTD<<1); //Naar een lagere waarde shiften

}

void opgave5(void)
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
