#include "RoboRally.h"

//Default constructor
RoboRally::RoboRally()
{
	init();
}

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
	_curDirection = NORTH;	//Eventueel aanpassen, zodat deze nooit met neus naar muur begint.
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

//Deconstructor
RoboRally::~RoboRally(void)
{
}
