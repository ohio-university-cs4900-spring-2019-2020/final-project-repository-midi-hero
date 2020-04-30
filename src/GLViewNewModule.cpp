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
	int messageType = noteOff;
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
			// Indicate Correct Note
			// Display next fingering

		}
		else {
			// Indicate incorrect note
		}
		// Start note sound
		char noteSoundPath[] = "../mm/sounds/note00.wav";
		int noteTens = notePlayed / 10;
		char noteDigit = '0' + noteTens;
		noteSoundPath[17] = noteDigit;
		int noteOnes = notePlayed % 10;
		noteSoundPath[18] = '0' + noteOnes;
		char* noteSoundPathIrrklang = noteSoundPath;
		irrklang::ISound* noteSound = soundEngine->play2D(noteSoundPath, false);
		//xwingSound->setVolume(1);
	}
	else if (noteOffRecieved) {
		noteOffRecieved = false;
		if (currentSong->checkEndOfSong()) {
			// Display song completed indicator
		}
		// Update measure indicator
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
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_water+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_dust+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_mountains+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_winter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/early_morning+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_afternoon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_cloudy3+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_day2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_deepsun+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_evening+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_morning2+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_noon+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/sky_warp+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_Hubble_Nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_gray_matter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_easter+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_hot_nebula+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_ice_field+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_lemon_lime+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_milk_chocolate+6.jpg" );
   skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_solar_bloom+6.jpg" );
   //skyBoxImageNames.push_back( ManagerEnvironmentConfiguration::getSMM() + "/images/skyboxes/space_thick_rb+6.jpg" );

   float ga = 0.1f; //Global Ambient Light level for this module
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
 
   ////Create the infinite grass plane (the floor)
   /*
   wo = WO::New( grass, Vector( 1, 1, 1 ), MESH_SHADING_TYPE::mstFLAT );
   wo->setPosition( Vector( 0, 0, 0 ) );
   wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   ModelMeshSkin& grassSkin = wo->getModel()->getModelDataShared()->getModelMeshes().at( 0 )->getSkins().at( 0 );
   grassSkin.getMultiTextureSet().at( 0 )->setTextureRepeats( 5.0f );
   grassSkin.setAmbient( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Color of object when it is not in any light
   grassSkin.setDiffuse( aftrColor4f( 1.0f, 1.0f, 1.0f, 1.0f ) ); //Diffuse color components (ie, matte shading color of this object)
   grassSkin.setSpecular( aftrColor4f( 0.4f, 0.4f, 0.4f, 1.0f ) ); //Specular color component (ie, how "shiney" it is)
   grassSkin.setSpecularCoefficient( 10 ); // How "sharp" are the specular highlights (bigger is sharper, 1000 is very sharp, 10 is very dull)
   wo->setLabel( "Grass" );
   worldLst->push_back( wo );
   */

   ////Create the infinite grass plane that uses the Open Dynamics Engine (ODE)
   //wo = WOStatic::New( grass, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT );
   //((WOStatic*)wo)->setODEPrimType( ODE_PRIM_TYPE::PLANE );
   //wo->setPosition( Vector(0,0,0) );
   //wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0).getMultiTextureSet().at(0)->setTextureRepeats( 5.0f );
   //wo->setLabel( "Grass" );
   //worldLst->push_back( wo );

   ////Create the infinite grass plane that uses NVIDIAPhysX(the floor)
   //wo = WONVStaticPlane::New( grass, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT );
   //wo->setPosition( Vector(0,0,0) );
   //wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //wo->getModel()->getModelDataShared()->getModelMeshes().at(0)->getSkins().at(0).getMultiTextureSet().at(0)->setTextureRepeats( 5.0f );
   //wo->setLabel( "Grass" );
   //worldLst->push_back( wo );

   ////Create the infinite grass plane (the floor)
   //wo = WONVPhysX::New( shinyRedPlasticCube, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT );
   //wo->setPosition( Vector(0,0,50.0f) );
   //wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //wo->setLabel( "Grass" );
   //worldLst->push_back( wo );

   //wo = WONVPhysX::New( shinyRedPlasticCube, Vector(1,1,1), MESH_SHADING_TYPE::mstFLAT );
   //wo->setPosition( Vector(0,0.5f,75.0f) );
   //wo->renderOrderType = RENDER_ORDER_TYPE::roOPAQUE;
   //wo->setLabel( "Grass" );
   //worldLst->push_back( wo );

   //wo = WONVDynSphere::New( ManagerEnvironmentConfiguration::getVariableValue("sharedmultimediapath") + "/models/sphereRp5.wrl", Vector(1.0f, 1.0f, 1.0f), mstSMOOTH );
   //wo->setPosition( 0,0,100.0f );
   //wo->setLabel( "Sphere" );
   //this->worldLst->push_back( wo );

   //wo = WOHumanCal3DPaladin::New( Vector( .5, 1, 1 ), 100 );
   //((WOHumanCal3DPaladin*)wo)->rayIsDrawn = false; //hide the "leg ray"
   //((WOHumanCal3DPaladin*)wo)->isVisible = false; //hide the Bounding Shell
   //wo->setPosition( Vector(20,20,20) );
   //wo->setLabel( "Paladin" );
   //worldLst->push_back( wo );
   //actorLst->push_back( wo );
   //netLst->push_back( wo );
   //this->setActor( wo );
   //
   //wo = WOHumanCyborg::New( Vector( .5, 1.25, 1 ), 100 );
   //wo->setPosition( Vector(20,10,20) );
   //wo->isVisible = false; //hide the WOHuman's bounding box
   //((WOHuman*)wo)->rayIsDrawn = false; //show the 'leg' ray
   //wo->setLabel( "Human Cyborg" );
   //worldLst->push_back( wo );
   //actorLst->push_back( wo ); //Push the WOHuman as an actor
   //netLst->push_back( wo );
   //this->setActor( wo ); //Start module where human is the actor

   ////Create and insert the WOWheeledVehicle
   //std::vector< std::string > wheels;
   //std::string wheelStr( "../../../shared/mm/models/WOCar1970sBeaterTire.wrl" );
   //wheels.push_back( wheelStr );
   //wheels.push_back( wheelStr );
   //wheels.push_back( wheelStr );
   //wheels.push_back( wheelStr );
   //wo = WOCar1970sBeater::New( "../../../shared/mm/models/WOCar1970sBeater.wrl", wheels );
   //wo->setPosition( Vector( 5, -15, 20 ) );
   //wo->setLabel( "Car 1970s Beater" );
   //((WOODE*)wo)->mass = 200;
   //worldLst->push_back( wo );
   //actorLst->push_back( wo );
   //this->setActor( wo );
   //netLst->push_back( wo );
   //wo->rotate(z,radians

   /*on update set poition of camera or object to the other  with offset
	   vector of offset transformed through the display matrix (4 by 4)
	   look direction of camera set 
	   set position and get for wo
	   aftr utilities has matrix functions*/
	   
   //Create the sound engine
   soundEngine = irrklang::createIrrKlangDevice();

   loadAmazingGrace(); //default sheet music



   RtMidiIn *midiin = 0;

   try {

	   // RtMidiIn constructor
	   midiin = new RtMidiIn();

	   // Call function to select port.
	   if (chooseMidiPort(midiin) == false) {
		   std::cerr << "\n NO MIDI PORT SELECTED\n";
	   }

	   // Set our callback function.  This should be done immediately after
	   // opening the port to avoid having incoming messages written to the
	   // queue instead of sent to the callback function.
	   midiin->setCallback(&mycallback);

	   // Don't ignore sysex, timing, or active sensing messages.
	   midiin->ignoreTypes(false, false, false);
	   std::cout << "\nReading MIDI input.\n";
   }
   catch (RtMidiError &error) {
	   error.printMessage();
   }





   /*
   //create the star destroyer
   std::string starDestroyer(ManagerEnvironmentConfiguration::getLMM() + "/models/starDestroyer.stl");
   enemyShip = WO::New(starDestroyer, Vector(4, 4, 4), MESH_SHADING_TYPE::mstFLAT);
   enemyShip->setPosition(Vector(0, 0, 0));
   enemyShip->setLabel("Enemy Ship");
   Aftr::Vector starDestroyerDirection = (1.5, 1.5, 1.5);
   enemyShip->getModel()->setLookDirection(starDestroyerDirection);
  //enemyShip->getModel()->setLookDirection(Aftr::Vector(0, 0, 0));
   worldLst->push_back(enemyShip);

   std::string ywingModel(ManagerEnvironmentConfiguration::getLMM() + "/models/Ywing.stl");
   ywing = WO::New(ywingModel, Vector(0.1, 0.1, 0.1));
   ywingPosition = (50.0, 50.0, 0.0);
   ywing->setPosition(ywingPosition);
   ManagerGLView::getGLView()->getWorldContainer()->push_back(ywing);

   //Create Xwing
   xwing = (ManagerEnvironmentConfiguration::getLMM() + "/models/Xwing.stl");
   playerShip = WO::New(xwing, Vector(.1, .1, .1), MESH_SHADING_TYPE::mstFLAT);
   playerShip->setPosition(Vector(-100, -100, -100));
   playerShip->setLabel("Player Ship");
   worldLst->push_back(playerShip);
   */


   //fonts 
   /*
   WOGUILabel* label = WOGUILabel::New(nullptr);
   label->setText("A long long time ago...");
   label->setColor(255, 0, 0, 255);
   label->setFontSize(30); //font size is correlated with world size
   label->setPosition(Vector(0, 1, 0));
   label->setFontOrientation(FONT_ORIENTATION::foLEFT_TOP);
   label->setFontPath(comicSans);
   worldLst->push_back(label);
   */

   //directory begins in \modules\NewModule\cwin64
   //C:\Users\mattr\Documents\College Docs\Game Engine Design\modules\NewModule\cwin64
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
	
	delete this->currentSong;
	int aGNoteList[34] =
	  { 48, 53, 57, 53, 57, 55, 53, 50, 48, 48,				// Line one
		53, 57, 53, 57, 55, 60, 57,							// Line two
		60, 57, 60, 57, 53, 48, 50, 53, 53, 50, 48, 48,		// Line three
		53, 57, 53, 57, 55 };								// Line four
	currentSong = new sheetMusic(aGNoteList, 34);
	//set look direction

}


void GLViewNewModule::loadMemeMashup()
{
	delete currentSong;
	int mMNoteList[217] =
	{ 55, 62, 59, 59, 57, 55, 55, 60, 59, 59, 57, 57, 55, 55, 62, 59, 59, 57, 57, 55, 55, 52, 55, 55, 62, 59, 59, 57, 57, 55, 55, 60,
	59, 59, 57, 57, 55, 55, 62, 59, 59, 57, 55, 55, 57, 52,
		//never gonna
		 50, 52, 55, 52, 59, 59, 57, 50, 52, 55, 52, 57, 57, 55, 50, 52, 55, 52,
		55, 57, 54, 52, 50, 50, 57, 55, 50, 52, 55, 52, 59, 59, 57, 50, 52, 55, 52, 62, 54, 55, 54, 52, 50, 52, 55, 52, 55, 57, 54, 52, 50, 50, 57, 55,
		// Epic sax guy
		62, 62, 62, 62, 60, 62, 62, 62, 62, 62, 60, 62, 62, 65, 62, 60, 60, 58, 55, 55, 57, 58, 55,
		// Sandstorm
		59, 59, 59, 59, 59, 59, 59,
		59, 59, 59, 59, 59, 64, 64, 64, 64, 64, 64, 64, 62, 62, 62, 62, 62, 62, 62, 57, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 64, 64, 59, 59, 59, 59, 59, 59, 59,
		59, 59, 59, 59, 59, 64, 64, 64, 64, 64, 64, 64, 62, 62, 62, 62, 62, 62, 62, 57, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 64, 64, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59 };
	currentSong = new sheetMusic(mMNoteList, 217);
	//set look direction

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
	std::cout << "\nWould you like to open a virtual input port? [y/N] ";

	std::string keyHit;
	std::getline(std::cin, keyHit);
	if (keyHit == "y") {
		rtmidi->openVirtualPort();
		return true;
	}

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
		std::getline(std::cin, keyHit);  // used to clear out stdin
	}

	rtmidi->openPort(i);

	return true;
}