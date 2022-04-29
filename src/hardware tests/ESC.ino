/*
Coded by Marjan Olesch
Sketch from Insctructables.com
Open source - do what you want with this code!
*/
#include <ServoTimer2.h>
ServoTimer2 firstESC;

int value = 0; // set values you need to zero

void setup() {

  firstESC.attach(9);    // attached to pin 9 I just do this with 1 Servo
  delay(1);
  Serial.begin(9600);    // start serial at 9600 baud

}

void loop() {

//First connect your ESC WITHOUT Arming. Then Open Serial and follo Instructions
 
  firstESC.write(value);
 
  if(Serial.available()) 
    value = Serial.parseInt();    // Parse an Integer from Serial

}

