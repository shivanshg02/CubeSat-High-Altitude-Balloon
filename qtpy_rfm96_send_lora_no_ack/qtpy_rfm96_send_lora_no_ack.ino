// LoRa 9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX

#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define RFM95_CS 1
#define RFM95_RST 2
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 420.69

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

Adafruit_NeoPixel pixels(1, PIN_NEOPIXEL);

void setup() 
{
  pixels.begin();
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  //while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Arduino LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);  

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);

  rf95.setTxPower(23, false);
  //rf95.setSpreadingFactor(12);
  //rf95.setSignalBandwidth(125);
  //rf95.setCodingRate4(7);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop()
{
  Serial.println("enter message");
  while (!Serial.available());
  String data = Serial.readString();
  data.trim();
  int len = data.length();
  char radiopacket[len];
  int count = 0;
  for (int i = 0; i < len; i++) {
    radiopacket[i] = data.charAt(i);
  }
  
  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)radiopacket, len);

  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();

  Serial.println("Waiting for packet to complete..."); delay(10);
  rf95.waitPacketSent();
  pixels.clear();
  pixels.show();

delay(1000);
}
