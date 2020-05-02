/***Plays Amazing Grace and Meme Mashup over MIDI USB***/
/***With the option to play incorrect notes for game testing purposes***/

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
  
int mMNoteDuration[240] = 
  { 4, 2, 2, 4, 2, 2, 2, 4, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 2, 13, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
    4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 4, 2, 2, 4, 9, 
    
    1, 1, 1, 1, 3, 3, 6, 1, 1, 1, 1, 3, 3, 6, 1, 1, 1, 1, 
    4, 2, 3, 1, 4, 2, 4, 8, 1, 1, 1, 1, 3, 3, 6, 1, 1, 1, 1, 4, 2, 3, 1, 2, 1, 1, 1, 1, 4, 2, 3, 1, 4, 2, 4, 12,
    
    8, 2, 1, 1, 1, 2, 8, 2, 1, 1, 1, 2, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    8, 2, 1, 1, 1, 2, 8, 2, 1, 1, 1, 2, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2,

    2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 4, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 4, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 4, 2, 2, 
    2, 2, 2, 2, 4
    };
int aGNoteList[35] =
  {48, 53, 57, 53, 57, 55, 53, 50, 48, 48,        // Line one
    53, 57, 53, 57, 55, 60, 57,             // Line two
    60, 57, 60, 57, 53, 48, 50, 53, 53, 50, 48, 48,   // Line three
    53, 57, 53, 57, 55, 53};                // Line four
    // 60 = C4
int aGNoteDuration[35] = 
  {4, 8, 2, 2, 8, 4, 8, 4, 8, 4, 8, 2, 2, 8, 4, 20, 4, 6, /*8th rest*/2, 2, 2, 8, 4, 6, 2, 2, 2, 8, 4, 8, 2, 2, 8, 4, 24}; 
int aGL = 35;

int i = 0;
int j = 0;
const int channel = 1;
int mML =240;
double bpm = 50;
double sixteenthDuration = ((60/bpm)/16)*1000;
bool pinWasPrev = 1;


void setup() {
  // Allow time for the Game to launch
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  delay(10000); 
}

void loop() {
  while(digitalRead(0)) {
    //Play Amazing Grace
    if(!pinWasPrev) {
      i = 0;
    }
    pinWasPrev = true;
    if(!digitalRead(1)) {
      //Play incorrect note 
      usbMIDI.sendNoteOn(81, 99, channel);
      delay(sixteenthDuration*4);
      usbMIDI.sendNoteOff(81, 0, channel);
    } else {
       //Play Amazing Grace
       usbMIDI.sendNoteOn(aGNoteList[i], 99, channel);
       int currentNoteDuration = aGNoteDuration[i] * 2;
       while ( currentNoteDuration > 0) {
         delay(sixteenthDuration);
         currentNoteDuration--;
       }
       usbMIDI.sendNoteOff(aGNoteList[i], 0, channel);
       i++;
       if(i == aGL){
         delay(10000);
         i = 0;
       }
     }
  }
  while(!digitalRead(0)) {
    //Play Meme Mashup
    if(pinWasPrev) {
      j = 0;
    }
    pinWasPrev = false;
    if(!digitalRead(1)) {
      //Play incorrect note 
      usbMIDI.sendNoteOn(81, 99, channel);
      delay(sixteenthDuration*4);
      usbMIDI.sendNoteOff(81, 0, channel);
    } else {
      //Play next correct note
       usbMIDI.sendNoteOn(mMNoteList[j], 99, channel);
       int currentNoteDuration = mMNoteDuration[j];
       while ( currentNoteDuration > 0) {
         delay(sixteenthDuration);
         currentNoteDuration--;
       }
       usbMIDI.sendNoteOff(mMNoteList[j], 0, channel);
       j++;
       if(j == mML){
        delay(10000);
        i = 0;
       }
    }
  }

}
