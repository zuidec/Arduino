const int soil_sensor = A3;
const int power_pin = 5;
const int air_moisture = 600;
const int water_moisture = 300;
int value;
int percent;


// Approx 460 completely dry, 178 when completely submerged
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(power_pin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:


  readSoilLevel();
  Serial.print("Value: "); 
  Serial.println(value);
  Serial.print(percent);
  Serial.println(F("%"));
  delay(5000);

}
void readSoilLevel()
{
  digitalWrite(power_pin, HIGH);
  delay(250);
  value = analogRead(soil_sensor);
  digitalWrite(power_pin, LOW);

  percent = map(value, air_moisture, water_moisture, 0, 100);
}