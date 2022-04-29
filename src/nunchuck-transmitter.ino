 #include <Wire.h>
#include <wiinunchuck.h>
#include <VirtualWire.h>

/**
 * Nunchuck cables:
 * red -> 3.3V
 * white -> GND
 * yellow -> a5 (some analog pin)
 * green -> a4 (some analog pin)
 * black -> GND
 */

void setup() {
	 Serial.begin(9600);
 
	// Nunchuk initialisieren und Joystick auf Mittelposition kalibrieren
	vw_set_tx_pin(12);					// Sets pin D12 as the TX pin
	vw_set_ptt_inverted(true);	// Required for DR3100
	vw_setup(4000);				 // Bits per sec
	nunchuk_init();
	delay(100);
	nunchuk_calibrate_joy();
	delay(100);
}
 
void loop() {
 
	// Daten (6 Byte) vom Nunchuk Controller auslesen
	nunchuk_get_data();
	
	// Die einzelnen Werte stehen jetzt in Funktionen der WiiNunchuck-Library zur Verfügung
	if(nunchuk_cbutton()) {
		unsigned char msg =nunchuk_cjoy_y();
		vw_send((uint8_t *)&msg, sizeof(uint8_t));
		Serial.print("Send: int:");
		Serial.print(msg);
		
	} else if(!(nunchuk_cbutton())) {
			const unsigned char msg = 0;					//Stop signal
			vw_send((uint8_t *)&msg, sizeof(uint8_t));		//Sending the stop message
			Serial.print("Send(!C): ");
			Serial.println(msg);
	}
	if (nunchuk_zbutton()){
		/* do nothing aka hold the speed */
		Serial.print("if Z\n");
		
		//while(nunchuk_cbutton()){ delay(100);};
	}
	char buffer[25];
	//Serial.print("buff: ");
	//sprintf(buffer, "X:%3d Y:%3d Z:%1d C:%1d", nunchuk_cjoy_x(), nunchuk_cjoy_y(), nunchuk_zbutton(), nunchuk_cbutton());
	//Serial.println(buffer);
	delay(50);
}
