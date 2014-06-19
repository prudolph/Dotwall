/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <iostream>

#include "cinder/app/AppNative.h"
#include "LeapListener.h"
using namespace Leap;
using namespace ci;
using namespace ci::app;
using namespace std;
const std::string fingerNames[] = { "Thumb", "Index", "Middle", "Ring", "Pinky" };
const std::string boneNames[] = { "Metacarpal", "Proximal", "Middle", "Distal" };
const std::string stateNames[] = { "STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END" };

void LeapListener::onInit(const Controller& controller) {
	console() << "Initialized" << std::endl;
}

void LeapListener::onConnect(const Controller& controller) {
	console() << "Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_CIRCLE);
	controller.enableGesture(Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Gesture::TYPE_SWIPE);
	
}

void LeapListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	console() << "Disconnected" << std::endl;
}

void LeapListener::onExit(const Controller& controller) {
	console() << "Exited" << std::endl;
}

void LeapListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	const InteractionBox iBox = frame.interactionBox();


	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;


		// Get fingers
		if (hand.grabStrength() > 0.85)mDotWallRef->setGravity(true);
		else mDotWallRef->setGravity(false);

		const FingerList fingers = hand.fingers();
		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			//if (finger.type() == 1){//Is it the index finger

				//Get the finger point in relation to the application window
				Vector normalPnt = iBox.normalizePoint(finger.stabilizedTipPosition());
				float x = normalPnt.x * getWindowWidth();
				float y = getWindowHeight() - normalPnt.y * getWindowHeight();

				if (finger.touchZone() ==  finger.isExtended()){
					x = (float)(x) / (float)(getWindowWidth())*mDotWallRef->getNumCols();
					 y = (float)(y) / (float)(getWindowHeight())*mDotWallRef->getNumRows();
					Vec2f fingerTipPos = Vec2f(x, y);
					mDotWallRef->addDotUpdate(fingerTipPos, DotController::FlipMode::Flip);


				}
			//}

		}

	}
}

void LeapListener::onFocusGained(const Controller& controller) {
	console() << "Focus Gained" << std::endl;
}

void LeapListener::onFocusLost(const Controller& controller) {
	console() << "Focus Lost" << std::endl;
}

void LeapListener::onDeviceChange(const Controller& controller) {
	console() << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		console() << "id: " << devices[i].toString() << std::endl;
		console() << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void LeapListener::onServiceConnect(const Controller& controller) {
	console() << "Service Connected" << std::endl;
}

void LeapListener::onServiceDisconnect(const Controller& controller) {
	console() << "Service Disconnected" << std::endl;
}
