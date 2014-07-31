/*
 * MainMenuView.h
 *
 *  Created on: Jul 25, 2014
 *      Author: bastl
 */

#ifndef MAINMENUVIEW_H_
#define MAINMENUVIEW_H_

#include <controls/IView.h>
#include <hw/IHWLayer.h>
#include <players/Player.h>
#include <data/IStepMemory.h>
#include <controls/RadioButtons.h>
#include <players/PlayerSettings.h>

class MainMenuView : public IView {
public:
	MainMenuView();
	~MainMenuView();
	void init(IHWLayer * hw, Player * player, IStepMemory * memory, PlayerSettings * settings);
	void update();
private:

	IHWLayer * hw_;
	Player * player_;
	IStepMemory * memory_;
	PlayerSettings * settings_;

	IView * currentView_;
	unsigned char currentViewIndex_;
	unsigned char currentPattern_;

	RadioButtons * modeButtons_;
	unsigned char modeButtonIndexes_[4] ;

	void createView(unsigned char viewIndex);

};

#endif /* MAINMENUVIEW_H_ */
