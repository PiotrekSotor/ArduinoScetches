#include <TFT.h>

#include <SPI.h>

//#include <Adafruit_ILI9341.h>

//#include <Adafruit_GFX.h>


#define _cs 40
#define _dc 42
#define _rst 44
#define _sclk 52
#define _miso 50
#define _mosi 51

//Adafruit_ILI9341 tft = Adafruit_ILI9341 (_cs, _dc, _rst);
TFT screen = TFT (_cs, _dc, _rst);

void setup()
{
  Serial.begin(9600);
  
  
  //tft.begin();
  screen.begin();
  //tft.fillScreen(ILI9341_BLACK);
  Serial.println("setup!");
  screen.background(0,0,0);
  Serial.println("koniec setup");
  
  
}


void loop()
{
   //tft.fillScreen(ILI9341_WHITE);
   screen.background(255,255,255);
   delay(1000);
   unsigned int time=micros();
   //tft.fillScreen(ILI9341_BLACK);
   screen.background(0,0,0);
   time = micros()-time;
   Serial.println(time);
   delay(1000);
   Serial.println("loop");
  
  
}
