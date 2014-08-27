/*
 * PatternView.h
 *
 *  Created on: Jul 25, 2014
 *      Author: bastl
 */

#ifndef PATTERNVIEW_H_
#define PATTERNVIEW_H_

#include <IView.h>
#include <IStepMemory.h>
#include <RadioButtons.h>
#include <Switches.h>
#include <PlayerSettings.h>

class PatternView  : public IView {
public:
	PatternView();
	~PatternView();
	void init(IHWLayer * hw, PlayerSettings * settigns, IStepMemory * memory);
	void update();
private:

	IHWLayer * hw_;
	PlayerSettings * settings_;
	IStepMemory * memory_;

	//Controls
	Switches instrumentTopSwitches_;
	Switches instrumentBottomSwitches_;
	RadioButtons * patternSelectRadioButtons_;

	unsigned char instrumentSwitches_[10];
	unsigned char patternSelectButtons_[16];

	unsigned char currentInstrumentStatuses_[3];
	unsigned char currentPattern_;

	void reflectPatternChange();
};

#endif /* PATTERNVIEW_H_ */
