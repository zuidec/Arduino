/* Use a photoresistor (or photocell) to turn on an LED in the dark
   More info and circuit schematic: http://www.ardumotive.com/how-to-use-a-photoresistor-en.html
   Dev: Michalis Vasilakis // Date: 8/6/2015 // www.ardumotive.com */
   

//Constants
const int pResistor = A0; // Photoresistor at Arduino analog pin A0
const int ledPin=9;       // Led pin at Arduino pin 9

//Variables
int value;				  // Store value from photoresistor (0-1023)

void setup(){
 pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
 pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)

 Serial.begin(9600); //Initialize serial connection at 9600 baud
}

void loop(){
 while(true){ 
   value = analogRead(pResistor); ///Read analog value from photoresistor
  
  //You can change value "25"
  if (value > 500){
    digitalWrite(ledPin, LOW);  //Turn led off
  }
  else{
    digitalWrite(ledPin, HIGH); //Turn led on
  }
  
 // Serial.print("Current value: ");
 /* if(value >= 100) Serial.print(value);                                         ///Testing code to get input working with python reading 3 bytes at a time
  else if (value < 100 && value >=10) {Serial.print("0"); Serial.print(value);}   ///
  else if (value < 10){Serial.print("00"); Serial.print(value);}                  ///
  */
  
  Serial.print(value); Serial.write('\0'); //Output current light reading from photoresistor to serial monitor, then print carriage return for receiving program to terminate reading
  delay(500); //Small delay
 }
}