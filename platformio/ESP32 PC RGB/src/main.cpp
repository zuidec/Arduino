#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define NUM_LEDS  48
#define NUM_FANS  4
#define LED_PIN   11
#define PC_POWER_PIN 4

CRGB fanLEDs[NUM_LEDS] = {0};

//----------------------{R,   G,    B}
CRGB WoKLightOrange =   {240, 100,  30};
CRGB WoKLightBlue =     {124, 160, 198};
CRGB WoKDarkBlue =      {30,  80,  255};

void fillLEDs(CRGB *LEDsToFill, uint8_t size, CRGB color);
void FanLEDOn();
void FanLEDOff();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Serial initialized at 115200"));
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PC_POWER_PIN, INPUT);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(fanLEDs, NUM_LEDS);               // Add our LED strip to the FastLED library
  FastLED.setBrightness(24);
  FastLED.clear();
  FanLEDOn();
}

void loop() {
 /*
  if(digitalRead(PC_POWER_PIN)==HIGH) {

    FanLEDOn();
    Serial.println("HIGH");
  }
  else  {
    FanLEDOff();
    Serial.println("LOW");
  }
  delay(1000);
  */
  
  //FanLEDOff();
  
  //delay(1000);
}

void fillLEDs(CRGB *LEDsToFill, uint8_t size, CRGB color) {
  for(int i=0;i<size;i++)
  {
    LEDsToFill[i] = color;
  }
}

void FanLEDOn() {
  // Back fan
  fillLEDs(&fanLEDs[0], 12, WoKLightOrange);
  fillLEDs(&fanLEDs[7], 3, CRGB::OrangeRed);
  fillLEDs(&fanLEDs[1], 2, WoKLightBlue);
  // Top fan
  fillLEDs(&fanLEDs[12], 12, WoKLightOrange);
  fillLEDs(&fanLEDs[14], 6, CRGB::OrangeRed);
  // Middle fan
  fillLEDs(&fanLEDs[24], 12, WoKLightBlue);
  fillLEDs(&fanLEDs[26], 7, WoKLightOrange);
  // Bottom fan
  fillLEDs(&fanLEDs[36], 12, WoKDarkBlue);
  fillLEDs(&fanLEDs[39], 5, WoKLightBlue);
 
  FastLED.show();
}

void FanLEDOff()  {
  fillLEDs(&fanLEDs[0], NUM_LEDS, CRGB::Black);
  FastLED.show();
}
