#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <DS1307RTC.h>
#include <Time.h>

int LICZBA_UZYTKOWNIKOW = 0;
int USERSPINS[20];

String USERSNAMES[20];
int LICZBA_POJAZDOW = 0;
short VEHICLEID [20];
String VEHICLENAMES [20];


int value = 0;
short loggedInUserIndex = -1;
short chosenVehicleIndex = -1;


//--- wait for hardware
void saveOverall() {
  File logFile;

  logFile = SD.open("log.txt", FILE_WRITE);
  tmElements_t tm;
  RTC.read(tm);

  logFile.print(String() + tm.Day + "." + tm.Month + "." + tmYearToCalendar(tm.Year) + "\t");
  logFile.print(String() + tm.Hour + ":" + tm.Minute + "\t");
  logFile.print(USERSNAMES[loggedInUserIndex] + "\t");
  logFile.print(VEHICLENAMES[chosenVehicleIndex] + "\t");
  logFile.println(value);
  logFile.close();

}
void saveSpecifiedVehicle() {
  File vehicleFile;
  if (!SD.exists("vehicles"))
    SD.mkdir("vehicles");

  tmElements_t tm;
  RTC.read(tm);
  String strPath = "vehicles/" + VEHICLENAMES[chosenVehicleIndex] + ".txt";
  vehicleFile = SD.open( strPath.c_str(), FILE_WRITE);
  vehicleFile.print(String() + tm.Day + "." + tm.Month + "." + tmYearToCalendar(tm.Year) + "\t");
  vehicleFile.print(String() + tm.Hour + ":" + tm.Minute + "\t");
  vehicleFile.print(USERSNAMES[loggedInUserIndex] + "\t");
  vehicleFile.print(VEHICLENAMES[chosenVehicleIndex] + "\t");
  vehicleFile.println(value);
  vehicleFile.close();
}
void saveSpecifiedUser() {
  File userFile;
  if (!SD.exists("users"))
    SD.mkdir("users");

  tmElements_t tm;
  RTC.read(tm);

  String strPath = "users/" + USERSNAMES[loggedInUserIndex] + ".txt";
  userFile = SD.open(strPath.c_str() , FILE_WRITE);
  userFile.print(String() + tm.Day + "." + tm.Month + "." + tmYearToCalendar(tm.Year) + "\t");
  userFile.print(String() + tm.Hour + ":" + tm.Minute + "\t");
  userFile.print(USERSNAMES[loggedInUserIndex] + "\t");
  userFile.print(VEHICLENAMES[chosenVehicleIndex] + "\t");
  userFile.println(value);
  userFile.close();
}


char readWord[20];


void readWordFromFile (File file)
{
  char c;
  int index = 0;
  do {
    c = file.read();
    readWord[index++] = c;

  }
  while ( c != ' ' && (int)c != -1 );
  readWord[index++] = '\0';

}


void readUsers()
{
  File userFile;
  LICZBA_UZYTKOWNIKOW = 0;
  userFile = SD.open ("users/list.txt", FILE_READ);
  while (userFile.available())
  {
    readWordFromFile(userFile);

    USERSNAMES[LICZBA_UZYTKOWNIKOW] = readWord;
    Serial.println(USERSNAMES[LICZBA_UZYTKOWNIKOW]);
    readWordFromFile(userFile);

    USERSPINS[LICZBA_UZYTKOWNIKOW++] = atoi(readWord);
    Serial.println(USERSPINS[LICZBA_UZYTKOWNIKOW - 1]);
  }
  userFile.close();

}
void readVehicles()
{

  File vehicleFile;
  LICZBA_POJAZDOW = 0;
  vehicleFile = SD.open ("vehicles/list.txt", FILE_READ);
  while (vehicleFile.available())
  {
    readWordFromFile(vehicleFile);
    VEHICLEID[LICZBA_POJAZDOW] = atoi(readWord);
    Serial.println(VEHICLEID[LICZBA_POJAZDOW]);
    readWordFromFile(vehicleFile);

    VEHICLENAMES[LICZBA_POJAZDOW++] = readWord;
    Serial.println(VEHICLENAMES[LICZBA_POJAZDOW - 1]);
  }
  vehicleFile.close();

}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SD.begin();
  
  
  Serial.println("ReadUsers():");
  readUsers();
  Serial.println("ReadVehicles():");
  readVehicles();
  
  chosenVehicleIndex = 2;
  loggedInUserIndex = 1;  
  value = 17;  
  saveOverall();
  saveSpecifiedVehicle();
  saveSpecifiedUser();
  Serial.println("done_1");
  
  delay(1000);
  
  chosenVehicleIndex = 1;
  loggedInUserIndex = 0;  
  value = 33;  
  saveOverall();
  saveSpecifiedVehicle();
  saveSpecifiedUser();
  Serial.println("done_2");
}

void loop() {
  // put your main code here, to run repeatedly:

}
