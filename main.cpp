

#include <Arduino.h>
#include <portManipulations.h>
#include <sekvojHW.h>
#include <SdFat.h>




int main(void) {

  init();
  setup();

  while(true) {
    loop();
  }
}



extern sekvojHW hardware;

uint8_t lastNumber;
uint8_t lastNumberRead;

void printSerial(uint8_t number) {
	lastNumber = number;

}


void setup() {

	bit_dir_outp(PIN);

	Serial.begin(115200);

	delayMicroseconds(10000);


	hardware.init(&printSerial);

	hardware.setLED(16,sekvojHW::ON);
	hardware.setLED(17,sekvojHW::BLINK);
	hardware.setLED(18,sekvojHW::BLINK_INVERT);




	//hardware.printLEDStates();



}


void loop() {

	if (hardware.getButtonState(16) == sekvojHW::DOWN ) {
		hardware.setLED(16,sekvojHW::ON);
	} else {
		hardware.setLED(16,sekvojHW::OFF);
	}

	if (lastNumberRead != lastNumber) {
		lastNumberRead = lastNumber;
		Serial.println(lastNumber,DEC);
	}


}





