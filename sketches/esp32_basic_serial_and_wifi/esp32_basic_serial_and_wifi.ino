// this sample code provided by www.programmingboss.com
//#include <SoftwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define RXp2 19
#define TXp2 18
const char* ssid = "NETGEAR29";
const char* password = "festiveunicorn516";
const char* serverName = "http://192.168.0.22/post_esp_data.php";

String apiKeyValue = "tPmAT5Ab3j7F9";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXp2, TXp2);
  initWiFi();
}
void loop() {

   // Serial.println(Serial1.readStringUntil('\n'));
   if(WiFi.status()==WL_CONNECTED)
   {
     WiFiClient client;
     HTTPClient http;

     http.begin(client, serverName);

     http.addHeader("Content-Type","application/x-www-form-urlencoded");
    while(Serial1.available()>0) Serial1.readStringUntil('\n');

    delay(5000);

     String httpRequestData = "api_key=" + apiKeyValue + "&moisture_level=" + Serial1.readStringUntil('\n') + "";

         Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&location=Office&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
        
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
   }
   else
   {
     Serial.println(F("Wifi disconnected..."));
   }
   delay(30000);
}
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}