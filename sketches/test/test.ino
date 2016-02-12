// DS1302_LCD (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Adopted for DS1302RTC library by Timur Maksimov 2014
//
// A quick demo of how to use my DS1302-library to make a quick
// clock using a DS1302 and a 16x2 LCD.
//
// I assume you know how to connect the DS1302 and LCD.
// DS1302:  CE pin    -> Arduino Digital 27
//          I/O pin   -> Arduino Digital 29
//          SCLK pin  -> Arduino Digital 31
//          VCC pin   -> Arduino Digital 33
//          GND pin   -> Arduino Digital 35
// LCD:     DB7       -> Arduino Digital 7
//          DB6       -> Arduino Digital 6 
//          DB5       -> Arduino Digital 5
//          DB4       -> Arduino Digital 4
//          E         -> Arduino Digital 9
//          RS        -> Arduino Digital 8

#include <Wire.h>          // Need for NewLiquidCrystal library
#include <LiquidCrystal.h>
#include <DS1302RTC.h>
#include <Time.h>

// Init the DS1302
// Set pins:  CE, IO,CLK
DS1302RTC RTC(7, 6, 5);

// Optional connection for RTC module
#define DS1302_GND_PIN 4
#define DS1302_VCC_PIN 3

// Init the LCD
//   initialize the library with the numbers of the interface pins
//            lcd(RS,  E, d4, d5, d6, d7, bl, polarity)
//LiquidCrystal lcd(8,   9,  4,  5,  6,  7, 10, POSITIVE);

void setup()
{
  // Setup LCD to 16x2 characters
  //lcd.begin(16, 2);
  Serial.begin(9600);
  // Activate RTC module
  digitalWrite(DS1302_GND_PIN, LOW);
  pinMode(DS1302_GND_PIN, OUTPUT);

  digitalWrite(DS1302_VCC_PIN, HIGH);
  pinMode(DS1302_VCC_PIN, OUTPUT);

  Serial.print("RTC activated");

  delay(500);
  
  //lcd.clear();
  if (RTC.haltRTC())
    Serial.print("Clock stopped!");
  else
    Serial.print("Clock working.");

 // Serial.setCursor(0,1);
  if (RTC.writeEN())
    Serial.print("Write allowed.");
  else
    Serial.print("Write protected.");

  delay ( 2000 );
  
  // Setup time library  
  //lcd.clear();
  
  Serial.print("RTC Sync");
  setSyncProvider(RTC.get);          // the function to get the time from the RTC
  if(timeStatus() == timeSet)
    Serial.print(" Ok!");
  else
    Serial.print(" FAIL!");
  
  delay ( 2000 );
  
  //lcd.clear();
}

void loop()
{
  static int sday = 0; // Saved day number for change check

  // Display time centered on the upper line
  //lcd.setCursor(3, 0);
  print2digits(hour());
  Serial.print("  ");
  print2digits(minute());
  Serial.print("  ");
  print2digits(second());
  
  // Update in 00:00:00 hour only
  if(sday != day()) {
    // Display abbreviated Day-of-Week in the lower left corner
    //lcd.setCursor(0, 1);
    Serial.print(dayShortStr(weekday()));

    // Display date in the lower right corner
    //lcd.setCursor(5, 1);
    Serial.print(" ");
    print2digits(day());
    Serial.print("/");
    print2digits(month());
   Serial.print("/");
    Serial.print(year());
  }
  // Warning!
  if(timeStatus() != timeSet) {
    //lcd.setCursor(0, 1);
    Serial.print(F("RTC ERROR: SYNC!"));
  }

  // Save day number
  sday = day();
Serial.println();
  // Wait small time before repeating :)
  delay (1000);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('_');
  }
  Serial.print(number);
}
