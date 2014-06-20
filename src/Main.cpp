#include "cinder/app/AppNative.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"


#include "cinder/gl/gl.h"
#include "cinder/rand.h"
#include "DotController.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "cinder/params/Params.h"
//Leap

#include "Leap.h"
#include "LeapMath.h"
#include "LeapListener.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Main : public AppNative {
public:	
	void prepareSettings(Settings *settings);
	void setup();
	void mouseDown(MouseEvent event);
	void mouseDrag( MouseEvent event );	
	void keyDown(KeyEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();

	//Patterns
	void randomDot();
	void verticalWave();
	void horizontalWave();
	void loadImage();


	std::shared_ptr<DotController> dotWallRef;

	float curX, curY;
	float mDotRadius, mDotSpacing;
	
	double lastUpdateTime;

	Channel32f mChannel;
	gl::Texture testImage;
	//CameraPersp		mCamera;
	// our camera
	MayaCamUI	mMayaCam;
	bool cPressed;
	Vec3f mCamPos, mTarget,mUp;
	Vec3f					mLightPosition;
	ColorA					mLightColor;
	params::InterfaceGlRef	mParams;


	Leap::Controller leapController;
	LeapListener leapListener;

};

void Main::prepareSettings(Settings *settings){
		settings->setFrameRate(60);
		settings->setFullScreen();
}
void Main::setup()
{
	cPressed = false;


	//SetCamera
	mCamPos = Vec3f(0, 0,-500);
	mTarget = Vec3f(0, 0, 0);
	mUp = -Vec3f::yAxis();

	mLightPosition = Vec3f(0, 0, 500);
	mLightColor =ColorA(1, 0, 0, 1);



	// set up the camera
	CameraPersp cam;
	cam.setEyePoint(mCamPos);
	cam.setCenterOfInterestPoint(mTarget);
	cam.setWorldUp(mUp);
	cam.setPerspective(60.0f, getWindowAspectRatio(), 1.0f, 3000.0f);
	mMayaCam.setCurrentCam(cam);




	// Setup the parameters
	mParams = params::InterfaceGl::create(getWindow(), "DotWall parameters", toPixels(Vec2i(200, 400)));
	
	mParams->addParam("Cam Pos", &mCamPos, "min=0.0 max=1000.0 step=5.0 keyIncr=z keyDecr=Z");
	mParams->addParam("Target", &mTarget, "min=0.0 max=1000.0 step=5.0 keyIncr=z keyDecr=Z");
	mParams->addParam("Light Direction", &mLightPosition, "");
	mParams->addParam("LightColor", &mLightColor, "min=0.0 max=1000.0 step=5.0 keyIncr=z keyDecr=Z");


	 mDotRadius = 7.0f;
	 mDotSpacing = 1.0f;
	 dotWallRef = std::shared_ptr<DotController>(new DotController());

	 dotWallRef->setup(Vec2f(100.0f, 100.0f), Vec2f(500, 500), mDotRadius, mDotSpacing);


	curX = curY = 0;


	lastUpdateTime = getElapsedSeconds();
	
	//Setup Leap
	leapListener.setDotwallRef(dotWallRef);
	leapController.addListener(leapListener);


}
void Main::mouseDown(MouseEvent event){
	if (cPressed){
		mMayaCam.mouseDown(event.getPos());

	}
}
void Main::mouseDrag(MouseEvent event)
{
	if (cPressed){
		mMayaCam.mouseDrag(event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown());

	}
	else{
		float x = (float)(event.getPos().x) / (float)(getWindowWidth())*dotWallRef->getNumCols();
		float y = (float)(event.getPos().y) / (float)(getWindowHeight())*dotWallRef->getNumRows();
		Vec2f mousePos = Vec2f(x, y);
		console() << "MousePos : " << event.getPos() << " CorrectedPosition : " << mousePos << endl;
		dotWallRef->flipDot(mousePos);
	}
}

void Main::keyDown(KeyEvent event){
	if (event.getChar() == 'i'){
		loadImage();
	}
	//Change the camera around
	if (event.getChar() == 'c')cPressed = true;

	if (event.getChar() == 'a')Dot::ToggleUsingAnimations();
	if (event.getChar() == 'g')DotController::toggleGravity();
	if (event.getChar() == 'q')quit();

}


void Main::keyUp(KeyEvent event){
	
	if (event.getChar() == 'c')cPressed = false;
	
}
void Main::loadImage(){

	mChannel = Channel32f(cinder::loadImage(loadAsset("Grid.png")));
	//mChannel = Channel32f(cinder::loadImage(loadAsset("cat.jpg")));

//	testImage = gl::Texture(mChannel);
	//Go through the image and get a chunk the size of a dot 

	float chunkSize = (mDotRadius*2) ;


	int numberOfHorizontalChunks = floor(mChannel.getWidth() / chunkSize);
	int numberOfVerticalChunks = floor(mChannel.getHeight() / chunkSize);

	for (int i = 0; i < numberOfHorizontalChunks; i++){
		for (int j = 0; j < numberOfVerticalChunks; j++){
			Vec2i startingSamplePnt = Vec2i(i*chunkSize, j*chunkSize);

			float sampleAccum = 0;
			for (int sampleI = 0; sampleI < chunkSize; sampleI++){
				for (int sampleJ = 0; sampleJ < chunkSize; sampleJ++){
					sampleAccum+=mChannel.getValue(startingSamplePnt + Vec2i(sampleI, sampleJ));
				}
			}

			//Average out the samples
			float sampleAverage = sampleAccum / (chunkSize*chunkSize);
			dotWallRef->setDotState(cinder::Vec2i(i, j), sampleAverage>0.5f ? Dot::DotState::On : Dot::DotState::Off);

		}
	}


}

void Main::update()
{

	console() << "CAMara :" << mMayaCam.getCamera().getWorldUp()<< endl;;
	dotWallRef->update();

	
	/*
//Move camera around the dotwall
	double angle = 2 * M_PI * getElapsedSeconds()/4.0f;
	double x = cos(angle)*200;
	double z = sin(angle)*200;
	mCamera.lookAt(Vec3f(x, 0, z), Vec3f(0,0, 0));
	*/
	
	


	//dotWall.update();
	double elapsedTime = getElapsedSeconds() - lastUpdateTime;

	if (((elapsedTime) > 1.0f / 30.0f)){


		lastUpdateTime = getElapsedSeconds();
		//for (int i = 0; i < 10; i++)
		//randomDot();
		//verticalWave();
		//horizontalWave();
		/*
		randomDot();
		verticalWave();
		horizontalWave();
		verticalWave();
		*/
	}
}

void Main::randomDot(){
	dotWallRef->flipDot(cinder::Vec2i(randInt(0, dotWallRef->getNumCols()), randInt(0, dotWallRef->getNumRows())));

}
void Main::verticalWave(){
	
	curX++;
	if (curX > dotWallRef->getNumCols())curX = 0;
	for (int i = 0; i < dotWallRef->getNumCols(); i++){
		dotWallRef->flipDot(cinder::Vec2i(curX, i));
	}


}
void Main::horizontalWave(){
	curY++;
	if (curY > dotWallRef->getNumRows())curY = 0;
	for (int i = 0; i < dotWallRef->getNumRows(); i++){
		dotWallRef->flipDot(cinder::Vec2i(i, curY));
	}
}

void Main::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );


	
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightPosition[] = { mLightPosition.x, mLightPosition.y, -mLightPosition.z, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	GLfloat ambientLight[] = { mLightColor.r, mLightColor.g, mLightColor.b, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);




	gl::setMatrices(mMayaCam.getCamera());
		gl::color(1, 1, 1);
		gl::drawColorCube(Vec3f(0, 0, 0), Vec3f(10, 10, 10));
		//if (testImage)gl::draw(testImage,Vec2f(100,100));
		dotWallRef->draw();

		mParams->draw();

		glDisable(GL_LIGHTING);
		//Draw Framerate
		gl::drawString(to_string(getAverageFps()), Vec2f(0.0f, 0.0f), ColorA(1.0f, 0.0f, 0.0f, 1.0f), Font("Arial", 20.0f));

}

CINDER_APP_NATIVE(Main, RendererGl)
