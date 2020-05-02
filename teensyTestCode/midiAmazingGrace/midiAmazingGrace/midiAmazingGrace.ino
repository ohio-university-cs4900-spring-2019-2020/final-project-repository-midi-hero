/***Plays Amazing Grace over MIDI USB***/
/***With some intentionally incorrect notes for game testing purposes***/
/***Proper note timing is not yet implemented***/

int aGNoteList[35] =
  {48, 53, 57, 53, 57, 55, 53, 50, 48, 48,        // Line one
    53, 57, 53, 57, 55, 60, 57,             // Line two
    60, 57, 60, 57, 53, 48, 50, 53, 53, 50, 48, 48,   // Line three
    53, 57, 53, 57, 55, 53};                // Line four
    // 60 = C4
int aGNoteDuration[35] = 
  {4, 8, 2, 2, 8, 4, 8, 4, 8, 4, 8, 2, 2, 8, 4, 20, 4, 6, /*8th rest*/2, 2, 2, 8, 4, 6, 2, 2, 2, 8, 4, 8, 2, 2, 8, 4, 24}; 
int i=0;
const int channel = 1;
int l=35;
double bpm = 50;
double sixteenthDuration = ((60/bpm)/16)*1000;


void setup() {
  // Allow time for the Game to launch
  Serial.begin(9600);
  delay(8000); 
  Serial.println(sixteenthDuration);
}

void loop() {
 usbMIDI.sendNoteOn(aGNoteList[i], 99, channel);
 int currentNoteDuration = aGNoteDuration[i] * 2;
 while ( currentNoteDuration > 0) {
   delay(sixteenthDuration);
   currentNoteDuration--;
 }
 usbMIDI.sendNoteOff(aGNoteList[i], 0, channel);

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
