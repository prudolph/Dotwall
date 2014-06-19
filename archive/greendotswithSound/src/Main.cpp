#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/rand.h"
#include "DotController.h"
#include "cinder/Camera.h"
using namespace ci;
using namespace ci::app;
using namespace std;

class Main : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	DotController dotWall;

	float curX, curY;
	int numdots;
	double lastUpdateTime;

	CameraPersp		mCamera;

	//Patterns
	void randomDot();

	
	void verticalWave();
	void horizontalWave();
};

void Main::setup()
{
	numdots = 10;
	dotWall.setup(Vec2f(0.0f, 0.0f), Vec2f(200.0f, 200.0f), Vec2i(numdots, numdots));
	curX = curY = 0;


	lastUpdateTime = getElapsedSeconds();
	
	
	// now tell our Camera that the window aspect ratio has changed
	mCamera.setPerspective(60, getWindowAspectRatio(), 1, 1000);

	mCamera.lookAt(Vec3f(0, 0, 0), Vec3f::zero());
	
	gl::setMatrices(mCamera);
}

void Main::mouseDown(MouseEvent event)
{
}

void Main::update()
{
	dotWall.update();
	double elapsedTime = getElapsedSeconds() - lastUpdateTime;

	if (((elapsedTime) > 1.0f / 30.0f)){


		lastUpdateTime = getElapsedSeconds();
		randomDot();
	
		/*
		randomDot();
		verticalWave();
		horizontalWave();
		verticalWave();
		*/
	}
}

void Main::randomDot(){
	dotWall.flipDot(cinder::Vec2i(randInt(0, numdots), randInt(0, numdots)));

}
void Main::verticalWave(){
	
	curX++;
	if (curX > numdots)curX = 0;
	for (int i = 0; i < numdots; i++){
		dotWall.flipDot(cinder::Vec2i(curX, i));
	}


}
void Main::horizontalWave(){
	curY++;
	if (curY > numdots)curY = 0;
	for (int i = 0; i < numdots; i++){
		dotWall.flipDot(cinder::Vec2i(i, curY));
	}
}

void Main::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

	dotWall.draw();

}

CINDER_APP_NATIVE(Main, RendererGl)
