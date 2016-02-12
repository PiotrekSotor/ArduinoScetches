#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <DS1307RTC.h>
#include <Time.h>
#include <String.h>

int ledPin = 13;

String USERSNAMES[5];
int LICZBA_POJAZDOW = 0;
short VEHICLEID [5];
String VEHICLENAMES [5];


int value = 0;
short loggedInUserIndex = -1;
short chosenVehicleIndex = -1;




//--- wait for hardware
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
    strPath = "POJAZDY/" + VEHICLENAMES[chosenVehicleIndex].substring(0,8)+".txt";
  }
  else
  {
    strPath = "POJAZDY/" + VEHICLENAMES[chosenVehicleIndex]+".txt";
  }
  Serial.println("q"+strPath + "q");
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
    strPath = "OSOBY/" + USERSNAMES[loggedInUserIndex].substring(0,8)+".txt";
  }
  else
  {
    strPath = "OSOBY/" + USERSNAMES[loggedInUserIndex]+".txt";
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

void setup() {

  pinMode(53, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!SD.begin(10,11,12,13)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }

  chosenVehicleIndex = 2;
  loggedInUserIndex = 1;
  USERSNAMES[loggedInUserIndex] = "T_USEdsadasR";
  VEHICLENAMES[chosenVehicleIndex] = "T_VEHICLE";
  value = 17;

  saveOverall();
  saveSpecifiedVehicle();
  saveSpecifiedUser();
  Serial.println("done_1");

  chosenVehicleIndex = 2;
  loggedInUserIndex = 0;
  USERSNAMES[loggedInUserIndex] = "T_USER_2";
  VEHICLENAMES[chosenVehicleIndex] = "T_V12312313EHIC";
  value = 99;

  saveOverall();
  saveSpecifiedVehicle();
  saveSpecifiedUser();
  Serial.println("done_2");


}

void loop() {

  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }



  // put your main code here, to run repeatedly:
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
