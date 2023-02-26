#define relay_pin 4
#define button_pin 2



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(relay_pin, OUTPUT);
  pinMode(button_pin, INPUT);
  digitalWrite(relay_pin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(button_pin)==HIGH)
  {
    digitalWrite(relay_pin, HIGH);
    Serial.println("ON");
  }
  else
  {
    digitalWrite(relay_pin, LOW);
    Serial.println("OFF");
  }
  delay(1000);

}
