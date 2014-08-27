/*
 * SetActiveView.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: bastl
 */

#include "SetActiveView.h"


SetActiveView::SetActiveView() : hw_(0),
								 memory_(0),
								 player_(0),
								 stepper_(0),
								 currentPattern_(0),
								 currentPanIndex_(0),
								 currentInstrumentIndex_(0),
								 currentStatuses_(0),
								 panButtons_(0),
								 instrumentButtons_(0){
  
  stepButtonIndexes_[0] = 43;
  stepButtonIndexes_[1] = 42;
  stepButtonIndexes_[2] = 41;
  stepButtonIndexes_[3] = 40;
  stepButtonIndexes_[4] = 39;
  stepButtonIndexes_[5] = 38;
  stepButtonIndexes_[6] = 37;
  stepButtonIndexes_[7] = 36;
  stepButtonIndexes_[8] = 44;
  stepButtonIndexes_[9] = 45;
  stepButtonIndexes_[10] = 46;
  stepButtonIndexes_[11] = 47;
  stepButtonIndexes_[12] = 20;
  stepButtonIndexes_[13] = 21;
  stepButtonIndexes_[14] = 22;
  stepButtonIndexes_[15] = 23;
  instrumentButtonIndexes_[0] =31;
  instrumentButtonIndexes_[1] =30;
  instrumentButtonIndexes_[2] =29;
  instrumentButtonIndexes_[3] =28;
  instrumentButtonIndexes_[4] =27;
  instrumentButtonIndexes_[5] =26;
  instrumentButtonIndexes_[6] =25;
  instrumentButtonIndexes_[7] =24;
  instrumentButtonIndexes_[8] =32;
  instrumentButtonIndexes_[9] =33;
  panButtonIndexes_[0] = 15;
  panButtonIndexes_[1] = 14;
  panButtonIndexes_[2] = 13;
  panButtonIndexes_[3] = 12;

}

SetActiveView::~SetActiveView() {
	delete panButtons_;
	delete instrumentButtons_;
}

void SetActiveView::init(IHWLayer * hw, IStepMemory * memory, Player * player, BastlMetronome * stepper, unsigned char pattern) {
	hw_ = hw;
	memory_ = memory;
	stepper_ = stepper;
	player_ = player;
	currentPattern_ = pattern;
	panButtons_ = new RadioButtons(hw, panButtonIndexes_, 4);
	instrumentButtons_ = new RadioButtons(hw, instrumentButtonIndexes_, 10);
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
	memory_->getActivesAndMutesForNote(currentInstrumentIndex_, currentPattern_, currentPanIndex_ * 2,  data);
	for (unsigned char i = 0; i < 16; i++) {
		bool stepActive = GETBIT(data[i / 8], i % 8);
		hw_->setLED(stepButtonIndexes_[i], stepActive ? IHWLayer::ON : IHWLayer::OFF);
	}
}

void SetActiveView::update() {

	panButtons_->update();
	instrumentButtons_->update();

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
		bool lastState = GETBIT(currentStatuses_, i);
		bool currentState = hw_->getButtonState(stepButtonIndexes_[i]);
		bool shift = false;
		unsigned char pressedStep = (currentPanIndex_ * 16) + i;
		if (lastState != currentState) {
			//Update
			currentState ? SETBITTRUE(currentStatuses_, i) : SETBITFALSE(currentStatuses_, i);
			if (shift) {
				DrumStep step = memory_->getDrumStep(currentInstrumentIndex_, currentPattern_, pressedStep);
				step.setActive(!step.isActive());
				memory_->setDrumStep(currentInstrumentIndex_, currentPattern_, pressedStep, step);
				hw_->setLED(stepButtonIndexes_[i], step.isActive() ? IHWLayer::ON : IHWLayer::OFF);
			} else if (currentState) {
				for (int stepIndex = 0; stepIndex < 64; stepIndex++) {
					bool newState = stepIndex <= pressedStep;
					DrumStep step = memory_->getDrumStep(currentInstrumentIndex_, currentPattern_, stepIndex);
					step.setActive(newState);
					memory_->setDrumStep(currentInstrumentIndex_, currentPattern_, stepIndex, step);
					if (stepIndex / 16 == currentPanIndex_) {
						hw_->setLED(stepButtonIndexes_[stepIndex % 16], newState ? IHWLayer::ON : IHWLayer::OFF);
					}
				}
				player_->setCurrentInstrumentStep(currentInstrumentIndex_, (stepper_->getCurrentStepIndex() / 4) % (pressedStep + 1));
			}

		}
	}
}

