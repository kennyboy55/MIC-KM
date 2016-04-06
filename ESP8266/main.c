/*
 * ESP8266.c
 *
 * Created: 6-4-2016 12:25:55
 * Author : kenny & martijn
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"
#include "uart.h"
#include "esp.h"


void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );						// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{
	esp_init();

	wait(50);

	lcd_init();

	display_text(0,"Ready", 0);

	wait(150);

	clear_display();

    /* Replace with your application code */
    while (1) 
    {
		wait(1000);							// every 50 ms (busy waiting)

		uart_sendString("AT\0");

		char c = uart_receiveChar();		// receive string from uart
		
		display_char(c);
    }
}

