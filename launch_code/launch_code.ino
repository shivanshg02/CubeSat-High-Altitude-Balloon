#include <SPI.h>
#include <SD.h>
#include <Arduino_LSM9DS1.h>
#include <Wire.h>
#include "SparkFunBME280.h"
#include <ArduinoNmeaParser.h>

static const uint32_t GPSBaud       = 9600;
static const int      raspiAddress  = 9;
static const int      numSensors    = 3;
static const int      sdCS          = 2;
static const int      i2cInterval   = 3000;
static const String   logName       = "log.txt";
static const int      bufferLen     = 251;
static const String   clearBuffer   = "                                                                                                                                                                                                                                                           ";

bool allInitialized = true;
float a_x, a_y, a_z;  // linear acceleration for imu
float r_x, r_y, r_z;  // angular acceleration for imu
float m_x, m_y, m_z;  // magnetic field for imu
unsigned long startTime;
unsigned long currTime;
unsigned long prevSendTime;
String gpsData;
char i2cBuffer[bufferLen];   // max buffer size is 32 bytes

BME280 bme;
File dataLog;
String currentData[numSensors];

bool initGPS(){
  Serial1.begin(GPSBaud);
  Serial.println("Started communication with GPS module...");
  return true;
}

void onRmcUpdate(nmea::RmcData const rmc)
{
  if      (rmc.source == nmea::RmcSource::GPS)     gpsData += "GPS: ";
  else if (rmc.source == nmea::RmcSource::GLONASS) gpsData += "GLONASS: ";
  else if (rmc.source == nmea::RmcSource::Galileo) gpsData += "Galileo: ";
  else if (rmc.source == nmea::RmcSource::GNSS)    gpsData += "GNSS: ";

  if (rmc.is_valid)
  {
    gpsData += (String) rmc.longitude + "," + (String) rmc.latitude + "," + (String) rmc.speed + "," + (String) rmc.course;
  }
  else {
    gpsData = "No GPS data";
  }
}

ArduinoNmeaParser parser(onRmcUpdate, nullptr);

String handleGPS(){
  while(Serial1.available()){
    parser.encode((char)Serial1.read());
  }
  Serial.println(gpsData);
  return gpsData;
}

/*
String handleGPS(){
  String toReturn = "";
  if(Serial1.available()){
    String gpsStr = Serial1.readStringUntil('\n');
    if(gpsStr.indexOf("$GPGGA") != -1){
      int comma = gpsStr.indexOf(',');
      gpsStr = gpsStr.substring(comma + 1);

      // add time from gps message
      toReturn += "Time: ";
      comma = gpsStr.indexOf(',');
      String gpsTime = gpsStr.substring(0, comma);
      if(gpsTime.length() > 6){
        toReturn += gpsTime.substring(0,2) + ":" + gpsTime.substring(2,4) + ":" + gpsTime.substring(4);
      }
      gpsStr = gpsStr.substring(comma + 1);

      // add lat
      toReturn += " Lat: ";
      comma = gpsStr.indexOf(',');
      String latitude = gpsStr.substring(0, comma);
      if(latitude.length() > 0){
        toReturn += latitude.substring(0,2) + "." + (String) ((latitude.substring(2).toFloat()) / 60);
      }
      gpsStr = gpsStr.substring(comma + 1);
      comma = gpsStr.indexOf(',');
      latitude = gpsStr.substring(0, comma);
      if(latitude.length() > 0){
        toReturn += latitude;
      }
      gpsStr = gpsStr.substring(comma + 1);

      // add long
      toReturn += " Long: ";
      comma = gpsStr.indexOf(',');
      String longitude = gpsStr.substring(0, comma);
      if(longitude.length() > 0){
        toReturn += longitude.substring(0,3) + "." + (String) ((longitude.substring(3).toFloat()) / 60);
      }
      gpsStr = gpsStr.substring(comma + 1);
      comma = gpsStr.indexOf(',');
      longitude = gpsStr.substring(0, comma);
      if(longitude.length() > 0){
        toReturn += longitude;
      }
      gpsStr = gpsStr.substring(comma + 1);

      // add altitude
      for(int i = 0; i < 3; i++){
        comma = gpsStr.indexOf(',');
        gpsStr = gpsStr.substring(comma + 1);
      }
      comma = gpsStr.indexOf(',');
      toReturn += " Alt: ";
      String gpsAlt = gpsStr.substring(0, comma);
      toReturn += gpsAlt;
      gpsStr = gpsStr.substring(comma + 1);
      comma = gpsStr.indexOf(',');
      toReturn += gpsStr.substring(0, comma);
      gpsStr = gpsStr.substring(comma + 1);
    }
  }
  return toReturn;
}*/

bool initIMU(){
  if(!IMU.begin()){
    Serial.println("Failed to initialize IMU");
    return false;
  }
  else{
    Serial.println("IMU initialized...");
    return true;
  }
}

String handleIMU(){
  String toReturn = "";
  if(IMU.accelerationAvailable()){
    IMU.readAcceleration(a_x, a_y, a_z);
  }
  if(IMU.gyroscopeAvailable()){
    IMU.readGyroscope(r_x, r_y, r_z);
  }
  if(IMU.magneticFieldAvailable()){
    IMU.readMagneticField(m_x, m_y, m_z);
  }
  toReturn += " Ax: " + (String) a_x + " Ay: " + (String) a_y + " Az: " + (String) a_z;
  toReturn += " Rx: " + (String) r_x + " Ry: " + (String) r_y + " Rz: " + (String) r_z;
  toReturn += " Mx: " + (String) m_x + " My: " + (String) m_y + " Mz: " + (String) m_z;
  return toReturn;
}

bool initBME(){
  if(!bme.beginI2C()){
    Serial.println("Failed to initialize BME280");
    return false;
  }
  else{
    Serial.println("BME280 initialized...");
    return true;
  }
}

String handleBME(){
  String toReturn = "";
  toReturn += " Hum: " + (String) bme.readFloatHumidity() + 
              " Pres: " + (String) bme.readFloatPressure() +
              " Alt: " + (String) bme.readFloatAltitudeFeet() + 
              " Temp: " + (String) bme.readTempF();

  return toReturn;
       
}

bool initSD(){
  if(!SD.begin(sdCS)){
    Serial.println("SD card failed to initialize...");
    return false;
  }
  else{
    Serial.println("SD card initialized...");
    return true;
  }
}

bool (*initFuncs[numSensors + 1])() = {initGPS,
                                       initIMU,
                                       initBME,
                                       initSD};

String (*sensorHandlers[numSensors])() = {handleGPS,
                                          handleIMU,
                                          handleBME};


void setup() {
  // Establish serial connection before anything
  Serial.begin(115200);
  //while(!Serial);
  Serial.println("System started...");
  
  // start I2C bus
  Wire.begin();

  // initialize sensors and SD card
  for(int i = 0; i < numSensors + 1; i++){
    allInitialized = allInitialized && initFuncs[i]();
  }

  // dont proceed unless all sensors are initialized
  if(allInitialized){
    Serial.println("ALL SENSORS INITIALIZED...");
  }
  else{
    while(true);
  }
  startTime = millis();
}

void loop() {
  // update timestamp
  currTime = millis() - startTime;
  // update sensor vals
  for(int i = 0; i < numSensors; i++){
    currentData[i] = sensorHandlers[i]();
  }
  // log data
  dataLog = SD.open(logName, FILE_WRITE);
  dataLog.println((String) currTime);
  for(int i = 0; i < numSensors; i++){
    dataLog.println(currentData[i]);
  }
  dataLog.println('\n');
  dataLog.close();

  // periodically send data to raspi to log
  if(currTime - prevSendTime > i2cInterval){
    prevSendTime = currTime;
    Wire.beginTransmission(raspiAddress);
    for(int i = 0; i < numSensors; i++){
      clearBuffer.toCharArray(i2cBuffer, bufferLen);
      currentData[i].toCharArray(i2cBuffer, bufferLen);
      Wire.write(i2cBuffer);
    }
    Wire.endTransmission();
  }
  delay(1000);
}
