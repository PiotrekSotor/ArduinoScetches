#include <SD.h>


#include <LiquidCrystal.h>
#define _SD_CS_pin 53
LiquidCrystal lcd (46,48,38,36,34,32);


void setup()
{
  lcd.begin(20,4);
 
  Serial.begin(9600);
  
//  pinMode(53,OUTPUT); // obowiazkowe na MEGA, na UNO -> pin 10
  lcd.setCursor(0,0);
  if (!SD.begin())
  {
     
     lcd.print("Blad karty SD");
  }
  else
     lcd.print("Karta SD OK");  
 
  lcd.setCursor(0,1);
  lcd.print("end of setup");
  delay(2000);
  
  File file = SD.open("test.txt");
  lcd.setCursor(0,2);
  if (file)
  {
   lcd.print("Otwarto plik");     
   while (file.available())
    {
      lcd.setCursor(0,3);
      lcd.print(file.read());
    }
    file.close(); 
  }
  else
    lcd.print("Blad otwarcia pliku");
  
  
  
  
  
  
  
  
//  lcd.clear();
}

void loop()
{
 
}


