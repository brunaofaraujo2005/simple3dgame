#pragma once

#include "TypeDefs.h"

/* Level representation in vector
	########	0  1  2  3  4  5  6  7
	# C # X#    8  9  10 11 12 13 14 15 etc.
	# # C ##
	#C######
	#  WL  #
	######C#
	#S  C  #
	########
*/

class Level{
private:
	unsigned int _numPlatforms;		//Aantal aanwezige platforms
	unsigned int _width, _height;	//Bevat de breedte en hoogte van het level
	position _startPosition;		//Bevat de startlocatie
	vector<elements> _maze;			//Simpele vector representatie van het level
public:
	Level();						//Default constructor
	Level(ifstream &mazeStrean);	//Constructor die een doolhof inleest.
	~Level();
	//Setters
	void setElement(unsigned int x, unsigned int y, unsigned int platform, elements element);
	void setElement(position point, elements element);
	//Getters
	elements getElement(unsigned int x, unsigned int y, unsigned int platform);
	elements getElement(position point);
	unsigned int getWidth(){return _width;};
	unsigned int getHeight(){return _height;};
	string getPlatform(unsigned int platform);
	unsigned int getPlatforms(){return _numPlatforms;};
	position getStartPosition(){return _startPosition;};
	unsigned int getStartPlatForm(){return _startPosition.platform;};
	unsigned int getStartX(){return _startPosition.x;};
	unsigned int getStartY(){return _startPosition.y;}
};