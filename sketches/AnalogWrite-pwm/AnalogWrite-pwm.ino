
const int pin = 3;



void setup() {
   Serial.begin(9600);
    pinMode(pin, OUTPUT);
  
}

void loop() {
  // iterate over the pins:
  
    for (int brightness = 0; brightness < 255; brightness++) {
      analogWrite(pin, brightness);
      delay(40);
    }
    // fade the LED on thisPin from brithstest to off:
    for (int brightness = 255; brightness >= 0; brightness--) {
      analogWrite(pin, brightness);
      delay(40);
    }
 
    delay(100);
  
}
