#include <SD.h>
#include <SPI.h>

String readWord;
String USERSNAMES[5];

int VEHICLEID [5];
String VEHICLENAMES [5];
int LICZBA_UZYTKOWNIKOW = 0;
int LICZBA_POJAZDOW = 0;
String USERPINS[20];


void readWordFromFile (File file)
{
  readWord = String();
  char c;
  do {
    c = file.read();
    //Serial.println("1" + String(c) + "1");
    readWord += String(c);

  }
  while ( c != ' ' && (int)c != -1 && c != '\n');
  //readWord[index++] = '\0';
  readWord = readWord.substring(0,readWord.length() - 1);
  //Serial.println("qqqqq" + readWord + "qqqqq");
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SD.begin(10, 11, 12, 13);
  // put your main code here, to run repeatedly:
  Serial.println("readUsers():");
  readUsers();
  Serial.println("readVehicles():");
  readVehicles();
  
  for (int i=0; i< LICZBA_POJAZDOW; ++i)
  {
     Serial.println(String(VEHICLEID[i]) + "  " + VEHICLENAMES[i]); 
  }
  for (int i=0;i<LICZBA_UZYTKOWNIKOW; ++i)
  {
     Serial.println(USERSNAMES[i] + "  " + String(USERPINS[i])); 
  }

}

void loop() {
  delay(9999);
}
