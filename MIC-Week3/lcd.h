/*
 * lcd.h
 *
 * Created: 23-2-2016 14:06:44
 *  Author: kenny
 */ 


#ifndef LCD_H_
#define LCD_H_

void init();
void display_text(int line, char *str);
void display_char(unsigned char d);
void set_cursor(int position);
void shift_display(int left, int count);

#endif /* LCD_H_ */