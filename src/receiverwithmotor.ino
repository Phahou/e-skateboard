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
int msg_nr;

#include <ServoTimer2.h>
ServoTimer2 mainESC;    // Could be multiple servos to control
#define MOTOR_STOP_TIMING 700
int esc_timing = 700;   // set esc_timings you need to zero motor reacts between 700 and 2000 ms

void setup() {
	Serial.begin(9600);        // Baud rate
    // init esc pins
	mainESC.attach(9);         // hook motor up to pin 9. Ref: https://pinout.xyz
	delay(1);
	Serial.println("setup");

    // wireless setup
	vw_set_rx_pin(12);          // Sets pin D12 as the RX Pin
	vw_set_ptt_inverted(true);  // Required for DR3100
	vw_setup(4000);             // Bits per sec
	vw_rx_start();              // Start the receiver PLL running
}

void loop() {
	uint8_t buf[VW_MAX_MESSAGE_LEN] = { 0 };

    // get a message
	if (vw_get_message(buf, &VW_MAX_MESSAGE_LEN)) { // Non-blocking
		int i=0;
		digitalWrite(13, true);	// Flash a light to show it received a message
        
        // I dont remember what I tried to do here
		for (i = 0; i < VW_MAX_MESSAGE_LEN; i++) {
    		unsigned char c = buf[i];
            char* msg=0;
    		if (c==0) {
	    		mainESC.write(MOTOR_STOP_TIMING); //stops the motor
                msg = "c == 0";
			
    		} else if ((c<=136)&&(c>=130)){
    			msg = "doing nothing";
    			
    		} else if (c>=137) {
    			esc_timing += 5;
    			mainESC.write(esc_timing);
    			msg = "val = +10";
    			
    		} else if (c<=129) {
    			esc_timing -= 5;
    			mainESC.write(esc_timing);
    			msg = "val = -10 ";
    		}
            Serial.print("Status: ");
            Serial.print(msg);
            Serial.print(" | c =")
    		Serial.print(c);
	    	Serial.print(" ");
		}
		msg_nr++;
		Serial.print("Got: ");
		Serial.print(msg_nr);
		Serial.print(" ");
		Serial.println(esc_timing);
		digitalWrite(13, false);
	}
}
