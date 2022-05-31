#include <Wire.h>

void setup()
{
   Wire.begin(); // join i2c bus as master
}

char str[17];

int x = 0;

void loop()
{
   sprintf(str, "Message %7d\n", x);
   if (++x > 9999999) x=0;

   Wire.beginTransmission(9); // transmit to device #9
   Wire.write(str);           // sends 16 bytes
   Wire.endTransmission();    // stop transmitting

   delay(50);
}