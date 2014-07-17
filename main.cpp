

#include <Arduino.h>
#include <portManipulations.h>
#include <sekvojHW.h>




int main(void) {

  init();
  setup();

  while(true) {
    loop();
  }
}




extern sekvojHW hardware;

void test(uint8_t v) {}


void setup() {



	hardware.init(&test);


}



void loop() {


	delay(2);


}




/*
void loop() {

	hardware.writeSRAM(0,B10101010);

	if (hardware.readSRAM(0) == B10101010) Serial.println("x");
	else Serial.println("y");

}

*/



