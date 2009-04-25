#include "TypeDefs.h"

#include <fstream>				//Om files te streamen
#include <vector>

//TODO: 

/* Level representation in vector
	########	0  1  2  3  4  5  6  7
	# C # X#    8  9  10 11 12 13 14 15 etc.
	# # C ##
	#C######
	#  WL  #
	######C#
	#S  C  #
	########
*/
using namespace std;

class Level{
private:
	struct position{                                //Bevat de positie x, y, platform
        unsigned int x;
        unsigned int y;
        unsigned int platform;
    };
	unsigned int _numPlatforms;		//Aantal aanwezige platforms
	unsigned int _width, _height;	//Bevat de breedte en hoogte van het level
	position _startPosition;		//Bevat de startlocatie
	vector<unsigned int> _maze;		//Simpele vector representatie van het level
public:
	Level();						//Default constructor
	Level(ifstream &mazeStrean);	//Constructor die een doolhof inleest.
	~Level();
	//Setters
	void setSymbol(unsigned int x, unsigned int y, unsigned int platform, unsigned int symbol);
	//Getters
	unsigned int getWidth(){return _width;};
	unsigned int getHeight(){return _height;};
	string getPlatform(unsigned int platform);
	unsigned int getPlatforms(){return _numPlatforms;};
	unsigned int getStartPlatForm(){return _startPosition.platform;};
	unsigned int getStartX(){return _startPosition.x;};
	unsigned int getStartY(){return _startPosition.y;}
};