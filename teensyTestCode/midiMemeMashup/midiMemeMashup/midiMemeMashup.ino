/***Plays Amazing Grace over MIDI USB***/
/***With some intentionally incorrect notes for game testing purposes***/
/***Proper note timing is not yet implemented***/
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
int i=0;
const int channel = 1;
int l=240;
double bpm = 50;
double sixteenthDuration = ((60/bpm)/16)*1000;


void setup() {
  // Allow time for the Game to launch
  Serial.begin(9600);
  delay(10000); 
  Serial.println(sixteenthDuration);
}

void loop() {
 usbMIDI.sendNoteOn(mMNoteList[i], 99, channel);
 int currentNoteDuration = mMNoteDuration[i] *2;
 while ( currentNoteDuration > 0) {
   delay(sixteenthDuration);
   currentNoteDuration--;
 }
 usbMIDI.sendNoteOff(mMNoteList[i], 0, channel);

 // Every 4 notes, send an incorrect note to test the games response
 /*
 if(i%4 == 0) { 
   usbMIDI.sendNoteOn(70, 99, channel);  
   delay(300);
   usbMIDI.sendNoteOff(70, 0, channel);  // 60 = C4
 }
*/
 i++;
 if(i == l){
  delay(10000);
  i = 0;
 }

  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}
