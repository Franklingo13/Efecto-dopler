/* Arduino test code */

int analogPin = A1; // radar output connected to analog pin 0

int val = 0; // variable to store the value read

void setup()
{
Serial.begin(4800); // setup serial
pinMode(13,OUTPUT); //Indicator LED on pin 13
}

void loop()
{
val = analogRead(analogPin); // read the input pin

//if(val > 180) digitalWrite(13,HIGH);   //LED on if signal detected higher than 180
//   else digitalWrite(13,LOW);

Serial.println(val); // debug value

}
