

#include <Arduino.h>
#include <portManipulations.h>
#include <sekvojHW.h>
#include <players/Player.h>
#include <modules/Stepper.h>
#include <players/ArduinoMIDICommandProcessor.h>
#include <players/PlayerSettings.h>
#include <data/FlashStepMemory.h>
#include <MIDI.h>
#include "MainMenuView.h"

MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);

Player * player;
Stepper * stepper;
ArduinoMIDICommandProcessor * processor;
FlashStepMemory * memory;
PlayerSettings * settings;


MainMenuView mainMenu;


int main(void) {

  init();
  setup();

  while(true) {
    loop();
  }
}

extern sekvojHW hardware;

void stepperStep() {
	player->stepFourth();
}

void noteOn(unsigned char note, unsigned char velocity, unsigned char channel) {
	 MIDI.sendNoteOn(35 + note, 127 ,channel);
}

void noteOff(unsigned char note, unsigned char velocity, unsigned char channel) {
	MIDI.sendNoteOff(35 + note, velocity ,channel);
}

void test(uint8_t v) {}


void initFlashMemory(FlashStepMemory * memory) {
	DrumStep::DrumVelocityType inactiveSteps[4] = {DrumStep::OFF, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
	DrumStep::DrumVelocityType activeSteps[4] = {DrumStep::NORMAL, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
	DrumStep::DrumVelocityType emptyStep[4] = {DrumStep::OFF, DrumStep::OFF, DrumStep::OFF, DrumStep::OFF};
	DrumStep activeDrumStep(true, true, activeSteps);
	DrumStep emptyDrumStep(true, true , activeSteps);
	DrumStep muteDrumStep(true, true, inactiveSteps);

	// Initialize memory to empty
	for (unsigned char instrument = 0; instrument < 20; instrument++) {
		for (unsigned char step = 0; step < 64; step++) {
			memory->setDrumStep(instrument, 0, step, muteDrumStep);
		}
	}

	//Set some steps so something is playing
	DrumStep inactiveDrumStep(false, false, inactiveSteps);
	for (int i = 16; i < 64; i++) {
		memory->setDrumStep(0, 0, i, inactiveDrumStep);
	}
	for (int i = 0; i < 16; i++) {
		memory->setDrumStep(0, 0, i, emptyDrumStep);
	}
}

void setup() {

	hardware.init(&test);

	stepper = new Stepper(&hardware);
	stepper->setBPM(100);
	stepper->setStepCallback(&stepperStep);

	settings = new PlayerSettings();
	settings->setCurrentPattern(0);

	for (unsigned char i = 0; i < 10; i++) {
			settings->setInstrumentOn(InstrumentTypes::DRUM, i, i == 0);
			settings->setInstrumentChannel(InstrumentTypes::DRUM, i, 2);
			settings->setDrumInstrumentNote(i, i);
	}
	memory = new FlashStepMemory(&hardware);

	initFlashMemory(memory);

	processor = new ArduinoMIDICommandProcessor(&noteOn, &noteOff);
	player = new Player(memory, processor, settings);
	Serial.end();
	MIDI.begin(0);

	hardware.clearDisplay();

	mainMenu.init(&hardware, player, memory, settings);
}



void loop() {
		stepper->update();
		mainMenu.update();
}




