/*
 BME 280 basic read from I2C and print to serial
*/

// #include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Adafruit_LTR390.h"
#include <SoftwareSerial.h> // Include the SoftwareSerial library
#define ARDUINO_GPS_RX 9 // Arduino RX pin connected to GPS TX
#define ARDUINO_GPS_TX 8 // Arduino TX pin connected to GPS RX
#define GPS_BAUD_RATE 9600 // The GPS Shield module defaults to 9600 baud
// #define BME_SCK 13
// #define BME_MISO 12
// #define BME_MOSI 11
// #define BME_CS 10
SoftwareSerial gpsPort(ARDUINO_GPS_TX, ARDUINO_GPS_RX);

// We'll also define a more descriptive moniker for the Serial Monitor port.
// This is the hardware serial port on pins 0/1.
#define SerialMonitor Serial
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

Adafruit_LTR390 ltr = Adafruit_LTR390();
bool bme_status;
bool ltr390_status;
int count;


void setup() {
  Wire.begin();
  gpsPort.begin(9600);
  Serial.begin(9600);

  bme_status = bme.begin();  

  delayTime = 1000;
  count = 0;

  ltr390_status = ltr.begin();

  ltr.setMode(LTR390_MODE_UVS);

  ltr.setGain(LTR390_GAIN_3);


  ltr.setResolution(LTR390_RESOLUTION_16BIT);

  ltr.setThresholds(100, 1000);
  ltr.configInterrupt(true, LTR390_MODE_UVS);

  
}


void loop(){
  count += 1;
  Serial.print(count);
  String returns = printValues();
  delay(delayTime);
}


String printValues() {
  String return_str = "";
  String gps_str = "";
  while (gpsPort.available()) {
    gps_str = String(gps_str + (char)gpsPort.read());
  }
  char *gpsc_str = gps_str.c_str();

  char *token;
  token = strtok(gpsc_str, ",");
  
  String current_str = "";
  char* prev;
  while(token != NULL) {
    if (strcmp(token, "N") == 0) {
      current_str = String(current_str + String(prev)+"N");
    }
    if (strcmp(token, "W") == 0) {
      current_str = String(current_str + String(prev)+"W");
    }
    prev = token;
    token = strtok(NULL, ",");
  }

  return_str = String(current_str);
  if (bme_status){
    //Serial.print("T");
    //Serial.print(1.8 * bme.readTemperature() + 32);
    return_str = String(return_str+"T"+(1.8 * bme.readTemperature() + 32));
    //Serial.print("P");
    //Serial.print(bme.readPressure() / 100.0F);
    return_str = String(return_str+"P"+bme.readPressure() / 100.0F);

    return_str = String(return_str+"A"+bme.readAltitude(SEALEVELPRESSURE_HPA));
    return_str = String(return_str+"H"+bme.readHumidity());
  }
  if (ltr390_status) {
      if (ltr.newDataAvailable()) {
        return_str = String(return_str+"U"+ltr.readUVS());
        //Serial.print("U");
        //Serial.print(ltr.readUVS());
      } 
  }
  String final_str = "";
  for (int i = 0; i < return_str.length(); i++) {
    if (return_str.charAt(i) > 32 && return_str.charAt(i) < 127) {
      final_str = final_str + return_str.charAt(i);
    }
  }
  Serial.println(final_str);
  return final_str;

}