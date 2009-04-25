#pragma once

#include <fstream>				
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct position{				//Bevat de positie x, y, platform
    unsigned int x;
    unsigned int y;
    unsigned int platform;
};

//Om makkelijk te werken een enumeration van veelgebruikte "termen".
enum {START, EXIT, SPACE, HOLE, LIFT, WATER, BATTERY, CHIP, WALL, NORTH, EAST, SOUTH, WEST};
