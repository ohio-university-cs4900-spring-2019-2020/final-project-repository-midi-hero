
#pragma once
#include <string>

using namespace std;

class sheetMusic
{
public:
	sheetMusic(int song[], int l);
	bool checkNote(int note);
	bool checkEndOfSong();
	void reset();
	void setMeasures(int song[], int l);
	int getCurrentMeasure();
	float getMeasureTrackerX();
	float getMeasureTrackerY();
	void setmeasureTrackerX(int xPos[], int l);
	void setmeasureTrackerY(int yPos[], int l);

	int length;
	int notes[250];
	int measures[250];
	int currentNote;
	int currentMeasure;
	string songName;
	float measureTrackerX[50];
	float measureTrackerY[50];


	~sheetMusic();
};