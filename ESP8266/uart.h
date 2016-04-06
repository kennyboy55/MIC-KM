/*
 * Project name		: uart0 : UART - send / receive characters with waiting
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

#ifndef _UART
#define _UART

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LF			0x0a					// ascii code for Line Feed
#define CR			0x0d					// ascii code for Carriage Return
#define BIT(x)		(1 << (x))
#define MYUBRR(x)		F_CPU/16/(x) - 1	// My USART Baud Rate Register

void uart_init( int )	;				// initialize USART0 - receive/transmit

void uart_start( void ) ;					// UART0 receiver & transmitter enable

char uart_receiveChar() ;					// UART0: receive ch

int uart_sendChar( char ch ) ;				// UART0: send ch

int uart_receiveString( char* string);		// UART0: receive string until LF

int uart_sendString( char* string);

#endif