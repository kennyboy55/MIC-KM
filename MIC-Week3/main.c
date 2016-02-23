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

#include "lcd.h"

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
	{"Kenneth"}, {"Janco"}, {"Arthur"}, {"Martijn"}, {"Mauro"}, {"Rene"}, {"Remco"}, {"Jannick"}, {"Tim"}   
};

int main(void)
{
	//opgave1();

	opgave2knop();

	return 1;
}

void opgave1()
{
	init();

	srand(14152);

	int r = 0;

	while(1){
		
		r = rand() % 9;

		display_text(1, ":-) ~ (-:");
		display_text(0, pattern[r].data);

		_delay_ms(1000);

		set_cursor(15);
		display_char(0b01000001);

		_delay_ms(1000);

		shift_display(1, 3);

		_delay_ms(1000);

		//clear
		lcd_command(0x01);
		_delay_ms(100);
	}
}

void opgave2knop()
{

}