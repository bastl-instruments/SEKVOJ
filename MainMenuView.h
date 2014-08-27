/*
 * MainMenuView.h
 *
 *  Created on: Jul 25, 2014
 *      Author: bastl
 */

#ifndef MAINMENUVIEW_H_
#define MAINMENUVIEW_H_

#include <IView.h>
#include <IHWLayer.h>
#include <Player.h>
#include <IStepMemory.h>
#include <RadioButtons.h>
#include <PlayerSettings.h>
#include <BastlMetronome.h>

class MainMenuView : public IView {
public:
	MainMenuView();
	~MainMenuView();
	void init(IHWLayer * hw, Player * player, BastlMetronome * stepper, IStepMemory * memory, PlayerSettings * settings, IMIDICommandProcessor * midiProcessor);
	void update();
private:

	IHWLayer * hw_;
	Player * player_;
	BastlMetronome * stepper_;
	IStepMemory * memory_;
	PlayerSettings * settings_;
	IMIDICommandProcessor * midiProcessor_;

	IView * currentView_;
	unsigned char currentViewIndex_;
	unsigned char currentPattern_;

	RadioButtons * modeButtons_;
	unsigned char modeButtonIndexes_[4] ;

	void createView(unsigned char viewIndex);

};

#endif /* MAINMENUVIEW_H_ */
