/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers
 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98
 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).
 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!
 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16


TinyGPSPlus gps;
SoftwareSerial mySerial(4,3);

unsigned long startTime;
unsigned long newTime;
const unsigned long period = 1000;
const unsigned long timeoutPeriod = 60000;

void setup() {
  

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(500);


  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  Serial.begin(115200);
  mySerial.begin(9600);

  startTime = millis();



  waitForFirstFix();

}

void loop() {
  



while(mySerial.available() > 0 ){ 

 // if(gps.location.isValid())
  {
    if(gps.encode(mySerial.read()))
  { newTime = millis();
    if(newTime-startTime > period){
      printLocation(); 
      displayLocation();
      startTime = newTime;
  }
  //delay(500);
  //Serial.print(F("Soft Serial device overflowed? " ));
  //Serial.println(mySerial.overflow() ? "YES!" : "No" );
  }
  }
}
  if(!gps.location.isValid())
  {
    Serial.println(F("Invalid GPS location."));

    display.clearDisplay();
    display.display();
    display.setTextSize(1);
    display.setCursor(0,1);
    display.println(F("Location"));
    display.println(F("Invalid"));
    display.display();
    
  }

}

void printLocation()
{
  Serial.println(F("Location:"));
  Serial.print(F("Lat: "));
  Serial.println(gps.location.lat(),6);
  Serial.print(F("Long: "));
  Serial.println(gps.location.lng(),6);
  Serial.print(F("Alt: "));
  Serial.println(gps.altitude.feet(),2);   

}

void displayLocation()
{
   
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.display();
  display.println(F("Location:"));
  display.setTextSize(1);
  display.print(F("Lat:  "));
  display.println(gps.location.lat(),6);
  display.print(F("Long: "));
  display.println(gps.location.lng(),6);
  display.print(F("Alt:  "));
  display.println(gps.altitude.feet());
  display.display();  

}

void waitForFirstFix()
{
  Serial.print(F("Acquiring location"));

  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(F("Acquiring"));
  display.print(F("Location"));
  display.display();

 while(!gps.location.isValid())
  {
    
    if(mySerial.available() > 0) {
      gps.encode(mySerial.read());  //gps.location.lng(); gps.location.lat(); gps.altitude.feet();
      
      }
    
    newTime = millis();
    if(newTime-startTime > (period*5) && millis() < timeoutPeriod)
    {
      display.print(F("."));
      display.display();
      Serial.print(F("."));
      startTime = newTime;
    }
    //delay(1000);
   
}
 Serial.println();
}
