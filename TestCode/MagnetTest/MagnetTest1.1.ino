#include <SoftwareSerial.h>
#include magPin 9

// SERIAL PORT VARS
char command;
int magnetPin = 9;

void setup() {
  Serial.begin(9600);
  pinMode(magPin, OUTPUT);
  digitalWrite(magPin, LOW);
}

void loop() {
  while (Serial.available())
    {
    command = Serial.read();
  
    if (command == '0') //command for off
    {
      digitalWrite(magnetPin, LOW);
    
      Serial.println("OFF");
    }
  
    else if (command == '1') //command for on
    {
      digitalWrite(magnetPin, HIGH);
      
      Serial.println("ON");
    }
    
    delay(50); //more delay
    
    //There needs to be a delay because the computer can't
      print a statement instantaneously after another statement
    }
}
