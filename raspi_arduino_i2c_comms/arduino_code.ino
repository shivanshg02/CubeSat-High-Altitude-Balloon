#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup()
{
   Wire.begin(); // join i2c bus as master

   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
}

char str[100];

void loop()
{
   
   DateTime now = rtc.now();
   sprintf(str,"%d/%d/%d %d:%d:%d eom",now.year(),now.month(),now.day(),now.hour(),now.minute(),now.second());
   Serial.println(str);
   Wire.beginTransmission(9); // transmit to device #9
   Wire.write(str);         
   Wire.endTransmission();    // stop transmitting

   delay(3000);
}