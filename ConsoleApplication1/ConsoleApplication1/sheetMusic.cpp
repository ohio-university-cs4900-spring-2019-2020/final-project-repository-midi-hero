#include "pch.h"
#include "sheetMusic.h"
#include <string>
#include <iostream>

using namespace std;
sheetMusic::sheetMusic(int song[], int l)
{
	this->length = l;
	this->currentNote = 0;
	for (int i = 0; i < length; i++) {
		this->notes[i] = song[i];
	}
}


sheetMusic::~sheetMusic()
{
}


bool sheetMusic::checkNote(int note)
{
	if (note == notes[currentNote]) {
		currentNote++;
		cout << "\n That was the RIGHT note! Good job!     :) \n";
		return true;
	} 
	else {
		cout << "\n That was the WRONG note!               :( \n";
		return false;
	}
}


bool sheetMusic::checkEndOfSong()
{
	if (currentNote + 1 == length) {
		cout << "\n SONG COMPLETED! \n";
		this->reset();
		return true;
	}
	else {
		return false;
	}
}


void sheetMusic::reset()
{
	currentNote = 0;
}