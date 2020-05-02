#pragma once

#include "GLView.h"
#include "irrKlang.h"
#include "RtMidi.h"
#include "sheetMusic.h"
#include "WOQuad.h"


namespace Aftr
{
   class Camera;
   class NetMessengerClient;

/**
   \class GLViewNewModule
   \author Scott Nykl 
   \brief A child of an abstract GLView. This class is the top-most manager of the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewNewModule : public GLView
{
public:
   static GLViewNewModule* New( const std::vector< std::string >& outArgs );
   virtual ~GLViewNewModule();
   virtual void updateWorld(); ///< Called once per frame
   virtual void loadMap(); ///< Called once at startup to build this module's scene
   virtual void createNewModuleWayPoints();
   virtual void onResizeWindow( GLsizei width, GLsizei height );
   virtual void onMouseDown( const SDL_MouseButtonEvent& e );
   virtual void onMouseUp( const SDL_MouseButtonEvent& e );
   virtual void onMouseMove( const SDL_MouseMotionEvent& e );
   virtual void onKeyDown( const SDL_KeyboardEvent& key );
   virtual void onKeyUp( const SDL_KeyboardEvent& key );
    
   irrklang::ISoundEngine* soundEngine = NULL;
   irrklang::ISound* starDestroyerSound = NULL;
   WO* playerShip = NULL;
   WO* playerTwoShip = NULL;
   WO* enemyShip = NULL;
   WO* ywing = NULL;
   float starDestroyerDistance = 0;
   float starDestroyerDirectionX = 0;
   float starDestroyerDirectionY = 0;
   float starDestroyerDirectionZ = 0;
   Vector xwingPosition;
   Vector ywingPosition;
   bool playerTwo = false;
   std::string xwing;
   //NetMessengerClient* client = NetMessengerClient::New();


   //MIDI Hero Related
   virtual void loadAmazingGrace();
   virtual void loadMemeMashup();
   virtual void usage();
   virtual bool chooseMidiPort(RtMidiIn *rtmidi);
   virtual void updateMeasureTracker();

   #define noteOn 144
   #define noteOff 128

   int notePlayed = 0; //range of 0 to 127
   bool noteOnRecieved = 0;
   bool noteOffRecieved = 0;
   bool correctNote = 0;
   sheetMusic* currentSong = NULL;

   WOQuad* aGMeasureTrackerBlue = NULL;
   WOQuad* aGMeasureTrackerRed = NULL;
   WOQuad* mMMeasureTrackerBlue = NULL;
   WOQuad* mMMeasureTrackerRed = NULL;

   irrklang::ISound* noteSound = NULL;

protected:
   GLViewNewModule( const std::vector< std::string >& args );
   virtual void onCreate();   
   NetMessengerClient* client;

};

/** \} */

} //namespace Aftr
