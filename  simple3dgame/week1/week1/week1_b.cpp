#include "RoboRally.h"

using namespace std;

bool main(){
	RoboRally rrGame;

	cout << "Naar voren: " << rrGame.canMove(FORWARD) ? "true" : "false";
	cout << "\r\nNaar achteren: " << rrGame.canMove(BACKWARD) ? "true" : "false";
	cout << "\r\n\r\nDraai naar links";
	rrGame.turnRobotLeft();
	cout << "\r\nNaar voren: " << rrGame.canMove(FORWARD) ? "true" : "false";
	cout << "\r\nNaar achteren: " << rrGame.canMove(BACKWARD) ? "true" : "false";
	rrGame.turnRobotLeft();
	cout << "\r\n\r\nDraai naar links";
	cout << "\r\nNaar voren: " << rrGame.canMove(FORWARD) ? "true" : "false";
	cout << "\r\nNaar achteren: " << rrGame.canMove(BACKWARD) ? "true" : "false";
	cout << "\r\n\r\nDraai naar rechts 2x";
	rrGame.turnRobotRight();
	rrGame.turnRobotRight();
	cout << "\r\nNaar voren: " << rrGame.canMove(FORWARD) ? "true" : "false";
	cout << "\r\nNaar achteren: " << rrGame.canMove(BACKWARD) ? "true" : "false";


	return 0;
}