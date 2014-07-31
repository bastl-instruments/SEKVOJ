/*
 * PatternView.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: bastl
 */

#include <PatternView.h>
#include <data/InstrumentDefinitions.h>

PatternView::PatternView() : hw_(0), settings_(0), memory_(0),
							 instrumentTopSwitches_(0),
							 instrumentBottomSwitches_(0),
							 patternSelectRadioButtons_(0),
							 instrumentSwitches_{31, 30, 29, 28, 27, 26, 25, 24, 32, 33},
							 patternSelectButtons_{43, 42, 41, 40, 39, 38, 37, 36, 44, 45, 46, 47, 20, 21, 22, 23},
							 currentInstrumentStatuses_{0, 0, 0},
							 currentPattern_(0) {
}

PatternView::~PatternView() {
	delete instrumentBottomSwitches_;
	delete instrumentTopSwitches_;
	delete patternSelectRadioButtons_;
}

void PatternView::init(IHWLayer * hw, PlayerSettings * settigns, IStepMemory * memory) {
	hw_ = hw;
	settings_ = settigns;
	memory_ = memory;
	currentPattern_ = settings_->getCurrentPattern();

	instrumentBottomSwitches_ = new Switches(hw_, instrumentSwitches_, 8);
	instrumentTopSwitches_ = new Switches(hw_, &instrumentSwitches_[8], 2);
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
		Switches * currentSwitches = (i / 8 == 0) ? instrumentBottomSwitches_ : instrumentTopSwitches_;
		currentSwitches->setStatus(i % 8, instrumentStatus);
	}
}

void PatternView::update() {
	patternSelectRadioButtons_->update();
	instrumentBottomSwitches_->update();
	instrumentTopSwitches_->update();

	unsigned char newPattern = 0;
	if (patternSelectRadioButtons_->getSelectedButton(newPattern) && newPattern != currentPattern_) {
		hw_->setLED(patternSelectButtons_[currentPattern_], IHWLayer::OFF);
		hw_->setLED(patternSelectButtons_[newPattern], IHWLayer::ON);
		currentPattern_ = newPattern;
		reflectPatternChange();
		return;
	}
	for (unsigned char i = 0; i < 10; i++) {
		Switches * currentSwitches = (i / 8 == 0) ? instrumentBottomSwitches_ : instrumentTopSwitches_;
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
