#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <cstring>
#include <iostream>
#include "Point.h"
#include "Color.h"
#include <list>

class TheMathGame;

class Spaceship {
	enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, STOP = 4, ROTATE = 5};
	enum {SPACESHIP_ROWS = 2, SPACESHIP_COLS = 3, NUMOFKEYS=6 };
	Point head;
	list<Point> templst; // this list is to check what is the given roatation status of each spaceship
	char sign, rotateStatus;
	int direction = STOP;
	char arrowKeys[NUMOFKEYS];
	Color color;
	TheMathGame* theGame;

public:
	Spaceship(char model) {	sign = model;} //c'tor
	int sendX()const {	return head.getX();}// the return of the X
	int sendY()const {	return head.getY();}// the return of the Y
	void setGame(TheMathGame* _theGame)  {	theGame = _theGame;}// set the pointer of the game
	void setPosition(int y, int x);
	void addPointToList(int y, int x);
	void findRotationStatusFromList();
	void setArrowKeys(const char* keys)  // sets the correct keys of the spaceships
	{
		arrowKeys[0] = keys[0];
		arrowKeys[1] = keys[1];
		arrowKeys[2] = keys[2];
		arrowKeys[3] = keys[3];
		arrowKeys[4] = keys[4];
		arrowKeys[5] = keys[5];
	}
	bool checkIfRotationIsPossible()const;
	void insertSpacesForRotation();
	void setRotationStatus();
	void setRotationStatusFromTheStart(char stat){	rotateStatus = stat; }// when scanning the board we will find how it was given to us (rotated or not)
	void setColor(Color c) 	{ color = c; } // sets the color of the spaceship
	void move();
	int getDirection(const list<char>& key)const;
	void setDirection(int dir) { direction = dir; } // sets the direction of the spaceship
	int getDirectionForReplay(char c, char sign)const;
	void clearList(){templst.clear();}
	

};

#endif