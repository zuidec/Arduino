#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "FanFX.h"

#define NUM_LEDS  48
#define NUM_FANS  4
#define LED_PIN   10

CRGB fanLEDs[NUM_LEDS] = {0};
uint8_t initialHue = 0;
uint8_t deltaHue = 4;
uint8_t hueDensity = 4;
CRGBPalette16 wayOfKingsPalette;

//----------------------{R,   G,    B}
CRGB WoKLightOrange =   {240, 100,  30};
CRGB WoKLightBlue =     {124, 160,  198};
CRGB WoKDarkBlue =      {30,  80,  255};

void fillLEDs(CRGB *LEDsToFill, uint8_t size, CRGB color);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(fanLEDs, NUM_LEDS);               // Add our LED strip to the FastLED library
  FastLED.setBrightness(128);
  FastLED.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  /*
  for(int i=0;i<NUM_LEDS;i++)
  {
    fanLEDs[i] = CRGB::Aquamarine;
    FastLED.show();
    if(i%12==0)  {
      delay(1000);
    }
    else  {
      delay(200);
    }
  }

  for(int i=0;i<NUM_LEDS;i++)
  {
    fanLEDs[i] = CRGB::Red;
    FastLED.show();
    if(i%12==0)  {
      delay(1000);
    }
    else  {
      delay(200);
    }
  }
  */

 fillLEDs(&fanLEDs[0], 12, WoKLightOrange);
 fillLEDs(&fanLEDs[7], 3, CRGB::OrangeRed);
 fillLEDs(&fanLEDs[1], 2, WoKLightBlue);

 fillLEDs(&fanLEDs[12], 12, WoKLightOrange);
 fillLEDs(&fanLEDs[14], 6, CRGB::OrangeRed);

 fillLEDs(&fanLEDs[24], 12, WoKLightBlue);
 fillLEDs(&fanLEDs[26], 7, WoKLightOrange);

 fillLEDs(&fanLEDs[36], 12, WoKDarkBlue);
 fillLEDs(&fanLEDs[39], 5, WoKLightBlue);
 
 /*
 for(int i=0; i<NUM_LEDS;i++) {
  if(i%1==0) {
    fanLEDs[i] = CRGB::Blue; 
  }
  if(i%2==0) {
    fanLEDs[i] = CRGB::Red; 
  }
  if(i%3==0) {
    fanLEDs[i] = CRGB::Green; 
  }
  if(i%4==0) {
    fanLEDs[i] = CRGB::Blue; 
  }
  if(i%5==0) {
    fanLEDs[i] = CRGB::Red; 
  }
  if(i%6==0) {
    fanLEDs[i] = CRGB::Green; 
  }
  if(i%7==0) {
    fanLEDs[i] = CRGB::Blue; 
  }
  if(i%8==0) {
    fanLEDs[i] = CRGB::Red; 
  }
  if(i%9==0) {
    fanLEDs[i] = CRGB::Green; 
  }
  if(i%10==0) {
    fanLEDs[i] = CRGB::Blue; 
  }
  if(i%11==0) {
    fanLEDs[i] = CRGB::Red; 
  }
  if(i%12==0) {
    fanLEDs[i] = CRGB::Green; 
  }
 }
  */
  FastLED.show();
  
}

void fillLEDs(CRGB *LEDsToFill, uint8_t size, CRGB color) {
  for(int i=0;i<size;i++)
  {
    LEDsToFill[i] = color;
  }
}
