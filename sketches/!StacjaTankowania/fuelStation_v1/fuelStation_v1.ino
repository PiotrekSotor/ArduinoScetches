#include <LiquidCrystal.h>
#include <Keypad.h>

//----------
//--- KEYBOARD
//----------
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
#define KEYROW0 7
#define KEYROW1 6
#define KEYROW2 5
#define KEYROW3 4

#define KEYCOL0 3
#define KEYCOL1 2
#define KEYCOL2 11
#define KEYCOL3 10

byte rowPins[ROWS] = {KEYROW0, KEYROW1, KEYROW2, KEYROW3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {KEYCOL0, KEYCOL1, KEYCOL2, KEYCOL3}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//---------
//--- LCD
//---------
#define LCD_RS 46
#define LCD_E  48
#define LCD_D4 38
#define LCD_D5 36
#define LCD_D6 34
#define LCD_D7 32

LiquidCrystal lcd (LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

//--------
//--- SD CARD
//---------


//-----------
//--- RTC
//-----------



//----------
//--- GLOBAL VARIABLES
//----------
#define RELAY_PIN

short USERSPINS[20];
char USERSNAMES[20][10];
short VEHICLEID [20];
char VEHICLENAMES [20][10];

int value = 0;
short loggedInUserIndex = -1;
short chosenVehicleIndex = -1;

void readUsers();
void readVehicles();
void login();
void vehicleSelection();
bool getValue(); // true - value get    false - back to selection
void saveOverall();
void saveSpecifiedVehicle();
void saveSpecifiedUser();
bool nextTank(); // true - nastepne tankowanie    false - koniec tankowania 
void endOfWork();

void setup() {
  lcd.begin(20, 4);
  pinMode(RELAY_PIN, OUTPUT);
  // put your setup code here, to run once:

  readUsers();
  while (loggedInUserIndex == -1)
  {
    login();
  }
  do
  {
    while (1)
    {
      value = 0;
      vehicleSelection();
      if (getValue())
        break;
    }
    saveOverall();
    saveSpecifiedVehicle();
    saveSpecifiedUser();
    
  }
  while (nextTank());
  endOfWork();
  
}

void loop() {
  
  delay(9999999);
  // put your main code here, to run repeatedly:

}



bool getValue()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Podaj liczbe litrow");
  lcd.setCursor(0,2);
  lcd.print("A - zatwierdz");
  lcd.setCursor(0,3);
  lcd.print("B - wroc");
  lcd.setCursor(10,3);
  lcd.print("D - kasuj");
  
  
  char customKey;
  while (1)
  {
    customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'A')
        return true;
      else if (customKey == 'B')
        return false;
      else if( customKey = 'D')
        value /=10;
      else if ((int)customKey >=48 && (int)customKey <=57)
     {
        value *=10;
        value += (int)customKey-48;
     }
     lcd.setCursor(0,1);
     for (int i=0;i<20;++i)
       lcd.print(" ");
     lcd.setCursor(0,1);
     lcd.print(value);  
    }
  }
  
  
    
}

//--- wait for hardware
void saveOverall(){}
void saveSpecifiedVehicle(){}
void saveSpecifiedUser(){}



bool nextTank()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Czy zakonczyc prace?");
  lcd.setCursor(0,2);
  lcd.print("C - tak");
  lcd.setCursor(0,3);
  lcd.print("D - nie");
  
  char customKey;
  while (1)
  {
    customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'C')
        return false;
      else if (customKey == 'D')
        return true;
    }
  }
}

void endOfWork()
{
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Wylacz zasilanie");
}
