// Import library to use with DHT sensor
#include <DHT.h>
#include <stdlib.h>
// Import library to communicate with OLED screen
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define physical pine connections
#define DHTTYPE DHT11
#define DHT11PIN 3
#define SDA A4
#define SDC A5

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
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };



const int tempPin=2;       // DHT11 at digital pin 2
float temp;
float hum;

DHT dht(DHT11PIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Start the dht sensor
  dht.begin();

  // Attempt OLED screen connection and pause if failed
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer and push to screen
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  
}

void loop() {
  delay(2000);
  // Clear display
  display.clearDisplay();

  //Read temp and humidity from DHT
  temp = dht.readTemperature(true);
  hum = dht.readHumidity();
  // Use dtostrf to convert float into char[]
  char txt[10] = "";
  dtostrf(temp, 4, 2, txt);
  // Print temperature to serial port
  Serial.print(F("Temperature (F): "));
  Serial.println(txt);
  //Send temperature to OLED buffer
  display.setCursor(0,0);
  display.println(F("Temp (F):"));
  display.setTextSize(2);
  display.println(txt);
  //display.setTextSize(1);

  delay(500);
  // Use dtostrf to convert float into char[]
  txt[10] = "";
  dtostrf(hum, 4, 2, txt);
  // Print humidity to serial port
  Serial.print(F("Humidity (%): "));
  Serial.println(txt);
  // Send humidity to OLED buffer
  display.println(F("Hum (%):"));
 // display.setTextSize(2);
  display.println(txt);
  //display.setTextSize(1);
  // Tell display to display contents
  display.display();
  

}
