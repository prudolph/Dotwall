//
//  Dot.h
//  Dot
//
//  Created by Paul Rudolph on 7/2/13.
//
//

#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/Timeline.h"
#include "cinder/gl/Vbo.h"



class Dot{

public:
	
	enum DotState{ On, Off };

	Dot();
	~Dot();
	void setup(cinder::Vec2f &cntrPos, float &radius);
	void setDotState(DotState state);
	void flipDot();

	void draw();
	cinder::Vec2f getDotPos(){ return mCenterPosition; };

protected:
	cinder::Vec2f mCenterPosition;
	cinder::Anim<cinder::Vec3f> mRotation;

	float		  mRadius;
	cinder::Anim<float> mRadiusOffset;
	DotState	  mDotState;
	
	cinder::ColorA mOnColor, mOffColor;
	cinder::TriMesh mesh;

	

};