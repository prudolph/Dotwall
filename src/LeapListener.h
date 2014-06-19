#pragma once
#include "Leap.h"
#include "DotController.h"


class LeapListener : public Leap::Listener {
public:
	virtual void onInit(const Leap::Controller&);
	virtual void onConnect(const Leap::Controller&);
	virtual void onDisconnect(const Leap::Controller&);
	virtual void onExit(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
	virtual void onFocusGained(const Leap::Controller&);
	virtual void onFocusLost(const Leap::Controller&);
	virtual void onDeviceChange(const Leap::Controller&);
	virtual void onServiceConnect(const Leap::Controller&);
	virtual void onServiceDisconnect(const Leap::Controller&);
	
	void setDotwallRef(std::shared_ptr<class DotController> ref){ mDotWallRef = ref; };
	
private:



	void getHandInfo();
	void getGestures();
	void getTools();

	std::shared_ptr<class DotController> mDotWallRef;
};