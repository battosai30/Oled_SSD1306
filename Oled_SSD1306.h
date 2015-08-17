/******************************************************************************
 * File Name   :  Oled_SSD1306.h
 * Author      :  B@tto - http://battomicro.wordpress.com 
				  based on work of 43oh - MSP430 News Projects and Forums.
               :  (http://www.43oh.com)
 * Description :  Hig level driver for the OLED SSD1306 
 * Date        :  August 17, 2015.
 *****************************************************************************/

#ifndef OLED_SSD1306_H_
#define OLED_SSD1306_H_

#include "Energia.h"
#include <SPI.h>

// OLED GPIO MACROS
#define OLED_SELECT      digitalWrite(OLED_CS,LOW)
#define OLED_DESELECT	 digitalWrite(OLED_CS,HIGH)
#define OLED_COMMAND     digitalWrite(OLED_DC,LOW)
#define OLED_DATA	     digitalWrite(OLED_DC,HIGH)
#define OLED_RES_LOW     digitalWrite(OLED_RES,LOW)
#define OLED_RES_HIGH    digitalWrite(OLED_RES,HIGH)

// SSD1306 PARAMETERS
#define SSD1306_LCDWIDTH 128
#define SSD1306_LCDHEIGHT 64
#define SSD1306_MAXROWS 7
#define SSD1306_MAXCONTRAST 0xFF

	// command table
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

	// scrolling commands
#define SSD1306_SCROLL_RIGHT 0x26
#define SSD1306_SCROLL_LEFT 0X27
#define SSD1306_SCROLL_VERT_RIGHT 0x29
#define SSD1306_SCROLL_VERT_LEFT 0x2A
#define SSD1306_SET_VERTICAL 0xA3

	// address setting
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDRESS 0x21
#define SSD1306_COLUMNADDRESS_MSB 0x00
#define SSD1306_COLUMNADDRESS_LSB 0x7F
#define SSD1306_PAGEADDRESS	0x22
#define SSD1306_PAGE_START_ADDRESS 0xB0

	// hardware configuration
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_SEGREMAP 0xA1
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

	// timing and driving
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETVCOMDETECT 0xDB
#define SSD1306_NOP 0xE3

	// power supply configuration
#define SSD1306_CHARGEPUMP 0x8D
#define SSD1306_EXTERNALVCC 0x10
#define SSD1306_SWITCHCAPVCC 0x20

// PROTOTYPES
void SSD1306PinSetup(int CS,int DC,int RST);
void SSD1306Init( void );

void SSD1306SendCommand(byte *data, int i );
void SSD1306SendData(byte *data, int i );
void setAddress( byte page, byte column );
void clearScreen(void);
void charDraw(byte row, byte column, int data);
void stringDraw( byte row, byte column, char *word);
void pixelDraw(byte x, byte y);
void horizontalLine(byte xStart, byte xStop, byte y);
void verticalLine(byte x, byte yStart, byte yStop);
void imageDraw(const byte IMAGE[], byte row, byte column);
//void circleDraw(char x, char y, char radius);
void circleDraw(register int x, register int y, int r);

void Set_Contrast_Control(byte d);
void Set_Inverse_Display( byte d);
void Set_Display_On_Off( byte d);

void Fill_RAM( byte data);
void Fill_RAM_PAGE(byte page, byte data);



#endif /*SSD1306_OLED_H_*/
