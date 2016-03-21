/*
 * spi.h
 *
 * Created: 21-3-2016 15:05:54
 *  Author: kenny
 */ 


#ifndef SPI_H_
#define SPI_H_

/*
* Project name : Demo5_7a : spi - 7 segments display
* Author : Avans-TI, WvdE, JW
* Revision History : 20110228: - initial release; 20120307: - this version
* Description : This program sends data to a 4-digit display with spi
* Test configuration:
 MCU: ATmega128
 Dev.Board: BIGAVR6
 Oscillator: External Clock 08.0000 MHz
 Ext. Modules: Serial 7-seg display
 SW: AVR-GCC
* NOTES : Turn ON switch 15, PB1/PB2/PB3 to MISO/MOSI/SCK
*/
#define BIT(x) ( 1<<x )
#define DDR_SPI DDRB // spi Data direction register
#define PORT_SPI PORTB // spi Output register
#define SPI_SCK 1 // PB1: spi Pin System Clock
#define SPI_MOSI 2 // PB2: spi Pin MOSI
#define SPI_MISO 3 // PB3: spi Pin MISO
#define SPI_SS 0 // PB0: spi Pin Slave Select

void spi_masterInit(void);
void spi_write( unsigned char );
char spi_writeRead( unsigned char );
void spi_slaveSelect(unsigned char);
void spi_slaveDeSelect(unsigned char);
void displayDriverInit();
void displayOn();
void displayOff();

void spi_writeWord( unsigned char adress, unsigned char data );
void writeLedDisplay( int value ); // toont de waarde van value op het 4-digit display



#endif /* SPI_H_ */