// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $

#include <VirtualWire.h>
int count;

#include <ServoTimer2.h>
ServoTimer2 firstESC; //Create as much as Servoobject you want. You can controll 2 or more Servos at the same time
int value; // set values you need to zero motor reacts between 700 and 2000 ms

void setup()
{
    Serial.begin(9600);  // Debugging only
    firstESC.attach(9);    // attached to pin 9 I just do this with 1 Servo
    delay(1);
    Serial.println("setup"); //Prints "Setup" to the serial monitor
    vw_set_rx_pin(12);       //Sets pin D12 as the RX Pin
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(4000);      // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
      int i;
      digitalWrite(13, true);  // Flash a light to show received good message
      // Message with a good checksum received, dump it.
      Serial.println("");
      Serial.print("Got: ");
      for (i = 0; i < buflen; i++)
      {
        unsigned char c = (buf[i]);
        if (c==0) {
          firstESC.write(700); //stops the motor
          Serial.print("c = 0 ");
          
        } else if ((c<=136)&&(c>=130)){ Serial.print(" doing nothing ");//change nothing
        } else if (c>=137) {
            value=value+5;
            firstESC.write(value);
            Serial.print("val = +10 ");
            
        } else if (c<=129) {
          value=value-5;
          firstESC.write(value);
          Serial.print("val = -10 ");
        }
        
        Serial.print(c);
        Serial.print(" ");
      }
        count++;
        Serial.print(count);
        Serial.print("    ");
        Serial.println(value);
        digitalWrite(13, false);
    }
}
