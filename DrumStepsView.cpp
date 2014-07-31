/*
 * drumStepsView.cpp
 *
 *  Created on: Jul 29, 2014
 *      Author: bastl
 */

#include <DrumStepsView.h>

DrumStepsView::DrumStepsView() :
	hw_(0),
	currentStatus_(0),
	lastStatus_(0),
	stepButtonIndexes_{43, 42, 41, 40, 39, 38, 37, 36, 44, 45, 46, 47, 20, 21, 22, 23},
	currentDownButton_(-1),
	ignoreOffs_(false),
	invertedButton_(-1){

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

	void DrumStepsView::setInvertedButton(char invertedButton) {
		if (invertedButton_ != invertedButton) {
			if (invertedButton_ != -1) {
				bool stepValue = ((currentStatus_ & (1 << invertedButton_)) != 0);
				hw_->setLED(stepButtonIndexes_[invertedButton_], stepValue ? IHWLayer:: ON : IHWLayer::OFF);
			}
			invertedButton_ = invertedButton;
			if (invertedButton_ != -1) {
				bool stepValue = ((currentStatus_ & (1 << invertedButton_)) != 0);
				hw_->setLED(stepButtonIndexes_[invertedButton_], stepValue ? IHWLayer:: OFF : IHWLayer::ON);
			}
		}
	}
