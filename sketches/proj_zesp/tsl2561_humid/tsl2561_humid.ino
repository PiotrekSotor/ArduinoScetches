#include <Wire.h>
#include "TSL2561.h"
#include "HTU21D.h"

// Example for demonstrating the TSL2561 library - public domain!

// connect SCL to analog 5
// connect SDA to analog 4
// connect VDD to 3.3V DC
// connect GROUND to common ground
// ADDR can be connected to ground, or vdd or left floating to change the i2c address

// The address will be different depending on whether you let
// the ADDR pin float (addr 0x39), or tie it to ground or vcc. In those cases
// use TSL2561_ADDR_LOW (0x29) or TSL2561_ADDR_HIGH (0x49) respectively
TSL2561 tsl(TSL2561_ADDR_FLOAT); 
HTU21D myHumidity;

uint32_t lux;
float humid;
int reg;

void setup(void) {
  Serial.begin(9600);
  Serial.println("Start");
  Wire.begin(0x01);
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  myHumidity.begin();
  
  if (tsl.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No sensor?");
    while (1);
  }
    
  
  tsl.setGain(TSL2561_GAIN_16X);      // set 16x gain (for dim situations)
  
  tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);  // shortest integration time (bright light)
  
}

void loop(void) {
  
  uint16_t x = tsl.getLuminosity(TSL2561_VISIBLE);     
  
  Serial.println(x, DEC);

  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print("IR: "); Serial.print(ir);   Serial.print("\t\t");
  Serial.print("Full: "); Serial.print(full);   Serial.print("\t");
  Serial.print("Visible: "); Serial.print(full - ir);   Serial.print("\t");
  
  Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));
  lux = tsl.calculateLux(full, ir);
  delay(100); 
  
  
  
  float humd = myHumidity.readHumidity();
  float temp = myHumidity.readTemperature();

  Serial.print("Time:");
  Serial.print(millis());
  Serial.print(" Temperature:");
  Serial.print(temp, 1);
  Serial.print("C");
  Serial.print(" Humidity:");
  Serial.print(humd, 1);
  Serial.print("%");
  humid = humd;
  Serial.println();
  delay(1000);

}

void receiveEvent(int howMany)
{
  reg = Wire.read();    // receive byte as an integer
}
void requestEvent()
{
  if (reg == 0x40)
  
    Wire.write((short)humid); 
  else if (reg == 0x39)
    Wire.write((short)lux);
}  

