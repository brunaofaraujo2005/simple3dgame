#include <fstream>				//Om files te streamen
#include <map>					//Multimap waar de posities in staan.

//TODO: function pointer, zodat ik een map met x,y kan maken

using namespace std;

class Level{
private:
	struct position{				//Bevat de positie level, x, y, platform
		unsigned int x;
		unsigned int y;
		int platform;
		unsigned int symbol;
	};
	enum{START, EXIT, SPACE, HOLE, LIFT, WATER, BATTERY, CHIP, WALL};	//Bevat level elementen
	unsigned int _numPlatforms;		//Aantal aanwezige platforms
	unsigned int _width, _height;	//Bevat de breedte en hoogte van het level
	position _startPosition;		//Bevat de startlocatie
//	map<position, unsigned int> _maze;		//Map waarbij de positie aangeeft, wat er op die positie is
	multimap<unsigned int, position> _maze;
public:
	Level();						//Default constructor
	Level(ifstream &mazeStrean);	//Constructor die een doolhof inleest.
	~Level();
	//Getters
	unsigned int getWidth(){return _width;};
	unsigned int getHeight(){return _height;};
	unsigned int getStartX(){return _startPosition.x;};
	unsigned int getStartY(){return _startPosition.y;};
	int getStartPlatForm(){return _startPosition.platform;};
};