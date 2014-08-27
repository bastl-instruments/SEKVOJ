/*
 * drumStepsView.cpp
 *
 *  Created on: Jul 29, 2014
 *      Author: bastl
 */

#include "DrumStepsView.h"

DrumStepsView::DrumStepsView() :
	hw_(0),
	currentStatus_(0),
	lastStatus_(0),
	currentDownButton_(-1),
	ignoreOffs_(false),
	invertedButton_(-1){
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
    
}

DrumStepsView::~DrumStepsView() {
}

	void DrumStepsView::init (IHWLayer * hw) {
		hw_ = hw;
		stepSwitches_.init(hw_, stepButtonIndexes_, 16, IHWLayer::UP);
	}

	void DrumStepsView::update() {
		lastStatus_ = currentStatus_;
		stepSwitches_.update();
		currentDownButton_ = -1;
		for (unsigned char i = 0; i < 16; i++) {
			if (hw_->getButtonState(stepButtonIndexes_[i]) == IHWLayer::DOWN) {
				currentDownButton_ = i;
			}
			bool stepValue = (currentStatus_ & (1 << i)) != 0;
			bool newStepValue = stepSwitches_.getStatus(i);
			if (stepValue != newStepValue) {
				if (newStepValue) {
					hw_->setLED(stepButtonIndexes_[i], (i == invertedButton_) ? IHWLayer:: OFF : IHWLayer::ON);
					currentStatus_ = currentStatus_ | (1 << i);
				} else {
					if (ignoreOffs_) {
						stepSwitches_.setStatus(i, true);
					} else {
						hw_->setLED(stepButtonIndexes_[i], (i == invertedButton_) ? IHWLayer::ON : IHWLayer::OFF);
						currentStatus_ = currentStatus_ & ~(1 << i);
					}
				}
			}
		}
	}

	void DrumStepsView::setStatus(unsigned int status) {
		currentStatus_ = status;
		lastStatus_ = status;
		for (unsigned char i = 0; i < 16; i++) {
			bool stepValue = (currentStatus_ & (1 << i)) != 0;
			stepSwitches_.setStatus(i, stepValue);
			hw_->setLED(stepButtonIndexes_[i], stepValue ? IHWLayer::ON: IHWLayer::OFF);
		}
	}

	unsigned int DrumStepsView::getNewOffs() {
		return (lastStatus_ ^  currentStatus_) & lastStatus_;
	}

	unsigned int DrumStepsView::getNewOns() {
		return (lastStatus_ ^  currentStatus_) & currentStatus_;
	}

	void DrumStepsView::setInvertedButton(unsigned char invertedButton) {
		if (invertedButton_ != invertedButton) {
			if (invertedButton_ != -1) {
				bool stepValue = ((currentStatus_ & (1 << invertedButton_)) != 0);
				hw_->setLED(stepButtonIndexes_[(unsigned char)invertedButton_], stepValue ? IHWLayer:: ON : IHWLayer::OFF);
			}
			invertedButton_ = invertedButton;
			if (invertedButton_ != -1) {
				bool stepValue = ((currentStatus_ & (1 << invertedButton_)) != 0);
				hw_->setLED(stepButtonIndexes_[(unsigned char)invertedButton_], stepValue ? IHWLayer:: OFF : IHWLayer::ON);
			}
		}
	}
