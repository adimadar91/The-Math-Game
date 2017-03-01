#ifndef _ITEM_H_
#define _ITEM_H_

#include "Point.h"

class TheMathGame;

class Item 
{
	enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, STOP = 4, ROTATE = 5 };
	list <Point> place; // this is the Point of the item
	char sign = 'I'; // sign that indicates that this is item
	int direction = DOWN; // the direction of the item
	int volume;
	Color color; // set the color of the item
	TheMathGame* game; // pointer to the game!
	Point temp;

public:
	bool operator==(const Item& other)const;
	int sizeoflistofpoints()const { return place.size(); }
	bool moveItem();
	Item() {}
	void setgame(TheMathGame* t){ game = t; } // setting the game
	list<Point>* getlist() { return &place; }
	list<Point> getlist2()const { return place; }
	void setItemPosition(int y, int x); // set item's point
	void setItemDirection(int dir) { direction = dir; }// set the direction
	void setItemColor(Color _color){color = _color;}
	bool theNextOfItemIsWall(int _dir)const;
	int FindMaxX()const;
	int FindMinX()const;
	int FindMaxY()const;
	int FindMinY()const;
	void setVolume();
	int getVolume()const;
	Color getColor() { return color; }
	void freeListOfPoints(){place.clear();}
	bool theNextOfItemIsItem(int _dir);
	bool thisIsMe(Point p)const;

	
};




#endif