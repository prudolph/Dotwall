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

using namespace ci;
using namespace ci::app;
using namespace std;

class Main : public AppNative {
  public:
	void setup();
	void mouseDrag( MouseEvent event );	
	void keyDown(KeyEvent event);
	void keyUp(KeyEvent event);
	void update();
	void draw();
	DotController dotWall;

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
	Vec3f					mLightDirection;
	ColorA					mLightColor;
	params::InterfaceGlRef	mParams;
	//Patterns
	void randomDot();

	
	void verticalWave();
	void horizontalWave();

	void loadImage();
};

void Main::setup()
{
	cPressed = false;


	//SetCamera
	mCamPos = Vec3f(500, 500,800);
	mTarget = Vec3f(500, 500, 0);
	mUp = -Vec3f::xAxis();
	mLightDirection = Vec3f(0, 0, 500);
	mLightColor =ColorA(1, 1, 1, 1);


	// now tell our Camera that the window aspect ratio has changed

	//mCamera.setPerspective(60.0f, getWindowAspectRatio(), 5.0f, 3000.0f);



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
	mParams->addParam("Light Direction", &mLightDirection, "");
	mParams->addParam("LightColor", &mLightColor, "min=0.0 max=1000.0 step=5.0 keyIncr=z keyDecr=Z");


	 mDotRadius = 7.0f;
	 mDotSpacing = 2.0f;
	 dotWall.setup(Vec2f(100.0f, 100.0f), Vec2f(500, 500), mDotRadius, mDotSpacing);


	curX = curY = 0;


	lastUpdateTime = getElapsedSeconds();
	

}

void Main::mouseDrag(MouseEvent event)
{
	if (cPressed){
		mMayaCam.mouseDrag(event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown());

	}
	else{
		float x = (float)(event.getPos().x) / (float)(getWindowWidth())*dotWall.getNumCols();
		float y = (float)(event.getPos().y) / (float)(getWindowHeight())*dotWall.getNumRows();
		Vec2f mousePos = Vec2f(x, y);
		console() << "MousePos : " << event.getPos() << " CorrectedPosition : " << mousePos << endl;
		dotWall.flipDot(mousePos);
	}
}

void Main::keyDown(KeyEvent event){
	if (event.getChar() == 'i'){
		loadImage();
	}
	if (event.getChar() == 'c')cPressed = true;
	
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
			dotWall.setDotState(cinder::Vec2i(i, j), sampleAverage>0.5f ? Dot::DotState::On : Dot::DotState::Off);

		}
	}


}

void Main::update()
{


	
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
	dotWall.flipDot(cinder::Vec2i(randInt(0, dotWall.getNumCols()), randInt(0, dotWall.getNumRows())));

}
void Main::verticalWave(){
	
	curX++;
	if (curX > dotWall.getNumCols())curX = 0;
	for (int i = 0; i < dotWall.getNumCols(); i++){
		dotWall.flipDot(cinder::Vec2i(curX, i));
	}


}
void Main::horizontalWave(){
	curY++;
	if (curY > dotWall.getNumRows())curY = 0;
	for (int i = 0; i < dotWall.getNumRows(); i++){
		dotWall.flipDot(cinder::Vec2i(i, curY));
	}
}

void Main::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );


	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightPosition[] = { -mLightDirection.x, -mLightDirection.y, -mLightDirection.z, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mLightColor);
	
	gl::setMatrices(mMayaCam.getCamera());
		gl::color(1, 1, 1);
		if (testImage)gl::draw(testImage,Vec2f(100,100));
		dotWall.draw();

		mParams->draw();
}

CINDER_APP_NATIVE(Main, RendererGl)
