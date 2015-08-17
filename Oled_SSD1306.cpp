/******************************************************************************
 * File Name   :  Oled_SSD1306.cpp
 * Author      :  B@tto - http://battomicro.wordpress.com 
				  based on work of 43oh - MSP430 News Projects and Forums.
               :  (http://www.43oh.com)
 * Description :  Hig level driver for the OLED SSD1306 
 * Date        :  August 17, 2015.
 *****************************************************************************/

#include "Oled_SSD1306.h"
#include "FontPack.h"

int OLED_CS,OLED_DC,OLED_RES;

byte SSD1306_init[] = {
		SSD1306_DISPLAYOFF,
		SSD1306_SETLOWCOLUMN,
		SSD1306_SETHIGHCOLUMN,
		SSD1306_SETSTARTLINE,
		SSD1306_SETCONTRAST,
		0xcf,
		SSD1306_SEGREMAP,
		SSD1306_NORMALDISPLAY,
		SSD1306_SETMULTIPLEX,
		0x3f,
		SSD1306_SETDISPLAYOFFSET,
		0x00,
		SSD1306_SETDISPLAYCLOCKDIV,
		0x80,
		SSD1306_SETPRECHARGE,
		0xf1,
		SSD1306_SETCOMPINS,
		0x12,
		SSD1306_SETVCOMDETECT,
		0x40,
		SSD1306_CHARGEPUMP,
		0x14,
		SSD1306_DISPLAYON
};

/* Function    : SSD1306PinSetup(int _CS,int _DC,int _RST)
 * Description : Sets up the pins to the OLED display.
 * Input       : None
 * Output      : None
 * 
 */
void SSD1306PinSetup(int _CS,int _DC,int _RST)
{

	OLED_RES=_RST;
	OLED_DC=_DC;
	OLED_CS=_CS;
	
	pinMode(OLED_RES,OUTPUT);
	pinMode(OLED_DC,OUTPUT);
	pinMode(OLED_CS,OUTPUT);

   delay(100);
}

/* Function    : SSD1306Init()
 * Description : Initializes the OLED display
 * Input       : None
 * Output      : None
 */
void SSD1306Init(void)
{
	OLED_DESELECT;

	OLED_COMMAND;

	OLED_RES_LOW;

   delay(200);

   OLED_RES_HIGH;

   delay(200);
    
   SPI.begin();

   SSD1306SendCommand(SSD1306_init, sizeof SSD1306_init);

}

/* Function    : SSD1306SendCommand( byte *data, int i )
 * Description : Sends a command to the OLED display
 * Input       : Command pointer and number of bytes to transfer
 * Output      : None
 * gwd
 */
void SSD1306SendCommand(byte *data, int i )
{ 

	OLED_DESELECT;	// cs high
	OLED_COMMAND;	// D/C low
	OLED_SELECT;	// cs low

	while(i)
	{
		
		SPI.transfer(*data);	// load data

		data++;				// increment pointer

		i--;				// decrement byte counter
	}

	OLED_DESELECT;	// cs high

}

/* Function    : SSD1306SendData(byte *data, int i)
 * Description : Sends data to the OLED display
 * Input       : Data
 * Output      : None
 */
void SSD1306SendData(byte *data, int i )
{


	OLED_DESELECT;	// cs high
	OLED_DATA;		// D/C high
	OLED_SELECT;	// cs low

	while(i)
	{

		SPI.transfer(*data);	// load data

		data++;				// increment pointer

		i--;				// decrement byte counter
	}

	OLED_DESELECT;	// cs high

}

/* Function		: setAddress(byte page, byte column)
 * Description	: sets position of LCD RAM
 * Input		: page (0-7), column (0-127)
 * Output		: none
 */
void setAddress( byte page, byte column )
{

	byte pageAddress[] = {SSD1306_PAGE_START_ADDRESS};
	byte columnAddress[] = { SSD1306_COLUMNADDRESS, SSD1306_COLUMNADDRESS_MSB, SSD1306_COLUMNADDRESS_LSB };

	if (page > SSD1306_MAXROWS)
	{
		page = SSD1306_MAXROWS;
	}

	if (column > SSD1306_LCDWIDTH)
	{
		column = SSD1306_LCDWIDTH;
	}

	pageAddress[0] = SSD1306_PAGE_START_ADDRESS | (SSD1306_MAXROWS - page);

	columnAddress[0] = SSD1306_COLUMNADDRESS;
	columnAddress[1] = SSD1306_COLUMNADDRESS_MSB | column;
	columnAddress[2] = SSD1306_COLUMNADDRESS_LSB;

	//__no_operation();

	SSD1306SendCommand(pageAddress, 1);
	SSD1306SendCommand(columnAddress, 3);

}

/* Function		: clearScreen(void)
 * Description	: wipes data in memory to 0x00
 * Input		: none
 * Output		: blank screen (0x00)
 */
void clearScreen(void)
{
	byte ramData[] = {0x00};

	byte i,j;

	for(i=0; i < 8 ;i++)
	{
		setAddress(i,0);

	    for(j=0; j < SSD1306_LCDWIDTH; j++)
	    {
	    	SSD1306SendData(ramData, 1);
	    }
	}
}

/* Function		: charDraw(byte row, byte column, int data)
 * Description  : write byte to address
 * Input		: (row, column), data
 * Output		: byte at address
 */
void charDraw(byte row, byte column, int data)
{
	int h;

	if (row > SSD1306_MAXROWS)
	{
		row = SSD1306_MAXROWS;
	}

	if (column > SSD1306_LCDWIDTH)
	{
		column = SSD1306_LCDWIDTH;
	}

	if ( data < 32 || data > 129 )
	{
		data = '.';
	}

	h = (data - 32) * 6;

	setAddress(row, column);
	SSD1306SendData( (byte *)FONT6x8 + h, 6 );

}

/* Function		: stringDraw(byte row, byte column, char *word)
 * Description  : write string to address
 * Input		: (row, column), data
 * Output		: byte at address
 */
void stringDraw( byte row, byte column, char *word)
{
	byte a = 0;

	if (row > SSD1306_MAXROWS)
	{
		row = SSD1306_MAXROWS;
	}

	if (column > SSD1306_LCDWIDTH)
	{
		column = SSD1306_LCDWIDTH;
	}

	while (word[a] != 0)
	{
		if (word[a] != 0x0A)
		{
			if (word[a] != 0x0D)
			{
				charDraw(row, column, word[a]);

				__no_operation();

				column += 6;

				if (column >= (SSD1306_LCDWIDTH - 6 ))
				{
					column = 0;
					if ( row < SSD1306_MAXROWS )
						row++;
					else
						row = 0;
				}

			}
			else
			{
				if ( row < SSD1306_MAXROWS )
					row++;
				else
					row = 0;
				column = 0;
			}
			a++;
		}
	}
}

/* Function		: pixelDraw(byte x, byte y)
 * Description  : draw individual pixel at coordinate
 * Input		: X,Y coordinates
 * Output		: single pixel
 */
void pixelDraw(byte x, byte y)
{
	byte page, temp;
	byte coordinate[] = {0x00};

	if (x > SSD1306_LCDWIDTH - 1)
	{
		x = SSD1306_LCDWIDTH - 1;
	}

	if (y > SSD1306_LCDHEIGHT - 1)
	{
		y = SSD1306_LCDHEIGHT - 1;
	}

	page = y / 8;
	temp = 0x80 >> (y % 8);
	coordinate[0] = temp;

	setAddress(page,x);
	SSD1306SendData(coordinate, 1);

}

/* Function		: horizontalLine(byte xStart, byte xStop, byte y)
 * Description  : draw horizontal line from xStart to xStop
 * Input		: X Start & Stop coordinate, Y
 * Output		: display horizontal line
 */
void horizontalLine(byte xStart, byte xStop, byte y)
{
	byte temp = 0;
	byte page, a;
	byte ramData[] = { 0x00 };

	if (xStart > SSD1306_LCDWIDTH - 1)
	{
		xStart = SSD1306_LCDWIDTH - 1;
	}

	if (xStop > SSD1306_LCDWIDTH - 1)
	{
		xStop = SSD1306_LCDWIDTH - 1;
	}

	if (y > SSD1306_LCDHEIGHT - 1)
	{
		y = SSD1306_LCDHEIGHT - 1;
	}

	if (xStart > xStop)
	{
		temp = xStart;
		xStart = xStop;
		xStop = temp;
	}

	page = y / 8;
	temp = 0x80 >> (y %8);

	a = xStart;
	ramData[0] = temp;

	setAddress(page, xStart);

	while (a <= xStop)
	{
		SSD1306SendData(ramData, 1);
		a++;
	}
}

/* Function		: verticalLine(byte x, byte yStart, byte yStop)
 * Description  : draw vertical line from yStart to yStop
 * Input		: Y Start & Stop coordinate, X
 * Output		: display vertical line
 */
void verticalLine(byte x, byte yStart, byte yStop)
{
	byte temp1 = 0, temp2 = 0;
	byte page1, page2, pageStart;
	byte a;
	byte ramData1[] = {0x00};
	byte ramData2[] = {0x00};

	if (x > SSD1306_LCDWIDTH - 1)
	{
		x = SSD1306_LCDWIDTH - 1;
	}

	if (yStart > SSD1306_LCDHEIGHT - 1)
	{
		yStart = SSD1306_LCDHEIGHT - 1;
	}

	if (yStop > SSD1306_LCDHEIGHT - 1)
	{
		yStop = SSD1306_LCDHEIGHT - 1;
	}

		if (yStart > yStop)
		{
			temp1 = yStart;
			yStart = yStop;
			yStop = temp1;
		}

	page1 = yStart / 8;
	page2 = yStop / 8;
	pageStart = yStart % 8;

	if (page1 != page2)
	{
		if ( pageStart > 0)
		{
			temp1 = 0xFF00 >> pageStart;
			temp1 = temp1 ^ 0xFF;
		}
		else
		{
			temp1 = 0xFF;
		}
	}
	else
	{
		temp1 = 0;

		a = yStart - (page1 * 8);
		a = 0xFF00 >> a;
		temp2 = temp2 ^ a;
	}

	ramData1[0] = temp1;

	setAddress(page1,x);
	SSD1306SendData(ramData1, 1);

	a = page1 + 1;
	ramData1[0] = 0xFF;

	while (a < page2)
	{
		setAddress(a, x);
		SSD1306SendData(ramData1, 1);
		a++;
	}

	temp2 = 8 - (yStop % 8);
	temp2--;
	temp2 = 0xFF << temp2;

	ramData2[0] = temp2;

	setAddress(page2, x);
	SSD1306SendData(ramData2, 1);
}

/* Function		: imageDraw(const byte IMAGE[], byte row, byte column)
 * Description  : draw image at row, column
 * Input		: image, row, column
 * Output		: display image
 */
// from TI_DogsLCD_HAL
void imageDraw(const byte IMAGE[], byte row, byte column)
{
	byte a, height, width;

	width = IMAGE[0];
	height = IMAGE[1];

	for ( a = 0; a < height; a++)
	{
		setAddress(row + a, column);
		SSD1306SendData((byte *)IMAGE + 2 + a * width, width);
	}
}


// circleDraw function is not complete. needs checks for page surround x-axis for proper column fill

/* Function		: circleDraw(register int x, register int y, int r)
 * Description  : draw circle at x,y of radius r
 * Input		: x,y, radius
 * Output		: display circle
 */
void circleDraw(register int x, register int y, int r)
{
    register int xx = -r;
    register int yy = 0;
    register int e = 2 - (2 * r);
    do {
        pixelDraw(x - xx, y + yy);
        pixelDraw(x - yy, y - xx);
        pixelDraw(x + xx, y - yy);
        pixelDraw(x + yy, y + xx);
        if(e >  xx) e += ((++xx << 1) + 1);
        if(e <= yy) e += ((++yy << 1) + 1);
    } while (xx < 0);
}


/* Function    : Fill_RAM( unsigned byte c )
 * Description : Fills the OLED with one particular data
 * Input       : Data
 * Output      : None
 */
void Fill_RAM( byte data )
{
   byte i,j;

   byte ramData[] = {0x00};

   ramData[0] = data;

   for(i=0; i < 8 ;i++)
   {
	  setAddress(i,0);

      for(j=0; j < SSD1306_LCDWIDTH; j++)
      {
         SSD1306SendData(ramData, 1);
      }
   }
}

/* Function    : Fill_RAM_PAGE
 * Description : Fills an OLED page with a particular data
 * Input       : page no.[0-7], Data
 * Output      : None
 */
void Fill_RAM_PAGE( byte page, byte data)
{
   byte j;

   byte ramData[] = { 0x00 };

   ramData[0] = data;

   setAddress(page,0);

   for(j=0; j < SSD1306_LCDWIDTH ;j++)
   {
      SSD1306SendData(ramData, 1);
   }
}

void Set_Contrast_Control( byte d)
{
	byte ramData[] = {SSD1306_SETCONTRAST, 0x00};
	ramData[1] = d;

	SSD1306SendCommand(ramData, 2);
}

void Set_Inverse_Display( byte d)
{
	// 0 = normal, 1 = inverted

	byte ramData[] = {SSD1306_NORMALDISPLAY};
	ramData[0] = ramData[0] + d;

	SSD1306SendCommand(ramData, 1);
}

void Set_Display_On_Off( byte d)	
{
	// 0 = off, 1 = on

	byte ramData[] = {SSD1306_DISPLAYOFF};
	ramData[0] = ramData[0] + d;

	SSD1306SendCommand(ramData, 1);
}
