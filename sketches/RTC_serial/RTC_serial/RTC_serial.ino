#include <Time.h>

#include <DS1302RTC.h>

#define DS_GND 4
#define DS_VCC 3

#define DS_CE 7
#define DS_IO 6
#define DS_SCLK 5

DS1302RTC rtc(DS_CE, DS_IO, DS_SCLK);
tmElements_t tm;
time_t t;
bool setTimeFlag = true;


void setup() {
  Serial.begin(9600);
  rtc.haltRTC(true);
  delay(100);
    
  if (setTimeFlag)
  {
    
    
    Serial.print("rtc: ");
    Serial.println(rtc.haltRTC());
    
    
    tm.Second = 0;
    tm.Minute = 25;
    tm.Hour = 19;
    tm.Wday = 6;
    tm.Day = 13;
    tm.Month = 2;
    tm.Year = 45;
    
    t = makeTime(tm);
    
    
    // put your setup code here, to run once:

  }
  pinMode(DS_GND,OUTPUT);
  digitalWrite(DS_GND,LOW);
  pinMode(DS_VCC,OUTPUT);
  digitalWrite(DS_VCC,HIGH);
  
  delay(500);
  Serial.println("Start");
  if (rtc.haltRTC())
  {
    Serial.println("haltRTC() = 1\nClock stopped");
    rtc.haltRTC(false);
    while(rtc.haltRTC())
      Serial.println("while");
  }
  else
  {
    Serial.println("haltRTC() = 0\nClock works");
  }
  if (setTimeFlag)
   rtc.set(t);
  delay(100);
  setSyncProvider(rtc.get);          // the function to get the time from the RTC
  setSyncInterval(10);
  if(timeStatus() == timeSet)
    Serial.println(" Sync Ok!");
  else
    Serial.println(" Sync FAIL!");
  rtc.haltRTC(true);
 }

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print("rtc: ");
  Serial.println(rtc.haltRTC());
  
  print2digits(hour());
  Serial.print(" : ");
  print2digits(minute());
  Serial.print(" : ");
  print2digits(second());
  Serial.print("   ");
  Serial.print(day());
  Serial.print("  ");
  Serial.print(month());
  Serial.print("  ");
  Serial.print(year());
  Serial.print("  ");
  
  Serial.println();
  
  delay(5000);
 }


void print2digits(int number) {
  if (number >= 0 && number < 10)
    Serial.write('0');
  Serial.print(number);
}

/*
rst - 7
dat - 6
clk - 5
gnd - 4
vcc - 3
*/
