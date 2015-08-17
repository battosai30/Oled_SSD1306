#include <SPI.h>
#include <FontPack.h>
#include <Oled_SSD1306.h>

#define DC_OLED P2_5
#define CS_OLED P4_2
#define RST_OLED P2_6

void setup()
{
  SSD1306PinSetup(CS_OLED,DC_OLED,RST_OLED);
  SSD1306Init();
  clearScreen();
}

void loop()
{
  
  circleDraw(64, 32, 10);
  delay(2000);
  clearScreen();
  horizontalLine(10, 120, 30);
  delay(2000);
  clearScreen();
  verticalLine(30, 10, 60);
  delay(2000);
  clearScreen();
  stringDraw( 2, 2, "Hello world");
  delay(2000);
  clearScreen();
  
}
