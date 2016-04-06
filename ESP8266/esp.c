/*
 * esp.c
 *
 * Created: 6-4-2016 12:33:10
 *  Author: kenny
 */ 

 #include "uart.h"

 void esp_init()
 {
	uart_init(9600);

	wait(50);

	uart_start();

	wait(10);
 }
 	