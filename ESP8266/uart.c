#include "uart.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)		(1 << (x))


void uart_init( int baud )						// initialize USART0 - receive/transmit
{
	int	ubrr = MYUBRR(baud);
	UBRR0H = ubrr>>8;						// baudrate register, hoge byte
	UBRR0L = ubrr;							// baudrate register, lage byte
	UCSR0C = 0b00000110;					// asynchroon, 8 data - 1 stop - no parity
	UCSR0B = 0b00000000;					// receiver & transmitter enable
}

void uart_start( void )					// receiver & transmitter enable
{
	UCSR0B |= BIT(RXEN)|BIT(TXEN);			// set bit RXEN = Receiver enable and TXEN = Transmitter enable
}

int uart_sendChar( char ch )
{
	while (!(UCSR0A & BIT(UDRE0))) ;		// wait until UDRE0 is set: transmit buffer is ready
	UDR0 = ch;								// send ch
	return 0;								// OK
}

char uart_receiveChar( void )
{
	while (!(UCSR0A & BIT(RXC0))) ;			// if RX0 is set: unread data present in buffer
	return UDR0;							// read ch
}

int uart_receiveString( char* string)
{
	int stop = 0; // boolean for stop value
	char ch;
	while (!stop)
	{
		ch = uart_receiveChar(); // read ch
		if ( ch == CR ) // stop at LF
			stop = 1;
		else
			*string++ = ch; // else fill buffer
	}
	*string = '\0'; // string terminator
	return 0;
}

int uart_sendString( char* string)
{
	int count = 0;

	while (string[count] != '\0') // while contunie
	{
		uart_sendChar(string[count]);
		count++;
	}

	uart_sendChar(CR);
	uart_sendChar(LF);
	
	return 0;
}