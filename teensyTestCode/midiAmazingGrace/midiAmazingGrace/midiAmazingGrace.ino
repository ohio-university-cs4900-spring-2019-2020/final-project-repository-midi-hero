/***Plays Amazing Grace over MIDI USB***/
/***With some intentionally incorrect notes for game testing purposes***/
/***Proper note timing is not yet implemented***/

int aGNoteList[34] =
  {48, 53, 57, 53, 57, 55, 53, 50, 48, 48,        // Line one
    53, 57, 53, 57, 55, 60, 57,             // Line two
    60, 57, 60, 57, 53, 48, 50, 53, 53, 50, 48, 48,   // Line three
    53, 57, 53, 57, 55};                // Line four
    // 60 = C4
int i=0;
const int channel = 1;
int l=34;

void setup() {
  // Allow time for the Game to launch
  delay(6000); 
}

void loop() {
 
 usbMIDI.sendNoteOn(aGNoteList[i], 99, channel);
 delay(300);
 usbMIDI.sendNoteOff(aGNoteList[i], 0, channel);

 // Every 4 notes, send an incorrect note to test the games response
 if(i%4 == 0) { 
   usbMIDI.sendNoteOn(70, 99, channel);  
   delay(300);
   usbMIDI.sendNoteOff(70, 0, channel);  // 60 = C4
 }

 i++;
 if(i == l){
  i = 0;
 }


  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  while (usbMIDI.read()) {
    // ignore incoming messages
  }
}
