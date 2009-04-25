#include "Level.h"
#include <vector>

using namespace std;

bool main(){
	int _numLevels;
	vector<Level> _levels;

	string mazeFile = "maze.txt";
	ifstream mazeStream;
	mazeStream.open(mazeFile.c_str());
	
	if (mazeStream.fail())
		return 0; //Fout bij openen bestand

	//Bestand lezen tot aantal levels
	char chrLine[201];
	string strLine;

	while(!mazeStream.eof()){
		mazeStream.getline(chrLine, 200);
		strLine.assign(chrLine);
		if ((chrLine[0] != '%') && (chrLine[0] != 0)){ //Commentaar en lege regels overslaan
			//eerste waarde is het aantal levels.
			_numLevels = atoi(chrLine);		

			//Hier de levels aanmaken //LET OP! kunnen er meerdere zijn
			while(!mazeStream.eof()){
				Level level;
				level = Level(mazeStream);
				_levels.push_back(level);
			}
		}

	}

	return 0;
}