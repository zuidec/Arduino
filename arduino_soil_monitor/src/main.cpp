/*
 *  Arduino pro mini 3.3 app to read a capacitive moisture sensor
 *  then transmit the reading using NRF24L01 to a base station
 * 
 *  Author: Case Zuiderveld
 *  Last updated 4/26/2023
 */

#include <Arduino.h>
// SPI and RF24 are needed to interface with NRF24L01 radio module 
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
// SoilMonitor.h has the functions to read soil sensor and use autowater
#include "SoilMonitor.h"

// Define all the pins so we can call them by name
#define SOIL_SENSOR_PWR_PIN   (5)
#define SOIL_SENSOR_DATA_PIN  (A0)
#define NRF24L01_MOSI_PIN     (11)
#define NRF24L01_MISO_PIN     (12)
#define NRF24L01_SCK_PIN      (13)
#define NRF24L01_CSN_PIN      (10)
#define NRF24L01_CE_PIN       (9)
#define NRF24L01_PWR_PIN      (6)
#define PUMP_PWR_PIN          (3)
#define FLOAT_SENSOR_PIN      (4)
#define TIME_TO_SLEEP
#define BUFFER_LENGTH         16

RF24 radio(NRF24L01_CE_PIN, NRF24L01_CSN_PIN);
SoilMonitor soilMonitor(SOIL_SENSOR_PWR_PIN,SOIL_SENSOR_DATA_PIN, PUMP_PWR_PIN, FLOAT_SENSOR_PIN);
struct PlantPacket  {
  char plantName[15];
  uint8_t percentSoilLevel;
};
PlantPacket packet;


bool isTXMode = true;
bool isRadioPowered = false;
uint8_t radioAddress[6] = {"ollie"};
uint8_t baseStationAddress[5] = {'b','a','s','e','\0'};
char plantName[15] = {'o','l','i','v','e','r','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
char buffer[BUFFER_LENGTH]    = {"\0"};

void ToggleRadioOn();
void ToggleRadioOff();
bool InitializeRadio();
void ClearBuffer(char *buffer, int bufferLength);
void SetPlantPacketName(char *plantName);
void CreatePlantPacket(char* buffer, PlantPacket *packet);
void EnterSleepMode(long long timeToSleepMicroseconds);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(NRF24L01_PWR_PIN, OUTPUT);
    
    SetPlantPacketName(&plantName[0]);
}

void loop() {
    // Read soil level
    soilMonitor.ReadSoilLevel();
    packet.percentSoilLevel = soilMonitor.percentSoilLevel;
    ClearBuffer(&buffer[0], BUFFER_LENGTH);
    CreatePlantPacket(&buffer[0], &packet);
    Serial.print("Buffer contents: ");
    for(int i=0;i<16;i++)  {
      Serial.print(buffer[i]);
    }
    Serial.println();
    // Turn on radio and attempt to transmit the soil level
    if(!InitializeRadio())  {
      Serial.println(F("Failed to initialize radio"));
    }
    else if(!radio.write(&buffer[0], sizeof(buffer)))  {
      Serial.println(F("Transmission failed"));
    }
    else  {
      Serial.println(F("Transmission successful"));
    }
    ToggleRadioOff();
    
    Serial.println(soilMonitor.percentSoilLevel);
    delay(2000);
}

void ToggleRadioOn() {
    if(!isRadioPowered)  {
      digitalWrite(NRF24L01_PWR_PIN, HIGH);
      isRadioPowered = true;
    }
}

void ToggleRadioOff() {
    if(isRadioPowered)  {
      digitalWrite(NRF24L01_PWR_PIN, LOW);
      isRadioPowered = false;
    }
}

bool InitializeRadio()  {
    if(!isRadioPowered) {
       ToggleRadioOn();
       delay(50);   // Wait 50ms for radio board to become powered
    }
    // Attempt to connect to radio up to three times
    for(uint8_t i=0; i<3 && !radio.begin(); i++)  {
      if(i==3)  {
        return false;
      } 
      delay(50); // Wait 50ms before attempting radio initialization again
    }

    // Initialize radio settings
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.setPayloadSize(sizeof(buffer));
    radio.openWritingPipe(baseStationAddress);
    //radio.openReadingPipe(1, radioAddress);

    return true;
}

void SetPlantPacketName(char *plantName)  {
  for(uint8_t i=0;i<15;i++) {
    packet.plantName[i] = plantName[i];
  }
}

void CreatePlantPacket(char* buffer, PlantPacket *packet) {
  for(uint8_t i=0; i<15;i++)  {
    buffer[i] = packet->plantName[i];
  }
  buffer[15] = (char)packet->percentSoilLevel;
}

void ClearBuffer(char *buffer, int bufferLength) {

  for(int i=0; i<bufferLength; i++) {
    buffer[i] = '\0';
  }

  return;
}