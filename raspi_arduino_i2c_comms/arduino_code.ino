#include <Wire.h>

void setup()
{
   Wire.begin(); // join i2c bus as master

}

char str[100];

void loop()
{
   int x = 0;
   sprintf(str,"1234567890 %d eom",x++);
   Serial.println(str);
   Wire.beginTransmission(9); // transmit to device #9
   Wire.write(str);         
   Wire.endTransmission();    // stop transmitting

   delay(3000);
}