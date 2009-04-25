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
								break;
							case 'W':
								symbol = WATER;
								break;
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

unsigned int Level::getElement(unsigned int x, unsigned int y, unsigned int platform){
	//Terugrekenen naar index van vector ((y*w)+x+(p*w*h))
	return _maze[(((y * _width) + x) + (platform * _width *_height))];
}

//Geeft het platform terug in een string (ASCII) representatie
string Level::getPlatform(unsigned int platform){
	string strPlatform = "";
	for(unsigned int y=0; y < _height; y++){
		for(unsigned int x=0; x < _width; x++){
			//Terugrekenen naar index van vector ((y*w)+x+(p*w*h))
			switch (_maze[(((y * _width) + x) + (platform * _width *_height))]){
				case START:
					strPlatform += "S";
					break;
				case EXIT:
					strPlatform += "X";
					break;
				case HOLE:
					strPlatform += "H";
					break;
				case LIFT:
					strPlatform += "L";
					break;
				case WATER:
					strPlatform += "W";
					break;
				case BATTERY:
					strPlatform += "B";
					break;
				case CHIP:
					strPlatform += "C";
					break;
				case WALL:
					strPlatform += "#";
					break;
				default:
					strPlatform += " ";
					break;
			}
		}
		//Toevoegen van endline
		strPlatform += "\r\n";
	}
	return strPlatform;
}

//Verander een bepaalde positie in het level
void Level::setElement(unsigned int x, unsigned int y, unsigned int platform, unsigned int element){
	unsigned int position;
	//Terugrekenen naar index van vector ((y*w)+x+(p*w*h))
	position = (((y * _width) + x) + (platform * _width * _height));
	_maze[position] = element;
}

//Deconstructor
Level::~Level(){
}