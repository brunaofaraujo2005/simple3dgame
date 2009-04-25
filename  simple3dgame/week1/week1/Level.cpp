#include "Level.h"

//Default constructor
Level::Level(){
	_numPlatforms = -1;
};

//Constructor opent een doolhof
Level::Level(ifstream &mazeStream){
	char chrLine[201];						//Bevat de regel in chars
	unsigned int step = 0;					//Houdt bij welke waarde er ingelezen wordt
	unsigned int lineNumber = 0;			//Houdt het regelnummer bij van het level (Y-as)
	unsigned int platform = 0;				//Houdt het niveau bij
	unsigned int symbol;					//Bevat het symbool wat op de betreffende positie staat
	
	while(platform != _numPlatforms){
		mazeStream.getline(chrLine, 200);
		if ((chrLine[0] != '%') && (chrLine[0] != 0)){ //Commentaar en lege regels overslaan
			//Level aanmaken, four steps numplatforms, width, height, platforms
			switch (step){
				case 0:	//Numplatforms
					_numPlatforms = atoi(chrLine);
					step++;
					break;
				case 1: //Width
					_width = atoi(chrLine);
					step++;
					break;
				case 2: //Height
					_height = atoi(chrLine);
					step++;
					break;
				case 3: //Platform
					//Complete breedte doorlopen
					for (unsigned int i = 0; i < _width; i++){
						//Vertaal symbool naar waarde
						switch (chrLine[i]){
							case 'S':
								symbol = START;
								_startPosition.platform = platform;
								_startPosition.x = i;
								_startPosition.y = lineNumber;
								break;
							case 'X':
								symbol = EXIT;
								break;
							case 'H':
								symbol = HOLE;
								break;
							case 'L':
								symbol = LIFT;
							case 'W':
								symbol = WATER;
							case 'B':
								symbol = BATTERY;
								break;
							case 'C':
								symbol = CHIP;
								break;
							case '#':
								symbol = WALL;
								break;
							default:
								symbol = SPACE;
								break;
						}
						_maze.push_back(symbol);	//Symbool toevoegen aan vector, wordt automatich geteld.
					}
					lineNumber++;
					//Controleren of het einde van het platform is bereikt.
					if (lineNumber == _height){
						platform++;
						lineNumber = 0;
						}
			}
		}	
	}
}

//Deconstructor
Level::~Level(){
}