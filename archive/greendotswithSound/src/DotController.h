//
//  DotController.h
//  Dot
//
//  Created by Paul Rudolph on 7/2/13.
//
//

#pragma once
#include "cinder/app/AppNative.h"
#include "Dot.h"


//Audio

//FMOD
#include "FMOD.hpp"
#include "fmod_errors.h"
class DotController{

public:

	DotController();
	~DotController();
	void setup(cinder::Vec2f &gridPosition, cinder::Vec2f &gridSize, cinder::Vec2i &resolution);
	void setDotState(cinder::Vec2i index, Dot::DotState state);
	void flipDot(cinder::Vec2i index);

	void update();
	void draw();

	int getNumRows(){ return mDotGrid.size(); };
	int getNumCols(){ return mDotGrid.at(0).size(); };


protected:
	
	cinder::Vec2f	mPosition,
					mSize;
	cinder::Vec2i	mResolution;

	std::vector<std::vector<class Dot>> mDotGrid;

	//Audio
	void createAudioPlayer();
	bool		fmodCheck(FMOD_RESULT result, const std::string& label = "", bool releaseOnError = false);

	FMOD::Channel*				mChannelSound;
	FMOD::Channel*				mChannelSynth;
	FMOD::Sound*				mSound;
	FMOD::System*				mSystem;

	float						mSoundLevel;

};