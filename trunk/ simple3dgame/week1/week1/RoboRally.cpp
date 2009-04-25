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

	_energy = 50;
	_lives = 3;
	_score = 0;
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
			if (_curLevel.getElement(_curPosition.x - move, _curPosition.y, _curPosition.platform) != WALL)
				return true;
			break;
		case SOUTH:
			if (_curLevel.getElement(_curPosition.x, _curPosition.y + move, _curPosition.platform) != WALL)
				return true;
			break;
		case WEST:
			if (_curLevel.getElement(_curPosition.x + move, _curPosition.y, _curPosition.platform) != WALL)
				return true;
			break;
		default:
			return false;
	}
	return false;
};

//Verplaats de robot
void RoboRally::moveRobot(directions direction){
	elements element;
	//Afhankelijk van vooruit of achteruit
	int move;
	if (direction == FORWARD)
		move = 1;
	else
		move = -1;
	
	if (canMove(direction)){		
		//Afhankelijk van de orientatie welke kant op tellen
		switch (_curOrientation){
			case NORTH:
				_curPosition.y = _curPosition.y - move;
				element = _curLevel.getElement(_curPosition);
				break;
			case EAST:
				_curPosition.x = _curPosition.x - move;
				element = _curLevel.getElement(_curPosition);
				break;
			case SOUTH:
				_curPosition.y = _curPosition.y + move;
				element = _curLevel.getElement(_curPosition);
				break;
			case WEST:
				_curPosition.x = _curPosition.x + move;
				element = _curLevel.getElement(_curPosition);
				break;
		}
		switch (element){
				case EXIT:
					//Je gaat naar het volgende level
					initLevel(_curLevelNr++);
					break;
				case HOLE:
					//Je gaat een niveau omlaag
					_curPosition.platform = _curPosition.platform - 1;
					break;
				case LIFT:
					//Je gaat een niveau omhoog
					_curPosition.platform = _curPosition.platform + 1;
					break;
				case WATER:
					//Kost leven en je gaat weer naar Start
					_lives--;
					_curPosition = _curLevel.getStartPosition();
					break;
				case BATTERY:
					//Baterij energie level stijgt
					_energy += _batteryFuel;
					break;
				case CHIP:
					//Score wordt opgehoogd
					_score += _chipPoints;
					break;
				default:	//SPACE
					break;
		}
		_energy -= _walkCost;

		//de rest
			//Controleren of de robot niet dood is
			//Controleren of deze niet door 2 Holes is gezakt
			//Level ten einde ??
	}
	else{ 
		//MUUR
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
	_energy -= _turnCost;
};

//Draait de robot rechtsom
void RoboRally::turnRobotRight(){
	if (_curOrientation == WEST)
		_curOrientation = NORTH;
	else
		_curOrientation++;
	_energy -= _turnCost;
};

//Deconstructor
RoboRally::~RoboRally(void)
{
}
