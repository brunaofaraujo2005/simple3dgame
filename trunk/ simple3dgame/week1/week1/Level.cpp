#include "Level.h"

//Default constructor
Level::Level(){
};

//Constructor opent een doolhof
Level::Level(ifstream &mazeStream){
	//Bestand lezen tot het einde	
	char chrLine[201];
	unsigned int step = 0;
	unsigned int lineNumber = 0;
	unsigned int platform = 0;
	unsigned int symbol;
	
	while(!mazeStream.eof()){
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
								_startPosition.symbol = symbol;
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
						position point;
						point.platform = platform;
						point.y = lineNumber;
						point.x = i;
						point.symbol = symbol;
						_maze.insert(pair<unsigned int, position>(i, point));
					}
					lineNumber++;
					//Controleren of het einde van het platform is bereikt.
					if (lineNumber == _height){
						//Controleren of nog niet de laatste floor is bereikt.
						if (platform != _numPlatforms-1){
							platform++;
							lineNumber = 0;
						}
						//Hier nog terugkeren naar game
					}
					break;
			}
		}	
	}
}

//Deconstructor
Level::~Level(){
}