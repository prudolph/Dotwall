//
//  Dot.cpp
//  Dot
//
//  Created by Paul Rudolph on 7/2/13.
//
//

#include "Dot.h"

using namespace std;
using namespace ci;
using namespace ci::app;

Dot::Dot() :
mCenterPosition(Vec2f(0.0f, 0.0f)),
mRadius(10.0f),
mRadiusOffset(1.0),
mDotState(Off),
mRotation(Vec3f(0, 0, 0)),
mOnColor(ColorA(134.0f / 255.0f, 226.0f / 255.0f, 5.0f / 255.0f, 1.0f)),
mOffColor(ColorA(58.0f / 255.0f, 61.0f / 255.0f, 54.0f / 255.0f, 1.0f))



{

}
Dot::~Dot(){
}
void Dot:: setup(cinder::Vec2f &cntrPos, float &radius){
	mCenterPosition = cntrPos;
	mRadius			= radius;

	float num_segments = 30;
	mesh.appendVertex(Vec3f(0, 0, 0));
	mesh.appendVertex(Vec3f(1, 0, 0));
	int i;
	for (i = 1; i <= num_segments+1; i++){
		double angle = 2 * M_PI * i / num_segments;
		double x = cos(angle)*mRadius;
		double y = sin(angle)*mRadius;
		mesh.appendVertex(Vec3f(x, y, 0));
		mesh.appendTriangle(0, i, i-1);
	}
	mesh.appendTriangle(0, 2,i-1);




}

void Dot::setDotState(DotState state){
	if (mDotState != state)flipDot();
	
}

void Dot::flipDot(){ 
	if (mDotState == On){
		mDotState = Off;

		timeline().apply(&mRotation, Vec3f(0, 0, 0), 0.50, EaseOutBounce());
	}
	else{
		mDotState = On;
		timeline().apply(&mRotation, Vec3f(0, 180, 0), 0.50, EaseOutBounce());
	}
}

void Dot::draw(){


	gl::pushModelView();
	gl::translate(mCenterPosition);
		gl::rotate(mRotation);
		//glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			gl::color(mOnColor);
			gl::draw(mesh);

			glCullFace(GL_BACK);
			gl::color(mOffColor);
			gl::draw(mesh);

		glDisable(GL_CULL_FACE);
		
	gl::popModelView();

	
}