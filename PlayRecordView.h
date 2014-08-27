/*
 * PlayRecordView.h
 *
 *  Created on: Jul 31, 2014
 *      Author: bastl
 */

#ifndef PLAYRECORDVIEW_H_
#define PLAYRECORDVIEW_H_

#include <IView.h>
#include <IHWLayer.h>
#include <ArduinoMIDICommandProcessor.h>
#include <PlayerSettings.h>
#include <Player.h>
#include "DrumStepsView.h"
#include <IStepMemory.h>

class PlayRecordView : public IView {
public:
	PlayRecordView();
	~PlayRecordView();
	void init(IHWLayer * hw, IMIDICommandProcessor * midiProcessor, PlayerSettings * settings, Player * player, IStepMemory * memory);
	void update();
private:
	IHWLayer * hw_;
	IMIDICommandProcessor * midiProcessor_;
	PlayerSettings * settings_;
	Player * player_;
	IStepMemory * memory_;
	unsigned char instrumentButtonIndexes_[10];
	unsigned int currentStatus_;
};

#endif /* PLAYRECORDVIEW_H_ */
