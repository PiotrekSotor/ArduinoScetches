#include <OneWire.h>




#include <DallasTemperature.h>



#include<LiquidCrystal.h>

#define ONE_WIRE_BUS 7
OneWire oneWire (ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(12,11,5,4,3,2);
void setup (void)
{
   Serial.begin(9600);
   sensors.begin();
   
   lcd.begin(16,2);
   
  
}
void loop(void)
{
   sensors.requestTemperatures();
  Serial.println("");
  lcd.setCursor (0,0);
 lcd.print ("tempC: ");
 lcd.print(sensors.getTempCByIndex(0));
 lcd.setCursor(0,1);
 lcd.print("tempF: ");
 lcd.print(sensors.getTempFByIndex(0));
 delay(500);
}
