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
#include <mutex>
class DotController{

public:
	enum FlipMode{ Flip,On, Off };
	DotController();
	~DotController();
	void setup(cinder::Vec2f &gridPosition, cinder::Vec2f &gridSize, float &dotRadius, float &dotSpacing);
	void setDotState(cinder::Vec2i index, Dot::DotState state);
	void flipDot(cinder::Vec2i index);

	void update();
	void draw();
	cinder::Vec2f getDotPosition(cinder::Vec2i index);
	int getNumRows(){ return mDotGrid.size(); };
	int getNumCols(){ return mDotGrid.at(0).size(); };

	void addDotUpdate(cinder::Vec2i index, FlipMode mode);
	static void toggleGravity(){ mGravity = !mGravity; };

protected:
	void stepGravity();
	cinder::Vec2f	mPosition,
					mSize;


	std::vector<std::vector<class Dot>> mDotGrid;

	//Audio
	void createAudioPlayer();
	bool		fmodCheck(FMOD_RESULT result, const std::string& label = "", bool releaseOnError = false);

	FMOD::Channel*				mChannelSound;
	FMOD::Channel*				mChannelSynth;
	FMOD::Sound*				mSound;
	FMOD::System*				mSystem;

	float						mSoundLevel;
	
	std::mutex mDotUpdateMutex;
	std::deque<std::pair<cinder::Vec2i, FlipMode> > dotUpdateList;
	static bool mGravity;

	double lastUpdateTime;
	
};