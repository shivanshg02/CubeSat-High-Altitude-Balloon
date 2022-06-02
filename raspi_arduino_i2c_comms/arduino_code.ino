#include <Wire.h>

void setup()
{
   Wire.begin(); // join i2c bus as master

}

char str[100];
int x = 0;

void loop()
{
   sprintf(str,"1234567890 %d eom",x++);
   Serial.println(str);
   Wire.beginTransmission(9); // transmit to device #9
   Wire.write(str);         
   Wire.endTransmission();    // stop transmitting

   delay(3000);
}