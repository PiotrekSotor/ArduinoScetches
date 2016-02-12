#include <Keypad.h>

#include <LiquidCrystal.h>


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {3, 2, 11, 10}; //connect to the column pinouts of the keypad



Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
LiquidCrystal lcd (46,48,38,36,34,32);

void setup()
{
  lcd.begin(20,4);
  lcd.cursor();
  Serial.begin(9600);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HELLO WORLD!");
  lcd.setCursor(0,1);
  lcd.print("lcd - klawiatura");
  lcd.setCursor(0,2);
  lcd.print("wprowadzone: ");
  lcd.setCursor(0,3);
  
  
}
char lancuch[20]="";
int licznik = 0;
void loop()
{
  
  
  char customKey = customKeypad.getKey();
  if (customKey)
  {
    lcd.setCursor(0,3);
    lancuch[licznik] = customKey;
    ++licznik;
    if (licznik == 20)
    {
      licznik = 1;
      for (int i=0;i<20;++i)
        lancuch[i]=' ';
      //lancuch = 
      lancuch[0] = customKey;
     
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print("HELLO WORLD!");
       lcd.setCursor(0,1);
       lcd.print("lcd - klawiatura");
       lcd.setCursor(0,2);
       lcd.print("wprowadzone: ");
       lcd.setCursor(0,3);
 
  
      
    }
    lcd.print(lancuch);
    delay(100);
  }
  
  
  //Serial.println(lancuch);
  
  
}

