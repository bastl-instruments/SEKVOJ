/*
 * SetActiveView.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: bastl
 */

#include <SetActiveView.h>


SetActiveView::SetActiveView() : hw_(0),
								 memory_(0),
								 currentPattern_(0),
								 currentPanIndex_(0),
								 currentInstrumentIndex_(0),
								 currentStatuses_(0),
								 panButtons_(0),
								 instrumentButtons_(0),
								 instrumentButtonIndexes_{31, 30, 29, 28, 27, 26, 25, 24, 32, 33},
								 stepButtonIndexes_{43, 42, 41, 40, 39, 38, 37, 36, 44, 45, 46, 47, 20, 21, 22, 23},
								 panButtonIndexes_{15, 14, 13, 12}{

}

SetActiveView::~SetActiveView() {
	delete panButtons_;
	delete instrumentButtons_;
}

void SetActiveView::init(IHWLayer * hw, IStepMemory * memory, unsigned char pattern) {
	hw_ = hw;
	memory_ = memory;
	currentPattern_ = pattern;
	panButtons_ = new RadioButtons(hw, panButtonIndexes_, 4);
	instrumentButtons_ = new RadioButtons(hw, instrumentButtonIndexes_, 10);
	bottomSwitches_.init(hw, stepButtonIndexes_, 8);
	topSwitches_.init(hw, &stepButtonIndexes_[8], 8);
	updateConfiguration();
}

void SetActiveView::updateConfiguration() {
	for (unsigned char i = 0; i < 4; i++) {
		hw_->setLED(panButtonIndexes_[i], i == currentPanIndex_ ? IHWLayer::ON : IHWLayer::OFF);
	}
	for (unsigned char i = 0; i < 10; i++) {
		hw_->setLED(instrumentButtonIndexes_[i], i == currentInstrumentIndex_ ? IHWLayer::ON : IHWLayer::OFF);
	}
	updateActives();
}

void SetActiveView::updateActives() {
	unsigned char data[4] = {0, 0, 0, 0};
	memory_->getActivesAndMutesForNote(currentInstrumentIndex_, currentPattern_, currentPanIndex_ * 2, data);
	for (unsigned char i = 0; i < 16; i++) {
		bool stepActive = GETBIT(data[i / 8], i % 8);
		hw_->setLED(stepButtonIndexes_[i], stepActive ? IHWLayer::ON : IHWLayer::OFF);
		Switches * currentSwitches = (i / 8 == 0) ? &bottomSwitches_ : &topSwitches_;
		currentSwitches->setStatus(i % 8, stepActive);
	}
	currentStatuses_ = ((0 | data[0]) << 8) | data[1];
}

void SetActiveView::update() {

	panButtons_->update();
	instrumentButtons_->update();
	bottomSwitches_.update();
	topSwitches_.update();

	unsigned char newInstrument = 0;
	if (instrumentButtons_->getSelectedButton(newInstrument) && currentInstrumentIndex_ != newInstrument) {
		currentInstrumentIndex_ = newInstrument;
		currentPanIndex_ = 0;
		panButtons_->setSelectedButton(0);
		updateConfiguration();
		return;
	}
	unsigned char newPan = 0;
	if (panButtons_->getSelectedButton(newPan) && currentPanIndex_ != newPan) {
		currentPanIndex_ = newPan;
		updateConfiguration();
		return;
	}
	for (unsigned char i = 0; i < 16; i++) {
		bool currentState = GETBIT(currentStatuses_, i);
		Switches * currentSwitches = (i / 8 == 0) ? &bottomSwitches_ : &topSwitches_;
		bool newState = currentSwitches->getStatus(i % 8);
		if (currentState != newState) {
			DrumStep step = memory_->getDrumStep(currentInstrumentIndex_, currentPattern_, (currentPanIndex_ * 16) + i);
			step.setActive(newState);
			memory_->setDrumStep(currentInstrumentIndex_, currentPattern_, (currentPanIndex_ * 16) + i, step);
			hw_->setLED(stepButtonIndexes_[i], newState ? IHWLayer::ON : IHWLayer::OFF);
			currentState = newState;
			newState ?  SETBITTRUE(currentStatuses_, i) : SETBITFALSE(currentStatuses_, i);
		}
	}
}

