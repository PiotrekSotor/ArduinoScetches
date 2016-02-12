#include <virtuabotixRTC.h>

#include<Time.h>
#include <LiquidCrystal.h>
#include <DS1302RTC.h>
LiquidCrystal lcd (46,48,38,36,34,32);
//DS1302RTC RTC(24,26,28);
virtuabotixRTC myRTC (28,26,24);
  tmElements_t tm;
void setup()
{
 lcd.begin(20,4);
 
  lcd.print("RTC activated");
  Serial.begin(9600);
  

  tm.Second = 0;
  tm.Minute = 10;
  tm.Hour = 22;
  tm.Wday = 3;
  tm.Day = 10;
  tm.Month = 2;
  tm.Year = 45;
  
  time_t t = makeTime(tm);
  lcd.setCursor(0,3);
  lcd.print(t);
  
  myRTC.setDS1302Time(00,59,23,6,10,1,2014);
  
  
  
  
  lcd.setCursor(0,2);
  //RTC.haltRTC(false);
  delay(50);
  lcd.print("lol");
  //lcd.print(RTC.haltRTC());
  lcd.print("lol");
  delay(1000);
  
  lcd.clear();
 
    //RTC.haltRTC(true);
    //Serial.print(RTC.set(t));
    lcd.print("Time set!");
    //lcd.print(RTC.haltRTC());
    //RTC.haltRTC(false);


  delay ( 2000 );
  // Setup time library  
  lcd.clear();
  lcd.print("RTC Sync");
  
  
  
  
  //setSyncProvider(RTC.get);          // the function to get the time from the RTC
  if(timeStatus() == timeSet)
    lcd.print(" Ok!");
  else
    lcd.print(" FAIL!");
  
  delay ( 2000 );
  
  lcd.clear();
}

void loop()
{
  
  myRTC.updateTime();
  
  // Start printing elements as individuals
  Serial.print("Current Date / Time: ");
  Serial.print(myRTC.dayofmonth);
  Serial.print("/");
  Serial.print(myRTC.month);
  Serial.print("/");
  Serial.print(myRTC.year);
  Serial.print(" ");
  Serial.print(myRTC.hours);
  Serial.print(":");
  Serial.print(myRTC.minutes);
  Serial.print(":");
  Serial.println(myRTC.seconds);
  
  // Delay so the program doesn't print non-stop
  delay( 5000);

  
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write(' ');
  }
  lcd.print(number);

}
