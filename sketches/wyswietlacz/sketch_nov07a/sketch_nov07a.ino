
int pin0;
int pin1;
int licznik;
void setup ()
{
  pin0=11;
  pin1=12;
  licznik=0;
  pinMode(7,OUTPUT);
  pinMode(pin0,OUTPUT); 
  pinMode(pin1,OUTPUT); 
}

void loop()
{
  digitalWrite(7,HIGH);
   ++licznik;
   if (licznik==4)
     licznik=0;
   int temp = licznik;
   bool bit0;
   bool bit1;
   
   bit0 = licznik%2;
   bit1 = licznik/2;
   
   digitalWrite(pin0,bit0);
   digitalWrite(pin1,bit1);
   
  delay(500);
}
