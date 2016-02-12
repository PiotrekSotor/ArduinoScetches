#include <Wire.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <SD.h>
#include <SPI.h>


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




//-------
//--- znaki wlasne
//------------
byte arrowUp[8] = {    // character 0
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
};
byte arrowDown[8] = {    // character 1
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
};
//----------
//--- GLOBAL VARIABLES
//----------
#define RELAY_PIN 13
int LICZBA_UZYTKOWNIKOW = 0;
String USERPINS[20];
String USERSNAMES[20];
int LICZBA_POJAZDOW = 0;
int VEHICLEID [20];
String VEHICLENAMES [20];
char RESULT_DATE[20];
String readWord;

int value = 0;
int overallValue = 0;
int destinationValue = 0;
short loggedInUserIndex = -1;
short chosenVehicleIndex = -1;

void readUsers();
void readVehicles();
void login();
void validDate(); //  done   jesli data zapisana w arduino jest aktualna to ok, a jesli nie to zapisuje nowa date
void vehicleSelection();
bool getValue(); // done  true - value get    false - back to selection
void saveOverall();
void saveSpecifiedVehicle();
void saveSpecifiedUser();
bool nextTank(); //  done    true - nastepne tankowanie    false - koniec tankowania
void endOfWork(); // done
void printSeq(int col, int row, char text[20]); // done
void makeDate(int dzien, int miesiac, int rok, int godzina, int minuta); //done
void listVehicles();
void readWordFromFile (File file);
void readOverallValue();
void readLastDelivery();
bool isDelivery();
void delivery();
void remaininigLiters();

//-------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  lcd.createChar(0, arrowUp);
  lcd.createChar(1, arrowDown);
  lcd.begin(20, 4);
  pinMode(10, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
//  pinMode(DS_GND, OUTPUT);
//  digitalWrite(DS_GND, LOW);
//  pinMode(DS_VCC, OUTPUT);
//  digitalWrite(DS_VCC, HIGH);
  // put your setup code here, to run once:
  SD.begin(10, 11, 12, 13);



  readUsers();
  while (loggedInUserIndex == -1)
  {
    login();
  }
  readOverallValue();
  readLastDelivery();
  validDate();
  if (isDelivery())
  {
    delivery();
  }
  else
  {
    remaininigLiters();
    readVehicles();
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
  }
  endOfWork();

}
//-----------------------------------------------------------------------------
void loop() {
  delay(9999999);
}

void readUsers()
{
  File userFile;
  LICZBA_UZYTKOWNIKOW = 0;
  userFile = SD.open ("OSOBY/list.txt", FILE_READ);
  if (!userFile)
    Serial.println("userFile error");
  while (userFile.available())
  {
    readWordFromFile(userFile);
    Serial.println(readWord);
    USERSNAMES[LICZBA_UZYTKOWNIKOW] = readWord;
    readWordFromFile(userFile);
    Serial.println(readWord);
    USERPINS[LICZBA_UZYTKOWNIKOW++] = readWord;
  }
  userFile.close();
}
void readVehicles()
{
  File vehicleFile;
  LICZBA_POJAZDOW = 0;
  vehicleFile = SD.open ("POJAZDY/list.txt", FILE_READ);
  if (!vehicleFile)
    Serial.println("vecFile error");
  while (vehicleFile.available())
  {
    readWordFromFile(vehicleFile);
    Serial.println(readWord + " 1" + readWord.toInt() + "1");
    VEHICLEID[LICZBA_POJAZDOW] = readWord.toInt();
    readWordFromFile(vehicleFile);
    Serial.println(readWord);
    VEHICLENAMES[LICZBA_POJAZDOW++] = readWord;
  }
  vehicleFile.close();

}
void login()
{
  lcd.clear();
  lcd.print("Wprowadz pin");
  lcd.setCursor(0, 2);
  lcd.print("A - zatwierdz");
  lcd.setCursor(0, 3);
  lcd.print("D - kasuj");

  int pin = -1;
  int wprowadzoneZnaki = 0;
  while (1)
  {
    char customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'A')
      {

        for (int i = 0; i < LICZBA_UZYTKOWNIKOW; ++i)
        {
          if (String(pin).equals(USERPINS[i]))
          {
            loggedInUserIndex = i;
            lcd.clear();
            digitalWrite(RELAY_PIN, HIGH);
            lcd.print("Zalogowano");
            delay(1000);
            return;
          }
        }
        return;
      }
      else if (customKey == 'D')
      {
        if (wprowadzoneZnaki)
          wprowadzoneZnaki--;
        if (pin < 10)
          pin = -1;
        else
          pin /= 10;
      }
      else if ((int)customKey >= 48 && (int)customKey <= 57)
      {
        wprowadzoneZnaki++;
        if (pin == -1)
          pin = 0;
        pin *= 10;
        pin += (int)customKey - 48;
      }

      lcd.setCursor(0, 1);
      for (int i = 0; i < 20; ++i)
        lcd.print(' ');
      lcd.setCursor(0, 1);
      lcd.print(pin);
      for (int i = 0; i < wprowadzoneZnaki; ++i)
        lcd.print('*');

    }
  }
}
void vehicleSelection()
{
  lcd.clear();
  lcd.print("ID: ");//4 znakow
  lcd.setCursor(11, 0);
  lcd.print("A - ok");
  lcd.setCursor(11, 1);
  lcd.print("B - ");
  lcd.write(byte(0));
  lcd.setCursor(11, 2);
  lcd.print("C - ");
  lcd.write(byte(1));
  lcd.setCursor(11, 3);
  lcd.print("D - kasuj");

  int wpisane = 0;
  lcd.setCursor(4, 0);
  lcd.print(wpisane);
  chosenVehicleIndex = 0;
  listVehicles();
  while (1)
  {
    char customKey = customKeypad.getKey();

    if (customKey)
    {
      Serial.println((int)customKey);
      if (customKey == 'A')
      {
        return;
      }
      else if (customKey == 'B')
      {

        if (chosenVehicleIndex != 0)
        {
          --chosenVehicleIndex;
          wpisane--;
        }

      }
      else if (customKey == 'C')
      {

        if (chosenVehicleIndex != LICZBA_POJAZDOW - 1)
        {
          ++chosenVehicleIndex;
          wpisane++;
        }
      }
      else if (customKey == 'D')
      {

        if (wpisane != -1)
        {
          wpisane /= 10;
          if (wpisane == 0)
            wpisane = 0;
          chosenVehicleIndex /= 10;
        }
      }
      else if ((int)customKey >= 48 && (int)customKey <= 57)
      {
        if (wpisane == -1)
          wpisane = 0;
        wpisane *= 10;
        wpisane += (int)customKey - 48;
        chosenVehicleIndex = wpisane;
      }
      listVehicles();

      lcd.setCursor(4, 0);
      for (int i = 0; i < 6; ++i)
        lcd.print(" ");
      if (wpisane != -1)
      {
        lcd.setCursor(4, 0);
        lcd.print(wpisane);
      }
    }
  }
}


void validDate()
{
  lcd.clear();
  tmElements_t tm;
  if (!RTC.read(tm))
  {
    lcd.print("Problem z RTC sync");
    delay(1000);
  }
  lcd.clear();
  lcd.print("Czy data jest ok?");
  lcd.setCursor(2, 1);
  lcd.print(tm.Day);
  lcd.print(".");
  lcd.print(tm.Month);
  lcd.print(".");
  lcd.print(tmYearToCalendar(tm.Year));
  lcd.print("  ");
  lcd.print(tm.Hour);
  lcd.print(":");
  lcd.print(tm.Minute);

  lcd.setCursor(0, 3);
  lcd.print("A - tak");
  lcd.setCursor(10, 3);
  lcd.print("B - nie");
  char customKey;
  while (1)
  {
    customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'A')
        return;
      else if (customKey == 'B')
      {
        int wprowadzoneZnaki = 0;
        int dzien = 0;
        int miesiac = 0;
        int rok = 0;
        int godzina = -1;
        int minuta = -1;

        lcd.clear();
        lcd.print("Wprowadz date");
        if (wprowadzoneZnaki == 12)
        {
          lcd.setCursor(0, 3);
          lcd.print("A - zatwierdz");
        }
        else
        {
          lcd.setCursor(0, 3);
          lcd.print("B - dalej");
        }
        lcd.setCursor(10, 3);
        lcd.print("D - kasuj");
        lcd.setCursor(2, 1);
        lcd.print("dd.mm.yyyy hh:mm");
        makeDate(dzien, miesiac, rok, godzina, minuta);
        printSeq(2, 2, RESULT_DATE);
        while (1)
        {
          customKey = customKeypad.getKey();
          if (customKey)
          {
            if ((int)customKey >= 48 && (int)customKey <= 57 && wprowadzoneZnaki < 12)
            {
              if (wprowadzoneZnaki < 2)
              {
                dzien *= 10;
                dzien += (int)customKey - 48;
              }
              else if (wprowadzoneZnaki < 4)
              {
                miesiac *= 10;
                miesiac += (int)customKey - 48;
                // miesiac
              }
              else if (wprowadzoneZnaki < 8)
              {
                rok *= 10;
                rok += (int)customKey - 48;
                Serial.println(rok);
                // rok
              }
              else if (wprowadzoneZnaki < 10)
              {
                if (godzina == -1)
                  godzina = 0;
                godzina *= 10;
                godzina += (int)customKey - 48;
                // godzina
              }
              else if (wprowadzoneZnaki < 12)
              {
                if (minuta == -1)
                  minuta = 0;
                minuta *= 10;
                minuta += (int)customKey - 48;
                //minuta
              }
              ++wprowadzoneZnaki;
            }
            else if (customKey == 'D' && wprowadzoneZnaki != 0)
            {
              if (wprowadzoneZnaki < 2)
              {
                dzien /= 10;
                wprowadzoneZnaki--;
              }
              else if (wprowadzoneZnaki < 4)
              {
                miesiac /= 10;
                wprowadzoneZnaki--;
                // miesiac
              }
              else if (wprowadzoneZnaki < 8)
              {
                rok /= 10;
                wprowadzoneZnaki--;
                // rok
              }
              else if (wprowadzoneZnaki < 10)
              {
                godzina /= 10;
                wprowadzoneZnaki--;
                if (!(wprowadzoneZnaki % 2))
                  godzina = -1;
                // godzina
              }
              else if (wprowadzoneZnaki < 12)
              {
                minuta /= 10;
                wprowadzoneZnaki--;
                if (!(wprowadzoneZnaki % 2))
                  minuta = -1;
                // minuta
              }
            }
            else if (customKey == 'A' && wprowadzoneZnaki == 12)
            {
              tmElements_t tm;
              tm.Second = 0;
              tm.Minute = minuta;
              tm.Hour = godzina;
              tm.Day = dzien;
              tm.Month = miesiac;
              tm.Year = rok;
              tm.Wday = 0;
              RTC.write(tm);
              delay(500);
              return;
              // ustawienie rtc...
            }
            else if (customKey == 'B' && wprowadzoneZnaki < 12)
            {
              if (wprowadzoneZnaki < 4)
              {
                wprowadzoneZnaki += 2 - wprowadzoneZnaki % 2;
              }
              else if (wprowadzoneZnaki < 8)
              {
                wprowadzoneZnaki += 4 - wprowadzoneZnaki % 4;
              }
              else if (wprowadzoneZnaki < 12)
              {
                wprowadzoneZnaki += 2 - wprowadzoneZnaki % 2;
              }

            }
            if (wprowadzoneZnaki == 12)
            {
              lcd.setCursor(0, 3);
              lcd.print("A - ok   ");
            }
            else
            {
              lcd.setCursor(0, 3);
              lcd.print("B - dalej");
            }
            makeDate(dzien, miesiac, rok, godzina, minuta);
            printSeq(2, 2, RESULT_DATE);
            Serial.println(wprowadzoneZnaki);
          }
        }
      }
    }
  }
}
bool getValue()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Podaj liczbe litrow");
  lcd.setCursor(0, 2);
  lcd.print("A - zatwierdz");
  lcd.setCursor(0, 3);
  lcd.print("B - wroc");
  lcd.setCursor(10, 3);
  lcd.print("D - kasuj");
  lcd.setCursor(0, 1);
  lcd.print(value);

  char customKey;
  while (1)
  {
    customKey = customKeypad.getKey();
    if (customKey)
    {
      Serial.println((int)customKey);
      if (customKey == 'A')
        return true;
      else if (customKey == 'B')
        return false;
      else if ( customKey == 'D')
        value /= 10;
      else if ((int)customKey >= 48 && (int)customKey <= 57) //  etwas ist nicht ja...
      {
        value *= 10;
        value += (int)customKey - 48;
        Serial.println("  LOLO");
        Serial.println((int)customKey);
        Serial.println(value);
      }
      lcd.setCursor(0, 1);
      for (int i = 0; i < 20; ++i)
        lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(value);
      Serial.println(value);
    }
  }



}
void saveOverall() {
  File logFile;

  logFile = SD.open("log.txt", FILE_WRITE);
  if (!logFile)
    Serial.println("logFile error");
  tmElements_t tm;
  RTC.read(tm);

  logFile.print(String() + tm.Day + "." + tm.Month + "." + tmYearToCalendar(tm.Year) + "\t");
  logFile.print(String() + tm.Hour + ":" + tm.Minute + "\t");
  logFile.print(USERSNAMES[loggedInUserIndex] + "\t");
  logFile.print(VEHICLENAMES[chosenVehicleIndex] + "\t");
  logFile.println(value);
  logFile.close();
  Serial.println("saveOverall - finished");

}
void saveSpecifiedVehicle() {
  if (!SD.exists("POJAZDY"))
    SD.mkdir("POJAZDY");

  tmElements_t tm;
  RTC.read(tm);
  String strPath;
  if (VEHICLENAMES[chosenVehicleIndex].length() > 7)
  {
    strPath = "POJAZDY/" + VEHICLENAMES[chosenVehicleIndex].substring(0, 8) + ".txt";
  }
  else
  {
    strPath = "POJAZDY/" + VEHICLENAMES[chosenVehicleIndex] + ".txt";
  }
  Serial.println("q" + strPath + "q");
  File vehicleFile;
  vehicleFile.close();
  vehicleFile = SD.open( strPath.c_str(), FILE_WRITE);
  if (!vehicleFile)
    Serial.println("\t\t\tvehicleFile error");
  vehicleFile.print(String() + tm.Day + "." + tm.Month + "." + tmYearToCalendar(tm.Year) + "\t");
  vehicleFile.print(String() + tm.Hour + ":" + tm.Minute + "\t");
  vehicleFile.print(USERSNAMES[loggedInUserIndex] + "\t");
  vehicleFile.print(VEHICLENAMES[chosenVehicleIndex] + "\t");
  vehicleFile.println(value);
  vehicleFile.close();
  Serial.println("saveVehicle - finished");
}
void saveSpecifiedUser() {
  if (!SD.exists("OSOBY"))
    SD.mkdir("OSOBY");

  String strPath;
  if (VEHICLENAMES[chosenVehicleIndex].length() > 7)
  {
    strPath = "OSOBY/" + USERSNAMES[loggedInUserIndex].substring(0, 8) + ".txt";
  }
  else
  {
    strPath = "OSOBY/" + USERSNAMES[loggedInUserIndex] + ".txt";
  }

  File userFile;
  userFile.close();
  userFile = SD.open(strPath.c_str(), FILE_WRITE);
  if (userFile)
  {
    tmElements_t tm;
    RTC.read(tm);

    Serial.println("1");
    Serial.println(strPath + "q");
    Serial.println("2");
    userFile.print(String() + tm.Day + "." + tm.Month + "." + tmYearToCalendar(tm.Year) + "\t");
    userFile.print(String() + tm.Hour + ":" + tm.Minute + "\t");
    userFile.print(USERSNAMES[loggedInUserIndex] + "\t");
    userFile.print(VEHICLENAMES[chosenVehicleIndex] + "\t");
    userFile.println(value);
    userFile.close();
  }
  else
  {
    Serial.println("error");
  }


  Serial.println("saveUser - finished");
}



bool nextTank()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Czy zakonczyc prace?");
  lcd.setCursor(0, 2);
  lcd.print("A - tak");
  lcd.setCursor(0, 3);
  lcd.print("D - nie");

  char customKey;
  while (1)
  {
    customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'A')
        return false;
      else if (customKey == 'D')
        return true;
    }
  }
}

void endOfWork()
{
  digitalWrite(RELAY_PIN, LOW);
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("Wylacz zasilanie");
}

void printSeq(int col, int row, char text[20])
{
  lcd.setCursor(col, row);
  for (int i = 0; i < 20 - col; ++i)
    lcd.print(" ");
  lcd.setCursor(col, row);
  for (int i = 0; i < 20 - col; ++i)
    lcd.print(text[i]);
}
void makeDate(int dzien, int miesiac, int rok, int godzina, int minuta)
{

  RESULT_DATE[16] = '\0';
  if (dzien == 0)
    RESULT_DATE[0] = RESULT_DATE[1] = '_';
  else
  {
    if (dzien < 10)
    {
      RESULT_DATE[0] = '0';
      RESULT_DATE[1] = (char)dzien + 48;
    }
    else
    {
      int cyfra = dzien % 10;
      RESULT_DATE[1] = (char)cyfra + 48;
      dzien /= 10;
      RESULT_DATE[0] = (char)dzien + 48;
    }
  }
  RESULT_DATE[2] = '.';
  if (miesiac == 0)
    RESULT_DATE[3] = RESULT_DATE[4] = '_';
  else
  {
    if (miesiac < 10)
    {
      RESULT_DATE[3] = '0';
      RESULT_DATE[4] = (char)miesiac + 48;
    }
    else
    {
      int cyfra = miesiac % 10;
      RESULT_DATE[4] = (char)cyfra + 48;
      miesiac /= 10;
      RESULT_DATE[3] = (char)miesiac + 48;
    }
  }
  RESULT_DATE[5] = '.';
  if (rok == 0)
    RESULT_DATE[6] = RESULT_DATE[7] = RESULT_DATE[8] = RESULT_DATE[9] = '_';
  else
  {
    for (int i = 3; i > -1; --i)
    {
      RESULT_DATE[6 + i] = (char)((rok % 10) + 48);
      Serial.print("rok ");
      Serial.print(rok);
      rok /= 10;
      Serial.print("   RESULT_DATE ");
      Serial.print(6 + i);
      Serial.print("  ");
      Serial.println((int)RESULT_DATE[6 + i]);
    }
  }
  RESULT_DATE[10] = ' ';
  if (godzina == -1)
    RESULT_DATE[11] = RESULT_DATE[12] = '_';
  else
  {
    if (godzina < 10)
    {
      RESULT_DATE[11] = '0';
      RESULT_DATE[12] = (char)godzina + 48;
    }
    else
    {
      int cyfra = godzina % 10;
      RESULT_DATE[12] = (char)cyfra + 48;
      godzina /= 10;
      RESULT_DATE[11] = (char)godzina + 48;
    }
  }
  RESULT_DATE[13] = ':';
  if (minuta == -1)
    RESULT_DATE[14] = RESULT_DATE[15] = '_';
  else
  {
    if (minuta < 10)
    {
      RESULT_DATE[14] = '0';
      RESULT_DATE[15] = (char)minuta + 48;
    }
    else
    {
      int cyfra = minuta % 10;
      RESULT_DATE[15] = (char)cyfra + 48;
      minuta /= 10;
      RESULT_DATE[14] = (char)minuta + 48;
    }
  }
  RESULT_DATE[16] = RESULT_DATE[17] = RESULT_DATE[18] = RESULT_DATE[19] = ' ';
}

void listVehicles()
{
  lcd.setCursor(0, 1);
  for (int i = 0; i < 10; i++)
    lcd.print(' ');
  lcd.setCursor(0, 2);
  for (int i = 0; i < 10; i++)
    lcd.print(' ');
  lcd.setCursor(0, 3);
  for (int i = 0; i < 10; i++)
    lcd.print(' ');

  if (chosenVehicleIndex == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(VEHICLEID[0]);
    lcd.print('+');
    lcd.print(VEHICLENAMES[0]);

    lcd.setCursor(0, 2);
    lcd.print(VEHICLEID[1]);
    lcd.print(' ');
    lcd.print(VEHICLENAMES[1]);

    lcd.setCursor(0, 3);
    lcd.print(VEHICLEID[2]);
    lcd.print(' ');
    lcd.print(VEHICLENAMES[2]);
  }
  else if (chosenVehicleIndex == LICZBA_POJAZDOW - 1)
  {
    lcd.setCursor(0, 1);
    lcd.print(VEHICLEID[LICZBA_POJAZDOW - 3]);
    lcd.print(' ');
    lcd.print(VEHICLENAMES[LICZBA_POJAZDOW - 3]);

    lcd.setCursor(0, 2);
    lcd.print(VEHICLEID[LICZBA_POJAZDOW - 2]);
    lcd.print(' ');
    lcd.print(VEHICLENAMES[LICZBA_POJAZDOW - 2]);

    lcd.setCursor(0, 3);
    lcd.print(VEHICLEID[LICZBA_POJAZDOW - 1]);
    lcd.print('+');
    lcd.print(VEHICLENAMES[LICZBA_POJAZDOW - 1]);
  }
  else if (chosenVehicleIndex > 0 && chosenVehicleIndex < LICZBA_POJAZDOW - 1)
  {
    lcd.setCursor(0, 1);
    lcd.print(VEHICLEID[chosenVehicleIndex - 1]);
    lcd.print(' ');
    lcd.print(VEHICLENAMES[chosenVehicleIndex - 1]);

    lcd.setCursor(0, 2);
    lcd.print(VEHICLEID[chosenVehicleIndex]);
    lcd.print('+');
    lcd.print(VEHICLENAMES[chosenVehicleIndex]);

    lcd.setCursor(0, 3);
    lcd.print(VEHICLEID[chosenVehicleIndex + 1]);
    lcd.print(' ');
    lcd.print(VEHICLENAMES[chosenVehicleIndex + 1]);
  }
}

void readWordFromFile (File file)
{
  readWord = String();
  char c;
  do {
    c = file.read();
    //Serial.println("1" + String(c) + "1");
    readWord += String(c);

  }
  while ( c != ' ' && (int)c != -1 && c != '\n' && c != '\t');
  //readWord[index++] = '\0';
  readWord = readWord.substring(0, readWord.length() - 1);
  //Serial.println("qqqqq" + readWord + "qqqqq");
}

void readOverallValue()
{
  File logFile;
  logFile = SD.open("LOG.TXT", FILE_READ);
  if (!logFile)
  {
    Serial.println("readOverallValue logFile error");
  }
  else
  {
    logFile.seek(logFile.size());
    while (logFile.peek() != ' ')
      logFile.seek(logFile.position() - 1);

    readWordFromFile(logFile); // overall
    overallValue = readWord.toInt();


    logFile.close();
  }

}
void readLastDelivery()
{
  File deliveryFile;
  deliveryFile= SD.open("DELIVERY.TXT", FILE_READ);
  if (!deliveryFile)
  {
    Serial.println("readLastDelivery deliveryFile error");
  }
  else
  {
    deliveryFile.seek(deliveryFile.size());
    while (deliveryFile.peek() != ' ')
      deliveryFile.seek(deliveryFile.position() - 1);

    readWordFromFile(deliveryFile); // destination
    destinationValue = readWord.toInt();
    deliveryFile.close();
  }
}
bool isDelivery()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wybierz?");
  lcd.setCursor(0, 2);
  lcd.print("A - tankowanie");
  lcd.setCursor(0, 3);
  lcd.print("B - dostawa");

  char customKey;
  while (1)
  {
    customKey = customKeypad.getKey();
    if (customKey)
    {
      if (customKey == 'A')
        return false;
      else if (customKey == 'B')
        return true;
    }
  }

}
void delivery()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Podaj liczbe litrow");
  lcd.setCursor(0, 2);
  lcd.print("A - zatwierdz");
  lcd.setCursor(10, 3);
  lcd.print("D - kasuj");
  lcd.setCursor(0, 1);
  lcd.print(value);

  char customKey;
  while (1)
  {
    customKey = customKeypad.getKey();
    if (customKey)
    {
      Serial.println((int)customKey);
      if (customKey == 'A')
        break;
      else if ( customKey == 'D')
        value /= 10;
      else if ((int)customKey >= 48 && (int)customKey <= 57) //  etwas ist nicht ja...
      {
        value *= 10;
        value += (int)customKey - 48;
        Serial.println("  LOLO");
        Serial.println((int)customKey);
        Serial.println(value);
      }
      lcd.setCursor(0, 1);
      for (int i = 0; i < 20; ++i)
        lcd.print(" ");
      lcd.setCursor(0, 1);
      lcd.print(value);
      Serial.println(value);
    }
  }
  File deliveryFile;
  deliveryFile= SD.open("DOSTAWY.TXT", FILE_WRITE);
  if (!deliveryFile)
  {
    Serial.println("delivery deliveryFile error");
  }
  else
  {
    tmElements_t tm;

    RTC.read(tm);

    deliveryFile.print(String() + tm.Day + "." + tm.Month + "." + tmYearToCalendar(tm.Year) + "\t");
    deliveryFile.print(String() + tm.Hour + ":" + tm.Minute + "\t");
    deliveryFile.print(String(value) + "\t");
    deliveryFile.print(String(destinationValue + value) + "\n");
    deliveryFile.close();
  }
  lcd.clear();
  lcd.print("Docelowa wartosc");
  lcd.setCursor(0, 1);
  lcd.print("licznika: ");
  lcd.print(String(destinationValue + value));
  delay(5000);
}

void remaininigLiters()
{
 lcd.clear();
 lcd.print("Stan licznika:");
 lcd.setCursor(0,1);
  lcd.print(String(overallValue)); 
 lcd.setCursor(0,2);
  lcd.print("Docelowo:");
   lcd.setCursor(0,3);
  lcd.print(String(destinationValue) + " (" + String(destinationValue - overallValue)+")"); 
   
   
}























