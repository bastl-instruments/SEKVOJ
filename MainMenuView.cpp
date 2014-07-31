/*
 * MainMenuView.cpp
 *
 *  Created on: Jul 25, 2014
 *      Author: bastl
 */

#include "MainMenuView.h"
#include "SetActiveView.h"
#include "PatternView.h"
#include "SetStepView.h"

MainMenuView::MainMenuView() : hw_(0), player_(0), memory_(0), currentView_(0), currentViewIndex_(0), currentPattern_(0),
							   modeButtons_(0) , modeButtonIndexes_{19, 18, 17, 16}{

}

MainMenuView::~MainMenuView() {
	delete modeButtons_;
}

void MainMenuView::init(IHWLayer * hw, Player * player, IStepMemory * memory, PlayerSettings * settings) {
	hw_ = hw;
	player_ = player;
	memory_ = memory;
	settings_ = settings;

	modeButtons_ = new RadioButtons(hw_, modeButtonIndexes_, 4);
	createView(currentViewIndex_);
	hw_->setLED(modeButtonIndexes_[0], IHWLayer::ON);
	hw_->setLED(modeButtonIndexes_[1], IHWLayer::OFF);
	hw_->setLED(modeButtonIndexes_[2], IHWLayer::OFF);
	hw_->setLED(modeButtonIndexes_[3], IHWLayer::OFF);
}

void MainMenuView::createView(unsigned char viewIndex) {
	switch (viewIndex) {
	case 0: {
		SetActiveView * activeView = new SetActiveView();
		activeView->init(hw_, memory_, settings_->getCurrentPattern());
		currentView_ = (IView*)activeView;
		break;
	}
	case 1: {
		PatternView * patternView = new PatternView();
		patternView->init(hw_, settings_, memory_);
		currentView_ = (IView*)patternView;
		break;
	}
	case 2: {
		SetStepView * setStepView = new SetStepView();
		setStepView->init(hw_, memory_, player_, settings_->getCurrentPattern());
		currentView_ = (IView*)setStepView;
		break;
	}
	case 3:
		break;
	}
}

void MainMenuView::update() {
	modeButtons_->update();
	unsigned char newIndex = 0;
	bool buttonSelected = modeButtons_->getSelectedButton(newIndex);
	if (buttonSelected && newIndex != currentViewIndex_) {
		hw_->setLED(modeButtonIndexes_[currentViewIndex_], IHWLayer::OFF);
		hw_->setLED(modeButtonIndexes_[newIndex], IHWLayer::ON);
		currentViewIndex_ = newIndex;
		if (currentView_ != 0) {
			delete currentView_;
			currentView_ = 0;
		}
		createView(currentViewIndex_);
		return;
	}
	if (currentView_) {
		currentView_->update();
	}
}


