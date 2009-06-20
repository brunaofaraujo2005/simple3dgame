#include "stdafx.h"
#include "Level.h"

using namespace std;

bool main(){
	int _numLevels = -1;
	vector<Level> _levels;

	string mazeFile = "maze.txt";
	ifstream mazeStream;
	mazeStream.open(mazeFile.c_str());
	
	if (mazeStream.fail())
		return 0; //Fout bij openen bestand

	//Bestand lezen tot aantal levels bekend is
	char chrLine[201];

	while (_numLevels == -1){
		mazeStream.getline(chrLine, 200);
		if ((chrLine[0] != '%') && (chrLine[0] != 0)){ //Commentaar en lege regels overslaan
			//eerste waarde is het aantal levels.
			_numLevels = atoi(chrLine);			
		}
	}

	//Levels ophalen
	while (_numLevels != _levels.size()){
		Level level;
		level = Level(mazeStream);
		_levels.push_back(level);		
	}

	//Ophalen leven en iets veranderen test code
	Level level1;
	level1 = _levels[0];
	level1.setElement(7,13,0,WATER);
	string strPlatform;
	strPlatform =level1.getPlatform(0);

	cout << strPlatform;
	
	unsigned int element;
	element = level1.getElement(7,13,0);

	return 0;
}