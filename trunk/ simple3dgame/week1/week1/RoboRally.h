#pragma once

#include "TypeDefs.h"
#include "Level.h"

class RoboRally{
private:
	//Global vars
	vector<Level> _levels;				//Bevat de levels bij index (level 1 == index 0)
	unsigned int _numLevels;			//Aantal levels
	//Ingame vars
	unsigned int _curDirection;			//Bevat de huidige oriëntatie
	Level _curLevel;					//Bevat het huidige level
	unsigned int _curLevelNr;			//Bevat het huidige level nummer
	position _curPosition;				//Bevat de huidige positie van de robot
	unsigned int _energy;				//Bevat het energielevel van de robot
	unsigned int _lives;				//Bevat het aantal levens	
	unsigned int _score;				//Bevat de score van de speler
	
	void init();						//Globale initialisatie
	void initLevel(unsigned int level);	//Initialiseert een level
	bool readLevels(string &file);		//Leest de levels uit een bestand in
public:
	RoboRally();
	~RoboRally();	
	//Getters
	position getCurrentPosition(){return _curPosition;};
	unsigned int getCurPositionPlatform(){return _curPosition.platform;};
	unsigned int getCurPositionX(){return _curPosition.x;};
	unsigned int getCurPositionY(){return _curPosition.y;};
	unsigned int getCurDirection(){return _curDirection;};
	unsigned int getEnergy(){return _energy;};
	unsigned int getLives(){return _lives;};
	unsigned int getScore(){return _score;};

};
