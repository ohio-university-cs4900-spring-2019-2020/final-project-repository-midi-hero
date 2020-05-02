#include "GLViewNewModule.h"

#include "WorldList.h" //This is where we place all of our WOs
#include "ManagerOpenGLState.h" //We can change OpenGL State attributes with this
#include "Axes.h" //We can set Axes to on/off with this
#include "PhysicsEngineODE.h"

//Different WO used by this module
#include "WO.h"
#include "WOStatic.h"
#include "WOStaticPlane.h"
#include "WOStaticTrimesh.h"
#include "WOTrimesh.h"
#include "WOHumanCyborg.h"
#include "WOHumanCal3DPaladin.h"
#include "WOWayPointSpherical.h"
#include "WOLight.h"
#include "WOSkyBox.h"
#include "WOCar1970sBeater.h"
#include "Camera.h"
#include "CameraStandard.h"
#include "CameraChaseActorSmooth.h"
#include "CameraChaseActorAbsNormal.h"
#include "CameraChaseActorRelNormal.h"
#include "Model.h"
#include "ModelDataShared.h"
#include "ModelMesh.h"
#include "ModelMeshDataShared.h"
#include "ModelMeshSkin.h"
#include "WONVStaticPlane.h"
#include "WONVPhysX.h"
#include "WONVDynSphere.h"
#include "AftrGLRendererBase.h"
#include "irrKlang.h"
#include "NetMsg.h"
#include "NetMsgMoveObject.h"
#include "NetMsgCreateWO.h"
#include "NetMessengerClient.h"
#include "NewModuleWayPoints.h"
#include "NetMessengerStreamBuffer.h"

#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
#include "sheetMusic.h"


class sheetMusic;
using namespace Aftr;


void mycallback(double deltatime, std::vector< unsigned char > *message, void */*userData*/)
{
	int messageType = 0;
	unsigned int nBytes = message->size();
	for (unsigned int i = 0; i < nBytes; i++) {
		std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
		if (i == 0) {
			messageType = (int)message->at(i);
		}
		if (messageType == noteOn && i == 1) {
			((GLViewNewModule*)ManagerGLView::getGLView())->notePlayed = (int)message->at(i);
			((GLViewNewModule*)ManagerGLView::getGLView())->noteOnRecieved = true;
		}
		else if (messageType == noteOff) {
			((GLViewNewModule*)ManagerGLView::getGLView())->noteOffRecieved = true;
		}
	}
}

GLViewNewModule* GLViewNewModule::New( const std::vector< std::string >& args )
{
   GLViewNewModule* glv = new GLViewNewModule( args );
   glv->init( Aftr::GRAVITY, Vector( 0, 0, -1.0f ), "aftr.conf", PHYSICS_ENGINE_TYPE::petODE );
   glv->onCreate();
   return glv;
}


GLViewNewModule::GLViewNewModule( const std::vector< std::string >& args ) : GLView( args )
{
   //Initialize any member variables that need to be used inside of LoadMap() here.
   //Note: At this point, the Managers are not yet initialized. The Engine initialization
   //occurs immediately after this method returns (see GLViewNewModule::New() for
   //reference). Then the engine invoke's GLView::loadMap() for this module.
   //After loadMap() returns, GLView::onCreate is finally invoked.

   //The order of execution of a module startup:
   //GLView::New() is invoked:
   //    calls GLView::init()
   //       calls GLView::loadMap() (as well as initializing the engine's Managers)
   //    calls GLView::onCreate()

   //GLViewNewModule::onCreate() is invoked after this module's LoadMap() is completed.
}


void GLViewNewModule::onCreate()
{
   //GLViewNewModule::onCreate() is invoked after this module's LoadMap() is completed.
   //At this point, all the managers are initialized. That is, the engine is fully initialized.

   if( this->pe != NULL )
   {
      //optionally, change gravity direction and magnitude here
      //The user could load these values from the module's aftr.conf
      this->pe->setGravityNormalizedVector( Vector( 0,0,-1.0f ) );
      this->pe->setGravityScalar( Aftr::GRAVITY );
   }
   this->setActorChaseType( STANDARDEZNAV ); //Default is STANDARDEZNAV mode
   //this->setNumPhysicsStepsPerRender( 0 ); 
   //pause physics engine on start up; will remain paused till set to 1
}


GLViewNewModule::~GLViewNewModule()
{
   //Dummy NetMsg
	/*
	NetMsgMoveObject dummyMsg;
	dummyMsg.model = ManagerEnvironmentConfiguration::getLMM() + "/models/Ywing.stl";
	dummyMsg.position = Vector(50, 50, 5);
	client->sendNetMsgSynchronousTCP(dummyMsg);

	WO* wo = WO::New(dummyMsg.model, Vector(0.1, 0.1, 0.1));
	wo->setPosition(dummyMsg.position);
	ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
	*/
	//Implicitly calls GLView::~GLView()
}


void GLViewNewModule::updateWorld()
{
   GLView::updateWorld(); //Just call the parent's update world first.
                          //If you want to add additional functionality, do it after
                          //this call.
	if (noteOnRecieved) {
		noteOnRecieved = false;
		std::cout << "\n The current note being played is: " << notePlayed << "\n";
		if (currentSong->checkNote(notePlayed)) {
			correctNote = 1;
		}
		else {
			correctNote = 0;
		}
		// Play the note sound
		char noteSoundPath[] = "../mm/sounds/note00.wav";
		int noteTens = notePlayed / 10;
		char noteDigit = '0' + noteTens;
		noteSoundPath[17] = noteDigit;
		int noteOnes = notePlayed % 10;
		noteSoundPath[18] = '0' + noteOnes;
		char* noteSoundPathIrrklang = noteSoundPath;

		noteSound = soundEngine->play2D(noteSoundPath, false);

		// Future feature: MIDI note velocity could be mapped to the note volume in the future
		//noteSound->setVolume(1);
	}
	else if (noteOffRecieved) {
		noteOffRecieved = false;
		//noteSound->stop();
		if (currentSong->checkEndOfSong()) {
			// song completed indicator
			irrklang::ISound* applause = soundEngine->play2D("../mm/sounds/applause.wav", false);
			irrklang::ISound* wellDone = soundEngine->play2D("../mm/sounds/wellDone.wav", false);

		}
		// Update measure indicator
		updateMeasureTracker();

	}
}


void GLViewNewModule::onResizeWindow( GLsizei width, GLsizei height )
{
   GLView::onResizeWindow( width, height ); //call parent's resize method.
}


void GLViewNewModule::onMouseDown( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseDown( e );
}


void GLViewNewModule::onMouseUp( const SDL_MouseButtonEvent& e )
{
   GLView::onMouseUp( e );
}


void GLViewNewModule::onMouseMove( const SDL_MouseMotionEvent& e )
{
   GLView::onMouseMove( e );
}


void GLViewNewModule::onKeyDown( const SDL_KeyboardEvent& key )
{
	static float offset = 0;
  GLView::onKeyDown( key );

  if (key.keysym.sym == SDLK_0)
  {
	  this->setNumPhysicsStepsPerRender(1);
  }

  if (key.keysym.sym == SDLK_1)
  {
	  loadAmazingGrace();
  }
  if (key.keysym.sym == SDLK_2)
  {
	  loadMemeMashup();
  }
  if (key.keysym.sym == SDLK_r)
  {
	  currentSong->reset();
  }
  if (key.keysym.sym == SDLK_c)
  {
	  this->cam = new CameraStandardEZNav(this, &this->mouseHandler);
  }
  /* //TEST CODE used only for programming the game
  if (key.keysym.sym == SDLK_w)
  {
	  Aftr::Vector mMMeasureTrackerBluePos = mMMeasureTrackerBlue->getPosition();
	  mMMeasureTrackerBluePos.y += 1;
	  mMMeasureTrackerBlue->setPosition(mMMeasureTrackerBluePos);
  }
  if (key.keysym.sym == SDLK_a)
  {
	  Aftr::Vector mMMeasureTrackerBluePos = mMMeasureTrackerBlue->getPosition();
	  mMMeasureTrackerBluePos.x -= 1;
	  std::cout << mMMeasureTrackerBluePos << "\n";
	  mMMeasureTrackerBlue->setPosition(mMMeasureTrackerBluePos);
  }
  if (key.keysym.sym == SDLK_s)
  {
	  Aftr::Vector mMMeasureTrackerBluePos = mMMeasureTrackerBlue->getPosition();
	  mMMeasureTrackerBluePos.y -= 1;
	  std::cout << mMMeasureTrackerBluePos << "\n";
	  mMMeasureTrackerBlue->setPosition(mMMeasureTrackerBluePos);
  }
  if (key.keysym.sym == SDLK_d)
  {
	  Aftr::Vector mMMeasureTrackerBluePos = mMMeasureTrackerBlue->getPosition();
	  mMMeasureTrackerBluePos.x += 1;
	  std::cout << mMMeasureTrackerBluePos << "\n";
	  mMMeasureTrackerBlue->setPosition(mMMeasureTrackerBluePos);
  }
  if (key.keysym.sym == SDLK_l) {
	  Aftr::Vector cameraDirection = this->getCamera()->getLookDirection();
	  std::cout << cameraDirection << "\n\n\n";
  }
  if (key.keysym.sym == SDLK_p) {
	  Aftr::Vector cameraPosition = this->getCamera()->getPosition();
	  std::cout << cameraPosition << "\n\n\n";
  }*/

}


void GLViewNewModule::onKeyUp( const SDL_KeyboardEvent& key )
{
   GLView::onKeyUp( key );
}


void Aftr::GLViewNewModule::loadMap()
{
   this->worldLst = new WorldList(); //WorldList is a 'smart' vector that is used to store WO*'s
   this->actorLst = new WorldList();
   this->netLst = new WorldList();

   ManagerOpenGLState::GL_CLIPPING_PLANE = 1000.0;
   ManagerOpenGLState::GL_NEAR_PLANE = 0.1f;
   ManagerOpenGLState::enableFrustumCulling = false;
   Axes::isVisible = true;
   this->glRenderer->isUsingShadowMapping( false ); //set to TRUE to enable shadow mapping, must be using GL 3.2+

   this->cam->setPosition( 0,0,0 );

   std::string shinyRedPlasticCube( ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl" );
   std::string wheeledCar( ManagerEnvironmentConfiguration::getSMM() + "/models/rcx_treads.wrl" );
   std::string grass( ManagerEnvironmentConfiguration::getSMM() + "/models/grassFloor400x400_pp.wrl" );
   std::string human( ManagerEnvironmentConfiguration::getSMM() + "/models/human_chest.wrl" );
   
   //SkyBox Textures readily available
   std::vector< std::string > skyBoxImageNames; //vector to store texture paths
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_milk_chocolate+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_solar_bloom+6.jpg" );
   skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_thick_rb+6.jpg" );

   float ga = 1.0f; //Global Ambient Light level for this module
   ManagerLight::setGlobalAmbientLight( aftrColor4f( ga, ga, ga, 1.0f ) );
   WOLight* light = WOLight::New();
   light->isDirectionalLight( true );
   light->setPosition( Vector( 0, 0, 100 ) );
   //Set the light's display matrix such that it casts light in a direction parallel to the -z axis (ie, downwards as though it was "high noon")
   //for shadow mapping to work, this->glRenderer->isUsingShadowMapping( true ), must be invoked.
   light->getModel()->setDisplayMatrix( Mat4::rotateIdentityMat( { 0, 1, 0 }, 90.0f * Aftr::DEGtoRAD ) );
   light->setLabel( "Light" );
   worldLst->push_back( light );

   //Create the SkyBox
   WO* wo = WOSkyBox::New( skyBoxImageNames.at( 0 ), this->getCameraPtrPtr() );
   wo->setPosition( Vector( 0,0,0 ) );
   wo->setLabel( "Sky Box" );
   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   worldLst->push_back( wo );
	   
   //Create the sound engine
   soundEngine = irrklang::createIrrKlangDevice();

   //loadMemeMashup(); //other song option
   loadAmazingGrace(); //default sheet music
   this->cam = new CameraStandardEZNav(this, &this->mouseHandler); //reset the camera

   //Load the images for the sheet music and measure trackers
   std::string aGImagePath = (ManagerEnvironmentConfiguration::getLMM() + "/images/amazingGrace.png");
   WOQuad* aGSheet = WOQuad::New(aGImagePath, 100, 167);
   aGSheet->setPosition(Vector(0, 0, -100));
   worldLst->push_back(aGSheet);

   std::string measureTrackerRedImagePath = (ManagerEnvironmentConfiguration::getLMM() + "/images/wrongNote.png");
   aGMeasureTrackerRed = WOQuad::New(measureTrackerRedImagePath, 7, 20);
   aGMeasureTrackerRed->setPosition(Vector(0, 0, -101));
   worldLst->push_back(aGMeasureTrackerRed);

   std::string measureTrackerBlueImagePath = (ManagerEnvironmentConfiguration::getLMM() + "/images/rightNote.png");
   aGMeasureTrackerBlue = WOQuad::New(measureTrackerBlueImagePath, 7, 20);
   aGMeasureTrackerBlue->setPosition(Vector(0, 0, -99));
   worldLst->push_back(aGMeasureTrackerBlue);

   std::string mMImagePath = (ManagerEnvironmentConfiguration::getLMM() + "/images/memeMashup.png");
   WOQuad* mMSheet = WOQuad::New(mMImagePath, 140, 100);
   mMSheet->setPosition(Vector(0, 0, 100));
   worldLst->push_back(mMSheet);

   mMMeasureTrackerRed = WOQuad::New(measureTrackerRedImagePath, 5, 13);
   mMMeasureTrackerRed->setPosition(Vector(0, 0, 99));
   worldLst->push_back(mMMeasureTrackerRed);

   mMMeasureTrackerBlue = WOQuad::New(measureTrackerBlueImagePath, 5, 13);
   mMMeasureTrackerBlue->setPosition(Vector(0, 0, 101));
   worldLst->push_back(mMMeasureTrackerBlue);

   // Setup for MIDI input

   RtMidiIn *midiin = 0;

   try {

	   // RtMidiIn constructor
	   midiin = new RtMidiIn();

	   // Call function to select port.
	   if (chooseMidiPort(midiin) == false) {
		   std::cerr << "\n NO MIDI PORT SELECTED\n";
	   }

	   // Set our callback function.
	   midiin->setCallback(&mycallback);

	   // Don't ignore sysex, timing, or active sensing messages.
	   midiin->ignoreTypes(false, false, false);
	   std::cout << "\nReading MIDI input.\n";
   }
   catch (RtMidiError &error) {
	   error.printMessage();
   }

   createNewModuleWayPoints();
}

void GLViewNewModule::createNewModuleWayPoints()
{
   // Create a waypoint with a radius of 3, a frequency of 5 seconds, activated by GLView's camera, and is visible.
   WayPointParametersBase params(this);
   params.frequency = 5000;
   params.useCamera = true;
   params.visible = true;
   WOWayPointSpherical* wayPt = WOWP1::New( params, 3 );
   wayPt->setPosition( Vector( 10, 10, 3  ) );
   worldLst->push_back( wayPt );
}


void GLViewNewModule::loadAmazingGrace()
{
	// Changing the cameras direction here breaks the camera unless it has first been moved, so the camera is reset after this is first ran
	// After moving the camera once, this works as intended
	this->getCamera()->setPosition(Aftr::Vector(0, 0, 1));
	this->getCamera()->setCameraLookAtPoint(Aftr::Vector(0, 0, -1));
	delete this->currentSong;
	int aGNoteList[35] =
	  { 48, 53, 57, 53, 57, 55, 53, 50, 48, 48,				// Line one
		53, 57, 53, 57, 55, 60, 57,							// Line two
		60, 57, 60, 57, 53, 48, 50, 53, 53, 50, 48, 48,		// Line three
		53, 57, 53, 57, 55, 53 };								// Line four
	currentSong = new sheetMusic(aGNoteList, 35);
	currentSong->songName = "Amazing Grace";
	int aGMeasureList[35] =
	{ 0, 1, 1, 1, 2, 2, 3, 3, 4, 4,
	  5, 5, 5, 6, 6, 7, 8,
	  9, 9, 9, 9, 10, 10, 11, 11, 11, 11, 12, 12,
	  13, 13, 13, 14, 14, 15 };
	currentSong->setMeasures(aGMeasureList, 35);
	int aGXCoords[16] = { -46, -19, 8, 34, 60, -42, -5, 28, 58, -42, -7, 27, 59, -43, -7, 27 };
	int aGYCoords[16] = { 9, 9, 9, 9, 9, -7, -7, -7, -7, -25, -25, -25, -25, -42, -42, -42 };
		
	currentSong->setmeasureTrackerX(aGXCoords, 15);
	currentSong->setmeasureTrackerY(aGYCoords, 15);
}


void GLViewNewModule::loadMemeMashup()
{
	this->getCamera()->setPosition(Aftr::Vector(0, 0, 250));
	this->getCamera()->setCameraLookAtPoint(Aftr::Vector(0, 0, 0));
	delete currentSong;
	int mMNoteList[240] =
	{ 55, 62, 59, 59, 57, 55, 55, 60, 59, 59, 57, 57, 55, 55, 62, 59, 59, 57, 57, 55, 55, 52, 55, 55, 62, 59, 59, 57, 57, 55, 55, 60,
	59, 59, 57, 57, 55, 55, 62, 59, 59, 57, 55, 55, 57, 52,
		//never gonna
		 50, 52, 55, 52, 59, 59, 57, 50, 52, 55, 52, 57, 57, 55, 50, 52, 55, 52,
		55, 57, 54, 52, 50, 50, 57, 55, 50, 52, 55, 52, 59, 59, 57, 50, 52, 55, 52, 62, 54, 55, 54, 52, 50, 52, 55, 52, 55, 57, 54, 52, 50, 50, 57, 55,
		// Epic sax guy
		62, 62, 62, 62, 60, 62, 62, 62, 62, 62, 60, 62, 62, 65, 62, 60, 60, 58, 55, 55, 57, 58, 55,
		62, 62, 62, 62, 60, 62, 62, 62, 62, 62, 60, 62, 62, 65, 62, 60, 60, 58, 55, 55, 57, 58, 55,
		// Sandstorm
		59, 59, 59, 59, 59, 59, 59,
		59, 59, 59, 59, 59, 64, 64, 64, 64, 64, 64, 64, 62, 62, 62, 62, 62, 62, 62, 57, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 64, 64, 59, 59, 59, 59, 59, 59, 59,
		59, 59, 59, 59, 59, 64, 64, 64, 64, 64, 64, 64, 62, 62, 62, 62, 62, 62, 62, 57, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 64, 64, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59 };
	currentSong = new sheetMusic(mMNoteList, 240);
	currentSong->songName = "Meme Mashup";
	int mMMeasureList[240] =
	{ 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
	  6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 
	  //never gonna
	  8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 
	  11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 16, 16,
	  // Epic sax guy
	  17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 
	  17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20,
	  // Sandstorm
	  21, 21, 21, 21, 21, 21, 21, 
	  22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23,   24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 
	  28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 
	  34, 34, 34, 34, 34 }; //35 measures
	currentSong->setMeasures(mMMeasureList, 240);

	int mMXCoords[35] = { -36, -22, -6, 10, 24, 37, 
		-33, -15, 0, 17, 35, 
		-35, -20, -5, 12, 28, 41, 
		-31, -15, 0, 18, 37, 
		-35, -20, -5, 9, 24, 38,
		-34, -19, -5, 9, 23, 38,
		-14	};
	int mMYCoords[35] = {
		32, 32, 32, 32, 32, 32,
		15, 15, 15, 15, 15,
		-1, -1, -1, -1, -1, -1,
		-19, -19, -19, -19, -19,
		-36, -36, -36, -36, -36, -36,
		-53, -53, -53, -53, -53, -53,
		-63 };
	currentSong->setmeasureTrackerX(mMXCoords, 35);
	currentSong->setmeasureTrackerY(mMYCoords, 35);
}


void GLViewNewModule::updateMeasureTracker() {
	if (currentSong->songName == "Amazing Grace") {
		if (correctNote) {
			aGMeasureTrackerBlue->setPosition(Vector(currentSong->getMeasureTrackerX(), currentSong->getMeasureTrackerY(), -99));
			aGMeasureTrackerRed->setPosition(Vector(currentSong->getMeasureTrackerX(), currentSong->getMeasureTrackerY(), -101));
		}
		else {
			aGMeasureTrackerBlue->setPosition(Vector(currentSong->getMeasureTrackerX(), currentSong->getMeasureTrackerY(), -101));
			aGMeasureTrackerRed->setPosition(Vector(currentSong->getMeasureTrackerX(), currentSong->getMeasureTrackerY(), -99));
		}
	}
	else if (currentSong->songName == "Meme Mashup") {
		if (correctNote) {
			mMMeasureTrackerBlue->setPosition(Vector(currentSong->getMeasureTrackerX(), currentSong->getMeasureTrackerY(), 101));
			mMMeasureTrackerRed->setPosition(Vector(currentSong->getMeasureTrackerX(), currentSong->getMeasureTrackerY(), 99));
		}
		else {
			mMMeasureTrackerBlue->setPosition(Vector(currentSong->getMeasureTrackerX(), currentSong->getMeasureTrackerY(), 99));
			mMMeasureTrackerRed->setPosition(Vector(currentSong->getMeasureTrackerX(), currentSong->getMeasureTrackerY(), 101));
		}
	}
}


void GLViewNewModule::usage(void) {
	// Error function in case of incorrect command-line
	// argument specifications.
	std::cout << "\nuseage: cmidiin <port>\n";
	std::cout << "    where port = the device to use (first / default = 0).\n\n";
	exit(0);
}



bool GLViewNewModule::chooseMidiPort(RtMidiIn *rtmidi)
{
	/*
	std::cout << "\nWould you like to open a virtual input port? [y/N] ";

	std::string keyHit;
	std::getline(std::cin, keyHit);
	if (keyHit == "y") {
		rtmidi->openVirtualPort();
		return true;
	}
	*/
	std::string portName;
	unsigned int i = 0, nPorts = rtmidi->getPortCount();
	if (nPorts == 0) {
		std::cout << "No input ports available!" << std::endl;
		return false;
	}

	if (nPorts == 1) {
		std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
	}
	else {
		for (i = 0; i < nPorts; i++) {
			portName = rtmidi->getPortName(i);
			std::cout << "  Input port #" << i << ": " << portName << '\n';
		}

		do {
			std::cout << "\nChoose a port number: ";
			std::cin >> i;
		} while (i >= nPorts);
		//std::getline(std::cin, keyHit);  // used to clear out stdin
	}

	rtmidi->openPort(i);

	return true;
}