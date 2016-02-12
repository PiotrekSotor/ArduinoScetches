#include <Wire.h>

void setup()
{
  Wire.begin(0x40);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(100);
}

void receiveEvent(int howMany)
{  
  int x = Wire.read();    // receive byte as an integer
  Serial.print("LOL1 ");
  Serial.println(x);         // print the integer
  x = Wire.read();    // receive byte as an integer
  Serial.print("LOL2 ");
  Serial.println(x);         // print the integer
  x = Wire.read();    // receive byte as an integer
  Serial.print("LOL3 ");
  Serial.println(x);         // print the integer
  x = Wire.read();    // receive byte as an integer
  Serial.print("LOL4 ");
  Serial.println(x);         // print the integer
}
void requestEvent()
{
  Serial.println("wysylanie");
  Wire.write("hello "); 
}
