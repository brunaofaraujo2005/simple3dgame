#pragma once

#include <fstream>				
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct position{				//Bevat de positie x, y, platform
    unsigned int x;
    unsigned int y;
    unsigned int platform;
};

//Om makkelijk te werken een enumeration van veelgebruikte "termen".
enum elements{START, EXIT, SPACE, HOLE, LIFT, WATER, BATTERY, CHIP, WALL};
enum orientations{NORTH, EAST, SOUTH, WEST};
enum directions{FORWARD, BACKWARD};

//To string functie om datatype naar een string te zetten.
template <class DataType>
inline string toString(const DataType& input){
	stringstream stream;
	stream << input;
	return stream.str();
}