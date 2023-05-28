#include <Arduino.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define NRF24L01_MOSI_PIN     (6)
#define NRF24L01_MISO_PIN     (5)
#define NRF24L01_SCK_PIN      (4)
#define NRF24L01_CSN_PIN      (10)
#define NRF24L01_CE_PIN       (7)
#define BUFFER_LENGTH         16

// Objects
RF24 radio(NRF24L01_CE_PIN, NRF24L01_CSN_PIN);
struct PlantPacket  {
  char plantName[15];
  uint8_t percentSoilLevel;
};

// Variables
uint8_t baseStationAddress[5] = {'b','a','s','e','\0'};
const char* ssid              = "TP-Link_5385";
const char* password          = "52957475";
const char* serverName        = "http://192.168.0.22/soil_monitor/post_esp_data.php";
const char* plantName[6]      = {"oliver", "lily", "gustav", "thumbelina", "ivy", "champ"};
String apiKeyValue            = "tPmAT5Ab3j7F9";
uint8_t buffer[BUFFER_LENGTH]    = {"\0"};

// Functions
bool InitializeRadio();
bool InitializeWifi();
bool UpdateMoistureDatabase(const char* plantName, int percentMoisture);
void ParsePlantPacket(uint8_t *buffer, int bufferLength, PlantPacket *packet);
void ClearBuffer(uint8_t *buffer, int bufferLength);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void setup() {

  Serial.begin(115200);
  if(!InitializeRadio())  {
    Serial.println("Failed to initialize radio");
  }

  if(!InitializeWifi()) {
    Serial.println("Failed to initialize WiFi");
  }
  Serial.println("Waiting for plant packets....");
}

void loop() {
  
  if(radio.available()) {
    radio.read(&buffer, sizeof(buffer));
    PlantPacket packet;
    ParsePlantPacket(&buffer[0], BUFFER_LENGTH, &packet);
    ClearBuffer(&buffer[0], BUFFER_LENGTH);
    Serial.print(packet.plantName);
    Serial.println(packet.percentSoilLevel); 

   // UpdateMoistureDatabase(packet.plantName, (int)packet.percentSoilLevel);
  }

}

bool InitializeRadio()  {

  // Attempt to connect to radio up to three times
  for(uint8_t i=0; i<3 && !radio.begin(); i++)  {
    if(i==3)  {
      return false;
    } 
    delay(50); // Wait 50ms before attempting radio initialization again
  }

  // Initialize radio settings
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_2MBPS);
  radio.openReadingPipe(1, baseStationAddress);
  radio.setPayloadSize(sizeof(buffer));
  radio.flush_rx();
  radio.startListening();
  if(!radio.isChipConnected())  {
    Serial.println("Radio chip not connected");
    return false;
  }

  return true;
}

bool InitializeWifi() {

  // Set Wifi connection settings and attempt to connect to network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");

  for (int i=0; WiFi.status() != WL_CONNECTED; i<=10) {
    Serial.print('.');
    delay(1000);

    if(i==10) {
      return false;
    }
  }
  Serial.println(WiFi.localIP());
  return true;
}

bool UpdateMoistureDatabase(const char* plantName, int percentMoisture) {
  
  // Check if WiFi is connected and attempt one reconnect if it isn't
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
    delay(5000);
    if(WiFi.status() != WL_CONNECTED) {
      return false;
    }
  }
    
  WiFiClient client;
  HTTPClient http;

  // Create http client to connect to server
  http.begin(client, serverName);
  http.addHeader("Content-Type","application/x-www-form-urlencoded");

  // Set up a string with our post request
  String httpRequestData = "api_key=" + apiKeyValue + "&moisture=" + percentMoisture + "%&plantname=" + plantName + "";
  Serial.print("httpRequestData: ");
  Serial.println(httpRequestData);

  // Send HTTP POST request
  int httpResponseCode = http.POST(httpRequestData);
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    http.end();

    return true;
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    http.end();

    return false;
  }

}

void ParsePlantPacket(uint8_t *buffer, int bufferLength, PlantPacket *packet)  {

  // Move plantname from buffer to packet one char at a time
  for(int i= 0; i < (bufferLength-1); i++)  {
    packet->plantName[i] = (char)buffer[i];
  }

  // Take the last byte of the buffer and move to the packet for soil level
  packet->percentSoilLevel = buffer[bufferLength];

  return;
}

void ClearBuffer(uint8_t *buffer, int bufferLength) {

  for(int i=0; i<bufferLength; i++) {
    buffer[i] = '\0';
  }

  return;
}