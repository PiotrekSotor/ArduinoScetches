#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int klik=0;
int licznik=0;
int pin = 0;


void  loop()
{
  int suma=0;
  int stanPinu = analogRead(pin);
  suma+=stanPinu;
  stanPinu=analogRead(pin);
  suma+=stanPinu;
  stanPinu=analogRead(pin);
  suma+=stanPinu;
  suma/=3;
  if ( stanPinu == HIGH)
  {
    ++licznik;
    klik=stanPinu;  
  }
  lcd.clear();
  lcd.print("Licznik: ");
  lcd.setCursor(0,1);
  lcd.print(licznik);
  lcd.print("  ");
  lcd.print(stanPinu);
  lcd.print(" ");
  lcd.print(suma);
  Serial.print(stanPinu);
  Serial.print("  ");
  Serial.println(suma);
  delay(200);  
}
void setup()
{
  Serial.begin(9600);
  pinMode(pin,INPUT);
  licznik = 0;
  lcd.begin(16,2);
  lcd.print("");
}
