#include "stdafx.h"
#include "RoboRally.h"

//Default constructor
RoboRally::RoboRally()
{
	init();
}

//Globale initialisatie
void RoboRally::init(){
	string file = "maze.txt";
	_numLevels = 0;
	readLevels(file);

	if (_numLevels > 0)
		initLevel(1);

	_curEnergy = _startEnergy;
	_curLives = _startLives;
	_curScore = 0;
}

//Initialiseer het nieuwe level
void RoboRally::initLevel(unsigned int level){
	//Maak een copy van het level waarin dingen gewijzigd kunnen worden, is volgende keer sneller laden.
	_curLevelNr = level;
	_curLevel = _levels[level-1];
	_curPosition = _curLevel.getStartPosition();
	_curOrientation = NORTH;	//Eventueel aanpassen, zodat deze nooit met neus naar muur begint.
}

//Controleert of de robot vooruit kan i.v.m. muur
bool RoboRally::canMove(directions direction){
	//Afhankelijk van vooruit of achteruit
	int move;
	if (direction == FORWARD)
		move = 1;
	else
		move = -1;
	//Afhankelijk van de orientatie welke kant op tellen
	switch (_curOrientation){
		case NORTH:
			if (_curLevel.getElement(_curPosition.x, _curPosition.y - move, _curPosition.platform) != WALL)
				return true;
			break;
		case EAST:
			if (_curLevel.getElement(_curPosition.x + move, _curPosition.y, _curPosition.platform) != WALL)
				return true;
			break;
		case SOUTH:
			if (_curLevel.getElement(_curPosition.x, _curPosition.y + move, _curPosition.platform) != WALL)
				return true;
			break;
		case WEST:
			if (_curLevel.getElement(_curPosition.x - move, _curPosition.y, _curPosition.platform) != WALL)
				return true;
			break;
		default:
			return false;
	}
	return false;
};

//geef een representatie in een string
string RoboRally::getCurState(){
	//currentState = _curLevel.getPlatform(_curPosition.platform);
	string currentState = "";
	for(unsigned int y=0; y < _curLevel.getHeight(); y++){
		for(unsigned int x=0; x < _curLevel.getWidth(); x++){
			//Robot tekenen
			if ((x == _curPosition.x) && (y == _curPosition.y)){
				switch (_curOrientation){
					case NORTH:
						currentState += "^";
						break;
					case EAST:
						currentState += ">";
						break;
					case SOUTH:
						currentState += "v";
						break;
					case WEST:
						currentState += "<";
						break;
				}
			}
			else{
				//Terugrekenen naar index van vector ((y*w)+x+(p*w*h))
				//switch (_maze[(((y * _width) + x) + (platform * _width *_height))]){
				switch (_curLevel.getElement(x, y, _curPosition.platform)){
					case START:
						currentState += "S";
						break;
					case EXIT:
						currentState += "X";
						break;
					case HOLE:
						currentState += "H";
						break;
					case LIFT:
						currentState += "L";
						break;
					case WATER:
						currentState += "W";
						break;
					case BATTERY:
						currentState += "B";
						break;
					case CHIP:
						currentState += "C";
						break;
					case WALL:
						currentState += "#";
						break;
					default:
						currentState += " ";
						break;
				}
			}
		}
		//Toevoegen van endline
		currentState += "\r\n";
	}

	currentState += "\r\n\r\n";
	currentState += "SCORE: " + toString(_curScore) + "\r\n";
	currentState += "ENERGY: " + toString(_curEnergy) + "\r\n";
	currentState += "LIVES: " + toString(_curLives) + "/" + toString(_startLives) + "\r\n";
	return currentState;
}

//Verplaats de robot
void RoboRally::moveRobot(directions direction){
	//Afhankelijk van vooruit of achteruit
	int move;
	if (direction == FORWARD)
		move = 1;
	else
		move = -1;
	
	if (canMove(direction)){		
		//Controleren of er genoeg energie is
		if (_curEnergy == 1){
			reInitLevel(--_curLives);
			return;
		}
		_curEnergy -= _walkCost;
		//Afhankelijk van de orientatie welke kant op tellen
		switch (_curOrientation){
			case NORTH:
				_curPosition.y = _curPosition.y - move;
				break;
			case EAST:
				_curPosition.x = _curPosition.x + move;
				break;
			case SOUTH:
				_curPosition.y = _curPosition.y + move;
				break;
			case WEST:
				_curPosition.x = _curPosition.x - move;
				break;
		}
		//Level vernieuwen
		refreshState();
	}
	else{ 
		//MUUR, kost dit ook energie?
	}
}

void RoboRally::refreshState(){
	//Element ophalen waar de robot nu op staat (space == element)
	elements element;
	element = _curLevel.getElement(_curPosition);
	//Controleren wat er moet gebeuren
	switch (element){
		case EXIT:
			//Je gaat naar het volgende level mits het niet het laatste Level was.
			if (_curLevelNr == _levels.size()){
				initLevel(1);//Gefinished, show hight scores ofzoiets, voor nu een herstart :P
				return;
			}
			initLevel(++_curLevelNr);
			break;
		case HOLE:
			//Je gaat een niveau omlaag
			_curPosition.platform = _curPosition.platform - 1;
			//Controleren op tweede hole, want dan kost je dat een leven.
			if (_curLevel.getElement(_curPosition) == HOLE)
				reInitLevel(--_curLives);
			break;
		case LIFT:
			//Je gaat een niveau omhoog
			_curPosition.platform = _curPosition.platform + 1;
			//Controlerne op tweede lift
			if (_curLevel.getElement(_curPosition) == LIFT)
				refreshState();
			break;
		case WATER:
			//Kost leven en je gaat weer naar Start, energie wordt gereset
			reInitLevel(--_curLives);
			break;
		case BATTERY:
			//Baterij energie level stijgt
			_curEnergy += _batteryFuel;
			_curLevel.setElement(_curPosition, SPACE);
			break;
		case CHIP:
			//Score wordt opgehoogd
			_curScore += _chipPoints;
			_curLevel.setElement(_curPosition, SPACE);
			break;
		default:	//SPACE
			break;
		}
}

//Herinitialiseer level
void RoboRally::reInitLevel(unsigned int lives){
	if (lives > 0){
		//Startpositie herstellen
		_curPosition = _curLevel.getStartPosition();
		_curOrientation = NORTH;
		//Nieuwe energy?
		_curEnergy = _startEnergy;
	}
	else{ //Helemaal vooraan starten
		initLevel(1);	
		_curEnergy = _startEnergy;
		_curLives = _startLives;
		_curScore = 0;
	}
}

//Laad levels in
bool RoboRally::readLevels(string &file){
	char chrLine[201];
	//Aanmaken filestream om te lezen
	ifstream fileStream;
	fileStream.open(file.c_str());
	
	if (fileStream.fail())
		return false; //Fout bij openen bestand

	//Bestand lezen tot aantal levels bekend is
	while (_numLevels == 0){
		fileStream.getline(chrLine, 200);
		if ((chrLine[0] != '%') && (chrLine[0] != 0)){ //Commentaar en lege regels overslaan
			//eerste waarde is het aantal levels.
			_numLevels = atoi(chrLine);			
		}
	}

	//Levels ophalen
	while (_numLevels != _levels.size()){
		Level level;
		level = Level(fileStream);
		_levels.push_back(level);		
	}

	return true;
}

//Draait de robot linksom
void RoboRally::turnRobotLeft(){
	if (_curOrientation == NORTH)
		_curOrientation = WEST;
	else
		_curOrientation--;
	_curEnergy -= _turnCost;
};

//Draait de robot rechtsom
void RoboRally::turnRobotRight(){
	if (_curOrientation == WEST)
		_curOrientation = NORTH;
	else
		_curOrientation++;
	_curEnergy -= _turnCost;
};

//Deconstructor
RoboRally::~RoboRally(void)
{
}
