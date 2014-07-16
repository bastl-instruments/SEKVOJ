
#include <sekvojHW-settings.h>

#ifndef SEKVOJHW_H_
#define SEKVOJHW_H_

#include <hw/IHWLayer.h>
#include <fifoBuffer.h>

class sekvojHW : public IHWLayer {



public:

	// sets up all the pins, timers and SPI interface
	// call this before using any other method from this class
	void init(void(*buttonChangeCallback)(uint8_t number));

	/***KNOBS***/

	//Disabled function in SEKVOJ
	virtual uint8_t getKnobValue(uint8_t index){return 0;};

	/***LEDS***/

	// set the state of a led
	virtual void setLED(uint8_t number, IHWLayer::LedState state);

	// print the state arrays to the Serial terminal
	void printLEDStates();


	/***BUTTONS***/

	// the the state of a button identified by its id
	virtual IHWLayer::ButtonState getButtonState(uint8_t number);

	// print the read button states to serial terminal
	void printButtonStates();


	/***RAM***/

	// write a byte to the given address
	virtual void writeSRAM(long address, uint8_t data);

	// write a number of bytes starting at the given address
	virtual void writeSRAM(long address, uint8_t* buf, uint16_t len);

	// read the byte stored at the given address
	virtual uint8_t readSRAM(long address);

	// read a number of bytes starting from the given address
	virtual void readSRAM(long address, uint8_t* buf, uint16_t len);



	/***DISPLAY***/

	void clearDisplay();
	void setDisplayCursor(uint8_t col, uint8_t row);



	/**TIMING**/

	uint16_t getElapsedBastlCycles();
	uint8_t  getBastlCyclesPerSecond();




	// only called by ISR routine.
	// they would be declared private but this would prevent the ISR from accessing them
	// there are workarounds for this but as they come at a cost I just left it like this
	void isr_updateNextLEDRow();
	void isr_updateButtons();
	uint16_t bastlCycles;


private:

	void initDisplay();
	void latchDisplayData();

	//void bufferDisplayCommand(uint8_t command);


	void sendDisplayCommand(uint8_t command);
	void sendDisplayData(uint8_t data);
	void sendByteToDisplay(uint8_t byte);


	uint16_t ledStatesBeg[4];
	uint16_t ledStatesEnd[4];
	uint16_t buttonStates[4];


	void (*buttonChangeCallback)(uint8_t number);

	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;

	fifoBuffer<uint16_t,BUFFER_DEPTH> displayBuffer;



};

static inline __attribute__((always_inline)) byte spiRead() {
  SPDR = 0xFF; // start SPI clock
  while (!(SPSR & _BV(SPIF)));
  return SPDR;
}

static inline __attribute__((always_inline)) byte spiWrite(byte data) {

  SPDR = data;
  while (!(SPSR & _BV(SPIF)));

  return SPDR;
}





#endif
