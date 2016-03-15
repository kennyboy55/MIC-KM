/*
 * lcd.h
 *
 * Created: 23-2-2016 14:06:44
 *  Author: kenny
 */ 


#ifndef LCD_H_
#define LCD_H_

void lcd_init();
void display_text(int line, char *str, int);
void display_char(unsigned char d);
void set_cursor(int position);
void shift_display(int left, int count);
void clear_display();

#endif /* LCD_H_ */