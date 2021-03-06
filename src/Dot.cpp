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



//cinder::TriMesh Dot::mesh;
bool Dot::mUseAnimation = false;

Dot::Dot() :
mCenterPosition(Vec2f(0.0f, 0.0f)),
mRadius(10.0f),
mRadiusOffset(1.0),
mDotState(Off),
mRotation(Vec3f(0, 0, 0)),

//mOnColor(ColorA(134.0f / 255.0f, 226.0f / 255.0f, 5.0f / 255.0f, 1.0f)),
//mOffColor(ColorA(58.0f / 255.0f, 61.0f / 255.0f, 54.0f / 255.0f, 1.0f))

mOnColor(ColorA::white()),
mOffColor(ColorA::black())


{

}
Dot::~Dot(){
}
void Dot:: setup(cinder::Vec2f &cntrPos, float &radius){
	mCenterPosition = cntrPos;
	mRadius			= radius;


//	if (mesh.getNumVertices()==0){
		float num_segments = 10;
		mesh.appendVertex(Vec3f(0, 0, 0));
		mesh.appendVertex(Vec3f(1, 0, 0));
		int i;
		for (i = 1; i <= num_segments + 1; i++){
			double angle = 2 * M_PI * i / num_segments;
			double x = cos(angle)*mRadius;
			double y = sin(angle)*mRadius;
			mesh.appendVertex(Vec3f(x, y, 0));
			mesh.appendTriangle(0, i, i - 1);
		}
		mesh.appendTriangle(0, 2, i - 1);
//	}



}

void Dot::setDotState(DotState state){
	if (mDotState != state)flipDot();
	
}

void Dot::flipDot(){ 
	if (mDotState == On){
		mDotState = Off;
		if (mUseAnimation){
			timeline().apply(&mRotation, Vec3f(0, 0, 0), 0.25, EaseOutBounce());
		}
		else{
			mRotation = Vec3f(0, 0, 0);
		}
	}
	else{
		mDotState = On;
		if (mUseAnimation){
			timeline().apply(&mRotation, Vec3f(0, 180, 0), 0.25, EaseOutBounce());
		}
		else{
			mRotation = Vec3f(0, 180, 0);
		}
	}
}

void Dot::draw(){
	gl::pushModelView();
	gl::translate(mCenterPosition);
		gl::rotate(mRotation);

		glMaterialfv(GL_FRONT, GL_AMBIENT, mOnColor);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mOnColor);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mOnColor);
		glMateriali(GL_FRONT, GL_SHININESS, 0);

		glMaterialfv(GL_BACK, GL_AMBIENT, mOffColor);
		glMaterialfv(GL_BACK, GL_DIFFUSE, mOffColor);
		glMaterialfv(GL_BACK, GL_SPECULAR, mOffColor);
		glMateriali(GL_BACK, GL_SHININESS, 0);

	
			gl::draw(mesh);


		
	gl::popModelView();

	
}