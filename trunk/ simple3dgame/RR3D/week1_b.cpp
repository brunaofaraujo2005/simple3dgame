/*
	Test klasse
	Representeert RoboRally in ASCII Art.
*/
#include "stdafx.h"
#include "RoboRally.h"

using namespace std;

bool main(){
	RoboRally rrGame;

/*	cout << "Naar voren: " << rrGame.canMove(FORWARD) ? "true" : "false";
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
*/
	rrGame.startGame();
	char invoer = '0';
	while (invoer != 'q'){
		cout << rrGame.getCurState();
		cin >> invoer;

		switch(invoer){
			case 'a':
				rrGame.turnRobotLeft();
				break;
			case 'd':
				rrGame.turnRobotRight();
				break;
			case 'w':
				rrGame.moveRobot(FORWARD);
				break;
			case 's':
				rrGame.moveRobot(BACKWARD);
				break;
		}
	}

	return 0;
}