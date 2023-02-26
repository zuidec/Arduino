#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <DHT.h>
#include <stdlib.h>

#define DHTTYPE DHT11
#define DHT11PIN 3
RF24 radio(8,9);  // CE, CSN
const byte address[10] = "ADDRESS01";
const int tempPin=2;       // DHT11 at digital pin 2
float temp;
float hum;

DHT dht(DHT11PIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  dht.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
  temp = dht.readTemperature(true);
  hum = dht.readHumidity();
  char txt[10] = "";
  dtostrf(temp, 4, 2, txt);

  Serial.println(txt);
  radio.write(&txt, sizeof(txt));
  delay(500);
  txt[10] = "";
  dtostrf(hum, 4, 2, txt);

  Serial.println(txt);
  radio.write(&txt, sizeof(txt));
  

}
