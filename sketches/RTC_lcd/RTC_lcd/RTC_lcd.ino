#include<Time.h>
#include <LiquidCrystal.h>
#include <DS1302RTC.h>
LiquidCrystal lcd (46,48,38,36,34,32);
DS1302RTC RTC(24,26,28);
  tmElements_t tm;
void setup()
{
 lcd.begin(20,4);
 
  lcd.print("RTC activated");
  Serial.begin(9600);
  

  tm.Second = 0;
  tm.Minute = 58;
  tm.Hour = 23;
  tm.Wday = 2;
  tm.Day = 9;
  tm.Month = 2;
  tm.Year = 45;
  
  time_t t = makeTime(tm);
  lcd.setCursor(0,3);
  lcd.print(t);
  
  
  
  
  
  lcd.setCursor(0,2);
  RTC.haltRTC(true);
  delay(50);
  lcd.print("lol");
  lcd.print(RTC.haltRTC());
  lcd.print("lol");
  delay(1000);
  
  lcd.clear();
  //Serial.print(RTC.write(tm));
  if (!RTC.haltRTC())
  {
    Serial.print(RTC.write(tm));
    lcd.print("Clock was stopped!");
    lcd.print(RTC.haltRTC());
    RTC.haltRTC(false);
  }
  else
    lcd.print("Clock was working.");

  lcd.setCursor(0,1);
  //if (RTC.writeEN())
   // lcd.print("Write allowed.");
  //else
    //lcd.print("Write protected.");

  delay ( 2000 );
  // Setup time library  
  lcd.clear();
  lcd.print("RTC Sync");
  setSyncProvider(RTC.get);          // the function to get the time from the RTC
  if(timeStatus() == timeSet)
    lcd.print(" Ok!");
  else
    lcd.print(" FAIL!");
  
  delay ( 2000 );
  
  lcd.clear();
}

void loop()
{
  if (second() ==10)
       setSyncProvider(RTC.get);
  
  static int sday = 0; // Saved day number for change check

  // Display time centered on the upper line
  lcd.setCursor(3, 0);
  print2digits(hour());
  lcd.print("  ");
  print2digits(minute());
  lcd.print("  ");
  print2digits(second());
  
  // Update in 00:00:00 hour only
  if(sday != day()) {
    // Display abbreviated Day-of-Week in the lower left corner
    lcd.setCursor(0, 1);
    lcd.print(dayShortStr(weekday()));

    // Display date in the lower right corner
    lcd.setCursor(5, 1);
    lcd.print(" ");
    print2digits(day());
    lcd.print("/");
    print2digits(month());
    lcd.print("/");
    lcd.print(year());
  }
  // Warning!
  if(timeStatus() != timeSet) {
    lcd.setCursor(0, 1);
    lcd.print(F("RTC ERROR: SYNC!"));
  }

  // Save day number
  sday = day();

  // Wait small time before repeating :)
  time_t t = makeTime(tm);
  lcd.setCursor(0,3);
  lcd.print(t);
  delay (100);
  
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    lcd.write(' ');
  }
  lcd.print(number);

}
