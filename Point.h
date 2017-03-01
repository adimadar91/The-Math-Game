#ifndef _POINT_H_
#define _POINT_H_

#include <iostream>
#include "Io_Utils.h"

using namespace std;

class Point {
	int x = 1; int y = 1; // initialize the coordinates to (1,1)

public:

	Point(int row=1, int col=1) { x = row; y = col; }; // this is a c'tor for the point.
	bool operator==(Point p) const 
	{ 
		return((p.getY() == y) && (p.getX() == x)); 
	}
	int getX() const // this function helps sends the X coordinate
	{
		return x;
	}
	int getY() const // this function helps sends the Y coordinate
	{
		return y;
	}
	void set(int _x, int _y) // this functions sets the X and the Y that we send.
	{
		x = _x;
		y = _y;
	}
	void draw(char ch, char sign, char rotateStatus = 'p');
	void move(int direction);
	Point next(int direction);// sends the next place by the direction - helps us in many funcs to determine our next step!
	bool isSame(const Point& p) const // checks if it is the same point.
	{
		return x == p.x && y == p.y;
	}
};

#endif