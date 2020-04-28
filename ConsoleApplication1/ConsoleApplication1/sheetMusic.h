#pragma once
#include <string>

using namespace std;

class sheetMusic
{
public:
	sheetMusic(int song[], int l);
	void setNotes(int* cursor);
	bool checkNote(int note);
	bool checkEndOfSong();
	void reset();

	int length;
	int notes[200];
	int currentNote;
	string imagePath;


	~sheetMusic();
};

