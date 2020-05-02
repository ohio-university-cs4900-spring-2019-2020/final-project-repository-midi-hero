#ifndef sheetMusic_h
#define sheetMusic_h
#include "sheetMusic.h"
#include <string>
#include <iostream>

using namespace std;
sheetMusic::sheetMusic(int song[], int l)
{
	length = l;
	currentNote = 0;
	currentMeasure = 0;
	for (int i = 0; i < length; i++) {
		notes[i] = song[i];
	}
}


sheetMusic::~sheetMusic()
{
}


bool sheetMusic::checkNote(int note)
{
	if (note == notes[currentNote]) {
		currentNote++;
		currentMeasure = measures[currentNote];
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
	if (currentNote  == length) {
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
	currentMeasure = 0;
}


void sheetMusic::setMeasures(int measureData[], int l) 
{
	for (int i = 0; i < l; i++) {
		this->measures[i] = measureData[i];
	}
}

int sheetMusic::getCurrentMeasure() {
	return measures[currentNote];
}
float sheetMusic::getMeasureTrackerX() {
	return measureTrackerX[currentMeasure];
}
float sheetMusic::getMeasureTrackerY() {
	return measureTrackerY[currentMeasure];
}
void sheetMusic::setmeasureTrackerX(int xPos[], int l)
{
	for (int i = 0; i < l; i++) {
		measureTrackerX[i] = xPos[i];
	}
}
void sheetMusic::setmeasureTrackerY(int yPos[], int l)
{
	for (int i = 0; i < l; i++) {
		measureTrackerY[i] = yPos[i];
	}
}
#endif