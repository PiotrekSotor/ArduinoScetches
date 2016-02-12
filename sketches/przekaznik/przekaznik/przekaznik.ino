boolean state = false;

void setup()
{
  pinMode(30,OUTPUT);
}

void loop()
{
  if (state)
  {
    digitalWrite(30,HIGH);
  }
  else
  {
    digitalWrite(30,LOW);    
  }
  state = !state;
  
  delay(1000);
}
