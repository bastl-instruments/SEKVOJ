/*
 * PatternView.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: bastl
 */

#include "PatternView.h"
#include <InstrumentDefinitions.h>

PatternView::PatternView() : hw_(0), settings_(0), memory_(0),
							 patternSelectRadioButtons_(0),
							 currentPattern_(0) {
  currentInstrumentStatuses_[0] = 0;
  currentInstrumentStatuses_[1] = 0;
  currentInstrumentStatuses_[2] = 0;
  patternSelectButtons_[0] = 43;
  patternSelectButtons_[1] = 42;
  patternSelectButtons_[2] = 41;
  patternSelectButtons_[3] = 40;
  patternSelectButtons_[4] = 39;
  patternSelectButtons_[5] = 38;
  patternSelectButtons_[6] = 37;
  patternSelectButtons_[7] = 36;
  patternSelectButtons_[8] = 44;
  patternSelectButtons_[9] = 45;
  patternSelectButtons_[10] = 46;
  patternSelectButtons_[11] = 47;
  patternSelectButtons_[12] = 20;
  patternSelectButtons_[13] = 21;
  patternSelectButtons_[14] = 22;
  patternSelectButtons_[15] = 23;
  instrumentSwitches_[0] =31;
  instrumentSwitches_[1] =30;
  instrumentSwitches_[2] =29;
  instrumentSwitches_[3] =28;
  instrumentSwitches_[4] =27;
  instrumentSwitches_[5] =26;
  instrumentSwitches_[6] =25;
  instrumentSwitches_[7] =24;
  instrumentSwitches_[8] =32;
  instrumentSwitches_[9] =33;
  
  
    
}

PatternView::~PatternView() {
	delete patternSelectRadioButtons_;
}

void PatternView::init(IHWLayer * hw, PlayerSettings * settigns, IStepMemory * memory) {
	hw_ = hw;
	settings_ = settigns;
	memory_ = memory;
	currentPattern_ = settings_->getCurrentPattern();

	instrumentBottomSwitches_.init(hw_, instrumentSwitches_, 8);
	instrumentTopSwitches_.init(hw_, &instrumentSwitches_[8], 2);
	patternSelectRadioButtons_ = new RadioButtons(hw_, patternSelectButtons_, 16);
	patternSelectRadioButtons_->setSelectedButton(currentPattern_);

	for (unsigned char i = 0; i < 16; i++) {
		hw_->setLED(patternSelectButtons_[i], (i == currentPattern_)  ? IHWLayer::ON : IHWLayer::OFF);
	}
	reflectPatternChange();
}

void PatternView::reflectPatternChange() {
	settings_->setCurrentPattern(currentPattern_);
	memory_->getPatternSettings(currentPattern_, currentInstrumentStatuses_);
	for (unsigned char i = 0; i < 10; i++) {
		bool instrumentStatus = GETBIT(currentInstrumentStatuses_[i / 8], i % 8);
		hw_->setLED(instrumentSwitches_[i], instrumentStatus ? IHWLayer::ON : IHWLayer::OFF);
		settings_->setInstrumentOn(InstrumentTypes::DRUM, i, instrumentStatus);
		Switches * currentSwitches = (i / 8 == 0) ? &instrumentBottomSwitches_ : &instrumentTopSwitches_;
		currentSwitches->setStatus(i % 8, instrumentStatus);
	}
}

void PatternView::update() {
	patternSelectRadioButtons_->update();
	instrumentBottomSwitches_.update();
	instrumentTopSwitches_.update();

	unsigned char newPattern = 0;
	if (patternSelectRadioButtons_->getSelectedButton(newPattern) && newPattern != currentPattern_) {
		hw_->setLED(patternSelectButtons_[currentPattern_], IHWLayer::OFF);
		hw_->setLED(patternSelectButtons_[newPattern], IHWLayer::ON);
		currentPattern_ = newPattern;
		reflectPatternChange();
		return;
	}
	for (unsigned char i = 0; i < 10; i++) {
		Switches * currentSwitches = (i / 8 == 0) ? &instrumentBottomSwitches_ : &instrumentTopSwitches_;
		bool newStatus = currentSwitches->getStatus(i % 8);
		bool oldStatus = GETBIT(currentInstrumentStatuses_[i / 8], i % 8);
		if (newStatus != oldStatus) {
			settings_->setInstrumentOn(InstrumentTypes::DRUM, i, newStatus);
			if (newStatus) {
				SETBITTRUE(currentInstrumentStatuses_[i / 8], i % 8);
			} else {
				SETBITFALSE(currentInstrumentStatuses_[i / 8], i % 8);
			}
			memory_->setPatternSettings(currentPattern_, currentInstrumentStatuses_);
			hw_->setLED(instrumentSwitches_[i], newStatus ? IHWLayer::ON : IHWLayer::OFF);
		}
	}
}
