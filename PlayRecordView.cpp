/*
 * PlayRecordView.cpp
 *
 *  Created on: Jul 31, 2014
 *      Author: bastl
 */

#include "PlayRecordView.h"
#include <MIDICommand.h>
#include <DrumStep.h>

PlayRecordView::PlayRecordView() : hw_(0),
								   midiProcessor_(0),
								   settings_(0),
								   currentStatus_(0){
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
}

PlayRecordView::~PlayRecordView() {
}

void PlayRecordView::init(IHWLayer * hw, IMIDICommandProcessor * midiPorcessor, PlayerSettings * settings, Player * player, IStepMemory * memory) {
	hw_ = hw;
	midiProcessor_ = midiPorcessor;
	settings_ = settings;
	player_ = player;
	memory_ = memory;
}

void PlayRecordView::update() {
	unsigned int newStatuses = 0;
	for (unsigned char i = 0; i < 10; i++) {
		bool currentStatus = (currentStatus_ & (1 << i)) != 0;
		bool newStatus = hw_->getButtonState(instrumentButtonIndexes_[i]) == IHWLayer::DOWN;
		if (!currentStatus && newStatus) {
			unsigned char currentStep = player_->getCurrentInstrumentStep(i);
			unsigned char currentSubStep = player_->getCurrentInstrumentSubStep(i);
			DrumStep step = memory_->getDrumStep(i, 0, currentStep);

			if (step.isMuted() || step.getSubStep(currentSubStep) == DrumStep::OFF) {
				if (step.isMuted()) {
					step.setMuted(false);
					for (unsigned char subStep = 0; subStep < 4; subStep++) {
						step.setSubStep(subStep, DrumStep::OFF);
					}
				}
				step.setSubStep(currentSubStep, DrumStep::NORMAL);
				memory_->setDrumStep(i, 0, currentStep, step);
				player_->playNote(i, DrumStep::NORMAL);
			}
		}
		if (newStatus) {
			newStatuses = newStatuses | 1 << i;
		} else {
			newStatuses = newStatuses & ~(1 << i);
		}
	}
	currentStatus_ = newStatuses;

}
