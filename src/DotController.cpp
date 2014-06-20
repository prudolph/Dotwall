//
//  DotController.cpp
//  DotController
//
//  Created by Paul Rudolph on 7/2/13.
//
//

#include "DotController.h"

using namespace std;
using namespace ci;
using namespace ci::app;


bool DotController::mGravity = false;

DotController::DotController() :
mPosition(Vec2f(0.0f,0.0f)),
mSize(Vec2f(0.0f, 0.0f)),


//Audio
mChannelSound(0),
mChannelSynth(0),
mSound(0),
mSystem(0),
mSoundLevel(1)

{

}
DotController::~DotController(){
}
void DotController::setup(cinder::Vec2f &gridPosition, cinder::Vec2f &gridSize, float &dotRadius, float &dotSpacing){
	mPosition	= gridPosition;
	mSize		= gridSize;


	float maxColumns	= floor(gridSize.x / (dotRadius + dotSpacing));
	float maxRows		= floor(gridSize.y / (dotRadius + dotSpacing));

	Vec2f currentPos = mPosition+Vec2f(dotRadius, dotRadius);
	for (int col = 0; col < maxColumns; col++){
		vector<class Dot> dotRow;
		for (int row = 0; row < maxRows; row++){
		
		
				Dot d = Dot();
			d.setup(currentPos, dotRadius);
			dotRow.push_back(d);
			currentPos.x += dotRadius * 2 + dotSpacing;
		}

		mDotGrid.push_back(dotRow);
		currentPos.x = mPosition.x + dotRadius;
		currentPos.y += dotRadius * 2 + dotSpacing;
		
	}


	//createAudioPlayer();
}
Vec2f DotController::getDotPosition(Vec2i index){
	if (index.x >= 0 &&
		index.x < getNumCols() &&
		index.y >= 0 &&
		index.y < getNumRows()){


		return mDotGrid.at(index.y).at(index.x).getDotPos();
	}
	return Vec2f::zero();
}
void DotController::setDotState(cinder::Vec2i index, Dot::DotState state){
	//Make sure the index is within the bounds
	if (index.x >= 0 &&
		index.x <getNumCols() &&
		index.y >= 0 &&
		index.y < getNumRows()){

		mDotGrid.at(index.y).at(index.x).setDotState(state);

	}
}

void DotController::flipDot(cinder::Vec2i index){
	if (index.x >= 0 &&
		index.x < getNumCols() &&
		index.y >= 0 &&
		index.y < getNumRows()){
		//mSystem->playSound(FMOD_CHANNELINDEX::FMOD_CHANNEL_FREE, mSound, false, &mChannelSound);

		mDotGrid.at(index.y).at(index.x).flipDot();

		
	}
	
}
void DotController::addDotUpdate(cinder::Vec2i index, FlipMode mode){
	mDotUpdateMutex.lock();
		dotUpdateList.push_back(std::pair<cinder::Vec2i, FlipMode>(index, mode));
	mDotUpdateMutex.unlock();
}

void DotController::update(){
	//mSystem->update();
	
	while (!dotUpdateList.empty()){
		std::pair<cinder::Vec2i, FlipMode> update;
		
		mDotUpdateMutex.lock();
			update = dotUpdateList.front();
			dotUpdateList.pop_front();
		mDotUpdateMutex.unlock();

		switch (update.second) {
		case Flip:
			flipDot(update.first);
			break;
		case On:
			setDotState(update.first, Dot::DotState::On);
			break;
		case Off:
			setDotState(update.first, Dot::DotState::Off);
			break;
		}

	}
	if (mGravity == true){
		double elapsedTime = getElapsedSeconds() - lastUpdateTime;
		
		
		if (((elapsedTime) > 1.0f / 12.0f)){
			lastUpdateTime = getElapsedSeconds();
			stepGravity();
		}
		
	}
}

void DotController::stepGravity(){
	console() << "DotWAll :: Step Gravity" << endl;
	for (int x = getNumCols() - 1; x >= 0; x--){

		for (int y = getNumRows() - 1; y >= 0; y--){
		


			if (mDotGrid.at(y).at(x).getDotState() == Dot::DotState::On){
				setDotState(Vec2i(x, y ), Dot::DotState::Off);
		
				setDotState(Vec2i(x,y+3), Dot::DotState::On);
			
			}
		}
	}


}
void DotController::draw(){
	
	for (int row = 0; row < getNumRows(); row++){
		for (int col = 0; col < getNumCols(); col++){
			mDotGrid.at(col).at(row).draw();
		}
	}

}

//Audio

void DotController::createAudioPlayer(){
	mChannelSound->setVolume(mSoundLevel);

	if (!fmodCheck(FMOD::System_Create(&mSystem), "Create system") || mSystem == 0) {
		console() << "system not created" << endl;
		return;
	}
	else {
		console() << "system CREATED" << endl;
	}
	if (!fmodCheck(mSystem->init(100, FMOD_INIT_NORMAL, 0), "Initialize system")) {
		console() << "system not initialized" << endl;
		return;
	}

	fs::path soundPath = getAssetPath("clack.wav");

	// Load and play the loop
	if (!fmodCheck(mSystem->createSound(soundPath.string().c_str(), FMOD_SOFTWARE, 0, &mSound), "Create sound")) {
		console() << "Unable to load sound" << endl;
	}

	fmodCheck(mSound->setMode(FMOD_LOOP_OFF), "Set loop");		//this does not loop audio



}
bool DotController::fmodCheck(FMOD_RESULT result, const std::string& label, bool releaseOnError){

	if (result == FMOD_OK) {
		return true;
	}

	console() << FMOD_ErrorString(result) << endl;
	if (releaseOnError && mSystem != 0) {
		mSystem->release();
	}

	return false;


}
