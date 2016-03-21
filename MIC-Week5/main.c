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
#include "spi.h"

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
	//opgaveb1();
	//opgaveb2();

	//opgaveb3();
	//opgaveb4();

	//test();
	test2();
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

		if(c >= 'a' && c <= 'z'){
			c = ('A' + c - 'a');
		}

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

int opgaveb3( void )
{
	DDRB=0x01; // Set PB0 pin as output for display select

	spi_masterInit(); // Initialize spi module
	displayDriverInit(); // Initialize display chip

	// clear display (all zero's)
	for (char i =1; i<=4; i++)
	{
		spi_slaveSelect(0); // Select display chip
		spi_write(i); // digit adress: (digit place)
		spi_write(0); // digit value: 0
		spi_slaveDeSelect(0); // Deselect display chip
	}

	wait(1000);

	// write 4-digit data
	for (char i =1; i<=4; i++)
	{
		spi_slaveSelect(0); // Select display chip
		spi_write(i); // digit adress: (digit place)
		spi_write(i); // digit value: i (= digit place)
		spi_slaveDeSelect(0); // Deselect display chip
		wait(1000);
	}

	wait(1000);
	return (1);
}

int opgaveb4( void )
{
	DDRB=0x01; // Set PB0 pin as output for display select

	spi_masterInit(); // Initialize spi module
	displayDriverInit(); // Initialize display chip

	int i = 4321;
	writeLedDisplay(i);

	i = 1234;
	wait(1000);
	writeLedDisplay(i);

	i = 567;
	wait(1000);
	writeLedDisplay(i);

	i = 98;
	wait(1000);
	writeLedDisplay(i);

	i = -123;
	wait(1000);
	writeLedDisplay(i);

	i = -23;
	wait(1000);
	writeLedDisplay(i);

	wait(5000);
	return (1);
}

int test(void)
{
	DDRB=0x01; // Set PB0 pin as output for display select

	spi_masterInit(); // Initialize spi module
	displayDriverInit(); // Initialize display chip

	for(int i=0; i<=127; i++)
	{
		writeLedDisplay(i);

		wait(10);

		spi_slaveSelect(0); // Select display chip
		spi_write(4); // digit adress: (digit place)
		spi_write(i); // digit value: i (= digit place)
		spi_slaveDeSelect(0); // Deselect display chip

		wait(500);
	}
}

int test2(void)
{
	DDRB=0x01; // Set PB0 pin as output for display select

	spi_masterInit(); // Initialize spi module
	displayDriverInit(); // Initialize display chip

	for(int i=-999; i<=9999; i++)
	{
		writeLedDisplay(i);
		wait(5);

		if(i < -150 && i > -250)
			wait(75);

		if(i > 150 && i < 250)
			wait(75);
	}


}