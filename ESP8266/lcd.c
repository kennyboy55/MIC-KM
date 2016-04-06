/*
 * lcd.c
 *
 * Created: 23-2-2016 14:06:31
 *  Author: kenny
 */ 
  #define F_CPU 8000000UL //CPU op 8MHz

 #include <avr/io.h>
 #include <util/delay.h>
 #include <stdlib.h>

 #include "lcd.h"

 //LOCAL
 void display_char(unsigned char dat);
 void lcd_command(unsigned char dat);

 void lcd_init()
 {
 	DDRC = 0b11111111;

 	_delay_ms(100);

 	// return home
 	lcd_command( 0x02 );
 	// mode: 4 bits interface data, 2 lines, 5x8 dots
 	_delay_ms(50);
 	lcd_command( 0x28 );
 	// display: on, cursor off, blinking off
 	_delay_ms(50);
 	lcd_command( 0x0C );
 	// entry mode: cursor to right, no shift
 	_delay_ms(50);
 	lcd_command( 0x06 );
 	// RAM address: 0, first position, line 1
 	_delay_ms(50);
 	clear_display();

 	_delay_ms(100);
 }

void display_text( int line, char *text, int start )
{
	// eerst de eerste 8 karakters = regel 1

	if(line)
	{
		// eerste pos regel 1
		lcd_command(0xC0);
	}
	else
	{
		// regel 0
		lcd_command(0x80);
	}

	int count = start;

	while (text[count] != '\0') {
		display_char( text[count] );
		count++;

		_delay_ms(1);
	}
}

void clear_display()
{
	lcd_command(0b00000001);
	lcd_command(0b10000000);
}

 void set_cursor(int position)
 {
	position = position | (1<<7);

	lcd_command(position);
 }

 void shift_display(int left, int count)
 {
	 unsigned char data = left ? 0b00011100 : 0b00011000;

	 for (int i=0; i<count; i++) {
		 lcd_command(data);
		 _delay_ms(2);
	 }
 }

 void display_char( unsigned char dat )
 {
	 PORTC = dat & 0xF0; // hoge nibble
	 PORTC = PORTC | 0x0C; // Enable on (EN = 1, RS=1),
	 _delay_ms(1); // wait 1 ms
	 PORTC = 0x04; // Enable off (EN = 0, RS = 1)

	 _delay_ms(1); // wait 1 ms

	 PORTC = (dat & 0x0F) << 4; // lage nibble
	 PORTC = PORTC | 0x0C; // Enable on (RS=1),
	 // start (EN=1)
	 _delay_ms(1); // wait 1 ms
	 PORTC = 0x00; // stop
	 // (EN=0 RS=0)
 }

 void lcd_command ( unsigned char dat )
 {
	 PORTC = dat & 0xF0; // hoge nibble
	 PORTC = PORTC | 0x08; // data (RS=0),
	 // start (EN=1)
	 _delay_ms(2); // wait 1 ms
	 PORTC = 0x04; // stop (EN = 0)

	 _delay_ms(2); // wait 1 ms

	 PORTC = (dat & 0x0F) << 4; // lage nibble
	 PORTC = PORTC | 0x08; // data (RS=0),
	 // start (EN=1)
	 _delay_ms(2); // wait 1 ms
	 PORTC = 0x00; // stop
	 // (EN=0 RS=0)
 }