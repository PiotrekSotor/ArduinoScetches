/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */


int D1;
int D2;
int D3;
int D4;
int A;
int B;
int C;
int D;
int E;
int F;
int G;
int DP;


void HexTo7Seg(bool[4] bity)
{
    
}

void 4x7SegDispl (byte[2] toDisplay)
{
    bool[16] 2Bajty;
    for (int i=0;i<16;++i)
    {
        2Bajty[15-i]=toDisplay%2;
        toDisplay/=2;
    }
    digitalWrite(D1,HIGH);
    HexTo7Seg(2Bajty[0]);
    digitalWrite(D1,LOW);
    
    digitalWrite(D2,HIGH);
    HexTo7Seg(2Bajty[4]);
    digitalWrite(D2,LOW);
    
    digitalWrite(D3,HIGH);
    HexTo7Seg(2Bajty[8]);
    digitalWrite(D3,LOW);
    
    digitalWrite(D4,HIGH);
    HexTo7Seg(2Bajty[12]);
    digitalWrite(D4,LOW);
    
    
    
}


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(8, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  //digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  //delay(1000);              // wait for a second
}
