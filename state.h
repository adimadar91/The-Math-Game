#ifndef _State_H_
#define _State_H_

#include "Point.h"

class state
{
	Point pos;
	int clock;
	int level;

	public:
	
	state (Point p, int c, int l) :pos(p), clock(c), level(l) {} // c'tor
	
	state() {}; // empty c'tor
	
	Point GetPos() // returns the position
	{ 
		return pos; 
	}
	
	void updateclock() 
	{ 
		clock++; 
	}
	void initClock()
	{
		clock = 0;
	}
		
	int getClock()  // returns the clock
	{
		return clock;
	}
	
	int getLevel() // returns the current level
	{
		return level;
	}
	void getClock(int savedGameClock)
	{
		clock = savedGameClock;
	}

};



#endif
