
#ifndef _CREATURES_H_
#define _CREATURES_H_


#include "Point.h"

class TheMathGame;

class Creatures
{


public:
	enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, STOP = 4, ROTATE = 5 };
	char sign;
	Point position;//we save the creatures with one Point
	int direction = STOP;//defult direction
	TheMathGame* theGame;
	Color color;
	Creatures(char _s) { sign = _s; }//ctr
	virtual bool move() = 0;
	virtual void setGame(TheMathGame* _g) = 0;
	virtual void setColor(Color c) = 0;
	virtual void setPosition(int row, int col) = 0;

};

class RowFlyers : public Creatures
{

public:
	RowFlyers(char s = '!', int _dir = LEFT) : Creatures(s) { direction = _dir, color = Color::LIGHTMAGENTA; }
	void setColor(Color c) { color = c; }
	void setGame(TheMathGame* _g) { theGame = _g; }
	void setPosition(int row, int col) { position.set(col, row); }
	bool move();

};

class ColFlyers : public Creatures
{
public:
	ColFlyers(char s = '$', int _dir = DOWN) : Creatures(s) { direction = _dir, color = Color::LIGHTMAGENTA; }
	void setColor(Color c) { color = c; }
	void setGame(TheMathGame* _g) { theGame = _g; }
	void setPosition(int row, int col) { position.set(col, row); }
	bool move();
};

class NumEaters : public Creatures
{
	bool isDead = false;//inform us if the numeaters is at the game or not
public:

	NumEaters(char s = '%') : Creatures(s) { color = Color::BROWN; }
	void setColor(Color c) { color = c; }
	void setGame(TheMathGame* _g) { theGame = _g; }
	void setPosition(int row, int col) { position.set(col, row); }
	bool move();
	bool getDeadStatus();
	bool operator==(const NumEaters& other)const;
};


#endif