/*
 * Project name		: Demo5_1 : UART - send / receive characters with waiting
 * Author			: Avans-TI, JW
 * Revision History	: 20110227: - initial release;
 * Description		: This program receive a character, and sends it back
 * Test configuration:
     MCU:             ATmega128
     Dev.Board:       BIGAVR6
     Oscillator:      External Clock 08.0000 MHz
     Ext. Modules:    -
     SW:              AVR-GCC
 * NOTES:
     - Turn ON switch 12, PEO/PE1 tot RX/TX
*/

#define F_CPU 8000000UL

#include "uart0.h"
#include "lcd.h"

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );						// library function (max 30 ms at 8MHz)
	}
}

int main( void )
{
	opgaveb1();
	//opgaveb2();

	return 0;
}


// send/receive uart - dB-meter
int opgaveb1( void )
{
	usart0_init();							// initialize USART0
	usart0_start();

	while (1)
	{
		wait(50);							// every 50 ms (busy waiting)

		char c = uart0_receiveChar();		// receive string from uart
		uart0_sendChar(c);

		//uart0_sendChar('*');
	}
}

// send/receive uart - dB-meter
int opgaveb2( void )
{
	char buffer[16];						// declare string buffer 

	lcd_init();								// initialize LCD-display
	usart0_init();							// initialize USART0
	usart0_start();

	wait(100);

	while (1)
	{
		wait(150);							// every 50 ms (busy waiting)

		uart0_receiveString( buffer );		// receive string from uart
		
		// write string to LCD display
		clear_display();
		display_text(0, buffer, 3);
	}
}