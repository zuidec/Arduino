#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

RF24 radio(6,10);  // CE, CSN
const byte address[10] = "ADDRESS01";
char txt[32] = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(radio.available())
  {
    for(int i=0; i<32;i++)txt[i] = "";
    radio.read(&txt, sizeof(txt));
    Serial.println(txt);

    displayTemp();
    for(int i=0; i<32;i++)txt[i] = "";
    delay(500);
    radio.read(&txt, sizeof(txt));
    displayHumidity();
  }
}

void displayTemp()
{

display.setCursor(0,0);
display.clearDisplay();
display.println(F("Temp degrees F"));
display.println(txt);
display.display();
}

void displayHumidity()
{

  display.println(F("Humidity"));
  display.println(txt);
  display.display();
}
