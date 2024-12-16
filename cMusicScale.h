#pragma once

#include <math.h>
#include <string>
#include <vector>

enum note
{
	C, Des, D, Es, E, F, Ges, G, As, A, B, H
};
//  0,  1 , 2, 3 , 4, 5, 6  , 7, 8 , 9,10,11

enum KeyColor
{
	white, black
};


struct key
{
	int oktave;
	int midi;
	double frequency;
	std::string name;
	bool isPressed;
	KeyColor color;
};

class cMusicScale
{
public:
	cMusicScale(double pitch);
	~cMusicScale();

	void pressKey(int midi, bool state);
	key findNearesNote(double frequency);
	double calcCent(key Note, double frequency);	
	
private:
	key m_keys[88]; //max. 88 Keys (Piano)
	double m_concertPitch = 440.0; // Hz

	std::vector<std::string> NoteNames = { "C", "CIS", "D", "DIS", "F", "FIS", "G", "GIS", "A", "AIS" ,"H" };


};

