/*
 * MIC-Week3.c
 *
 * Created: 23-2-2016 10:18:35
 * Author : kenny
 */ 

#define F_CPU 8000000UL //CPU op 8MHz
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

typedef struct {
	unsigned char data[10];
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
	{"Kenneth"}, {"Janco"}, {"Arthur"}, {"Martijn"}, {"Mauro"}, {"Rene"}, {"Remco"}, {"Jannick"}   
};

int main(void)
{
	opgave1();

	return 1;
}

void opgave1()
{
	lcd_init();

	srand(98126519);

	int r = 8;

	while(1){
		
		r = rand() % 8;

		lcd_writeLine(0, pattern[r].data);
		_delay_ms(1000);

		//clear
		lcd_command(0x01);
		_delay_ms(100);
	}
}

void lcd_init()
{
	DDRC = 0b11111111;

	_delay_ms(100);

	// return home
	lcd_command( 0x02 );
	// mode: 4 bits interface data, 2 lines, 5x8 dots
	lcd_command( 0x28 );
	// display: on, cursor off, blinking off
	lcd_command( 0x0C );
	// entry mode: cursor to right, no shift
	lcd_command( 0x06 );
	// RAM address: 0, first position, line 1
	lcd_command( 0x80 );
}

void lcd_writeChar( unsigned char dat )
{
	PORTC = dat & 0xF0; // hoge nibble
	PORTC = PORTC | 0x0C; // Enable on (EN = 1, RS=1),
	_delay_ms(1); // wait 1 ms
	PORTC = 0x04; // Enable off (EN = 0, RS = 1)

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
	_delay_ms(1); // wait 1 ms
	PORTC = 0x04; // stop (EN = 0)

	PORTC = (dat & 0x0F) << 4; // lage nibble
	PORTC = PORTC | 0x08; // data (RS=0),
	// start (EN=1)
	_delay_ms(1); // wait 1 ms
	PORTC = 0x00; // stop
	// (EN=0 RS=0)
}

void lcd_writeLine ( int line, char text[] )
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

	int count = 0;

	while (text[count] != '\0') {
		lcd_writeChar( text[count] );
		count++;
		_delay_ms(1);
	}
}