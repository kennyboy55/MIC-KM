/*
 * Project name:
     Demo4_5 : PWM with timer 1 Fast PWM mode at PORTB.5, PB.6 en PB.7
	 			parallel, 3x, for RGB LED
 * Author: Avans-TI, JW
 * Revision History: 
     20101230: - initial release;
 * Description:
     This program gives an interrupt on each ms
 * Test configuration:
     MCU:             ATmega128
     Dev.Board:       BIGAVR6
     Oscillator:      External Clock 08.0000 MHz
     Ext. Modules:    -
     SW:              AVR-GCC
 * NOTES:
     - Turn ON the PORT LEDs at SW12.1 - SW12.8
*/

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"

#define BIT(x)			(1 << (x))

unsigned int sCount=0, minutes=0, hours=0;

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

// Initialize timer 1: fast PWM at pin PORTB.6 (hundredth ms)
void timer1Init( void )
{
	OCR1A = 0;					// RED, default, off
	OCR1B = 0;					// GREEN, default, off
	OCR1C = 0;					// BLUE, default, off
	TCCR1A = 0b10101001;		// compare output OC1A,OC1B,OC1C
	TCCR1B = 0b00001011;		// fast PWM 8 bit, prescaler=64, RUN
}

// Set pulse width for RED on pin PB5, 0=off, 255=max
void setRed( unsigned char red )
{
	OCR1A = red;
}

// Set pulse width for Green on pin PB5, 0=off, 255=max
void setGreen( unsigned char green )
{
	OCR1B = green;
}

// Set pulse width for Blue on pin PB5, 0=off, 255=max
void setBlue( unsigned char blue )
{
	OCR1C = blue;
}

// void setGreen( unsigned char green)
// void setBlue( unsigned char blue)

// Initialize ADC:
void adcInitFree( void )
{
	ADMUX = 0b01100001;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
}

void adcInitPoll( void )
{
	ADMUX = 0b11100001;			// AREF=2,56 V, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b10000110;		// ADC-enable, no interrupt, no free running, division by 64
}


int main( void )
{

	//OpgaveB22();
	OpgaveB23();

	//OpgaveB31();

	//OpgaveB32();
	//OpgaveB33();

}


int OpgaveB31( void )
{

	DDRF = 0x00;
	DDRA = 0xFF;
	DDRB = 0xFF;

	adcInitFree();

	while(1)
	{
		PORTA = ADCH;
		PORTB = ADCL;
		wait(10);
	}

}

int OpgaveB33( void )
{
	DDRF = 0x00;
	adcInitPoll();
	lcd_init();

	char l1[6];

	while(1)
	{
		ADCSRA |= BIT(6);				// Start ADC
		while ( ADCSRA & BIT(6) ) ;		// Wait for completion

		sprintf(l1, "%d", ADCH);
		display_text(0, l1);

		wait(3000);

		clear_display();
	}
}


// Main program: Counting on T1
int OpgaveB32( void )
{
	DDRB = 0xFF;	
	DDRF = 0x00;					// set PORTF for input (ADC)
	DDRA = 0xFF;					// set PORTA for output
	adcInitPoll();						// initialize ADC
	timer1Init();

	while (1)
	{
		//ADCSRA |= BIT(6);				// Start ADC
		while ( ADCSRA & BIT(6) ) ;		// Wait for completion
		setRed(ADCH);
		//setBlue(ADCH);
		//setGreen(ADCH);					
		wait(100);						// every 50 ms (busy waiting)
		//alleKleuren();
		//opgaveextra();
		setWithButton();
	}
}

int setWithButton(void)
{
	if( (0b00000001 & PINC) != 0)
	{
		ADCSRA |= BIT(6);
	}
}

int OpgaveB23(void)
{
	DDRB = 0xFF;					// set PORTB for compare output
	timer1Init();
	wait(100);

	int delta1 = 1;
	int delta2 = 1;
	int delta = 1;
	setRed (0);
	setBlue(0);
	setGreen(0);

	for (int red = 0; red<=255; red+=delta)
	{
		setRed( red );				// 8-bits PWM on pin OCR1a
		delta += 2;					// progressive steps up
		wait(100);					// delay of 100 ms (busy waiting)
	}

	while (1)
	{

		for (int green = 0; green<=255; green+=delta1)
		{
			setGreen( green );				// 8-bits PWM on pin OCR1a
			delta1 += 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}

		for (int blue = 0; blue<=255; blue+=delta2)
		{
			setBlue( blue );				// 8-bits PWM on pin OCR1a
			delta2 += 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}

		for (int red = 255; red>=0; red-=delta)
		{
			setRed( red );				// 8-bits PWM on pin OCR1a
			delta -= 2;					// progressive steps down
			wait(100);					// delay of 100 ms (busy waiting)
		}

		for (int green = 255; green>=0; green-=delta1)
		{
			setGreen( green );				// 8-bits PWM on pin OCR1a
			delta1 -= 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}

		for (int red = 0; red<=255; red+=delta)
		{
			setRed( red );				// 8-bits PWM on pin OCR1a
			delta += 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
	
		for (int blue = 255; blue>=0; blue-=delta2)
		{
			setBlue( blue );				// 8-bits PWM on pin OCR1a
			delta2 -= 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}

	}

}
// Main program: Counting on T1
int OpgaveB22( void )
{
	DDRB = 0xFF;					// set PORTB for compare output 
	timer1Init();
	wait(100);

	while (1)
	{
		int delta = 1;
		setRed (0);
		setBlue(0);
		setGreen(0);

		// change some colors
		// RED
		for (int red = 0; red<=255; red+=delta)
		{
			setRed( red );				// 8-bits PWM on pin OCR1a 
			delta += 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		for (int red = 255; red>=0; red-=delta)
		{
			setRed( red );				// 8-bits PWM on pin OCR1a 
			delta -= 2;					// progressive steps down
			wait(100);					// delay of 100 ms (busy waiting)
		}
		setRed( 0 );
		delta = 1;
		wait(100);
		
		// GREEN
		for (int green = 0; green<=255; green+=delta)
		{
			setGreen( green );				// 8-bits PWM on pin OCR1a
			delta += 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		for (int green = 255; green>=0; green-=delta)
		{
			setGreen( green );				// 8-bits PWM on pin OCR1a
			delta -= 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		setGreen( 0 );
		delta = 1;
		wait(100);

		// BLUE
		for (int blue = 0; blue<=255; blue+=delta)
		{
			setBlue( blue );				// 8-bits PWM on pin OCR1a
			delta += 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		for (int blue = 255; blue>=0; blue-=delta)
		{
			setBlue( blue );				// 8-bits PWM on pin OCR1a
			delta -= 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		setBlue( 0 );
		delta = 1;
		wait(100);
		

		// YELLOW
		for (int yellow = 0; yellow<=255; yellow+=delta)
		{
			setBlue( yellow );	
			setGreen( yellow );			// 8-bits PWM on pin OCR1a
			delta += 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		for (int yellow = 255; yellow>=0; yellow-=delta)
		{
			setBlue( yellow );
			setGreen( yellow );			// 8-bits PWM on pin OCR1a
			delta -= 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		setBlue( 0 );
		setGreen( 0 );
		delta = 1;
		wait(100);

		// WHITE
		for (int white = 0; white<=255; white+=delta)
		{
			setBlue( white );
			setRed( white );
			setGreen( white );			// 8-bits PWM on pin OCR1a
			delta += 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		for (int white = 255; white>=0; white-=delta)
		{
			setBlue( white );
			setRed( white );
			setGreen( white );			// 8-bits PWM on pin OCR1a
			delta -= 2;					// progressive steps up
			wait(100);					// delay of 100 ms (busy waiting)
		}
		setBlue( 0 );
		setGreen( 0 );
		setRed( 0 );
		delta = 1;
		wait(100);
	} 
}