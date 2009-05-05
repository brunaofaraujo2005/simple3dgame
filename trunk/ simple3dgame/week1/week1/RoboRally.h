#pragma once

#include "TypeDefs.h"
#include "Level.h"

class RoboRally{
private:
	//Globale constanten
	static const unsigned int _startEnergy = 50;//Hoeveel energie je bij start krijgt
	static const unsigned int _startLives = 03;	//Aantal levens waar je mee begint
	static const unsigned int _chipPoints = 10;	//Aantal punten een chip oplevert
	static const unsigned int _batteryFuel = 10;//Hoeveelheid energie een battery oplevert.
	static const unsigned int _turnCost = 0;	//Hoeveel energie kost het om te draaien
	static const unsigned int _walkCost = 1;	//Hoeveel energie kost het om een stap te zetten
	//Global vars
	vector<Level> _levels;						//Bevat de levels bij index (level 1 == index 0)
	unsigned int _numLevels;					//Aantal levels
	//Ingame vars
	unsigned int _curOrientation;				//Bevat de huidige oriëntatie
	Level _curLevel;							//Bevat het huidige level
	unsigned int _curLevelNr;					//Bevat het huidige level nummer
	position _curPosition;						//Bevat de huidige positie van de robot
	unsigned int _curEnergy;					//Bevat het energielevel van de robot
	unsigned int _curLives;						//Bevat het aantal levens	
	unsigned int _curScore;						//Bevat de score van de speler
	//Methoden
	void init();								//Globale initialisatie
	void initLevel(unsigned int level);			//Initialiseert een level
	void refreshState();						//Voert alle game handelingen uit (na bijv. een stap).
	void reInitLevel(unsigned int lives);		//Level wordt gereïnitialiseerd
	bool readLevels(string &file);				//Leest de levels uit een bestand in
public:
	RoboRally();								//Constructor
	~RoboRally();								//Deconstructor
	void turnRobotLeft();						//Draai de robot naar links
	void turnRobotRight();						//Draai de robot naar rechts
	void moveRobot(directions direction);		//Verplaats de robot voor of achteruit

	//********* tijdelijk publiekelijk *********
	bool canMove(directions direction);			//Controleert of de robot vooruit kan i.v.m. muur

	//Getters
	string getCurState();
	Level getCurLevel(){return _curLevel;};
	position getCurrentPosition(){return _curPosition;};
	unsigned int getCurPositionPlatform(){return _curPosition.platform;};
	unsigned int getCurPositionX(){return _curPosition.x;};
	unsigned int getCurPositionY(){return _curPosition.y;};
	unsigned int getCurOrientation(){return _curOrientation;};
	unsigned int getCurEnergy(){return _curEnergy;};
	unsigned int getCurLives(){return _curLives;};
	unsigned int getCurScore(){return _curScore;};
};
