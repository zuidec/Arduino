 String in;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 

}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()==0){}
  in = Serial.readStringUntil('\n');
  if(in!="")
  {
    in.trim();
    Serial.print("I got:"); Serial.println(in);
    in ="";
  }

}
