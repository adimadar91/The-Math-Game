#include "TheMathGame.h"

bool Item::theNextOfItemIsWall(int _dir)const
{
	for (Point p : place)
	{
		if (game->isWall(p.next(_dir)))
			return true;
	}

	return false;
}
bool Item::theNextOfItemIsItem(int _dir)
{
	for (Point p : place)
	{
		if (game->isItemInTheWay(p.next(_dir)) && !(thisIsMe(p.next(_dir))))
		{
			game->combineItems(p.next(_dir), *this);
			return true;
		}
	}
	return false;
}
bool Item::thisIsMe(Point p)const
{
	
	for (Point temp : place)
	{
		if (temp==p)
			return true;
	}
	return false;
}
bool Item::operator==(const Item& other) const
{
	bool flag = false;
	if (other.sizeoflistofpoints() != place.size())
		return false;
	else
	{
		for (Point p : other.getlist2())
		{
			for (Point p2 : place)
			{
				if (p2 == p)
					flag = true;
				
			}
			if (flag == false)
				return false;
			flag = false;
		}
		return true;
	}
}
bool Item::moveItem()
{
	enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, STOP = 4, ROTATE = 5 };
	theNextOfItemIsItem(direction);

	bool flag = true;
	setVolume();

	for (Point p : place)
	{
		Point t = p.next(direction);
		if (game->isWall(t))
			flag = false;
	}
	
	if (flag == false)
		setItemDirection(STOP);


	if (flag == true )
	{
		for (list<Point>::iterator p = place.begin(); p != place.end(); p++)
		{
			if (game->isNum(p->next(direction))) // checks if the next place is num if yes update the board that this num is no longer exist
				game->insertCharInsteadOfNumForItem(p->next(direction));

			if (game->isSpaceshipAhead(p->next(DOWN), sign)) // if there is a spaceship in the next position and the item is falling down
			{
				if (game->HowManyLivesLeft() > 0)
				{
					game->decreaseLives(); // lives -- 
					//game->decreaseLevel();
					game->SetItemHitSpaceshipToTrue();
					
					game->init(); // init the board to start over 
					game->SetItemHitSpaceshipToFalse();
					return false;
				}
			}
		}

		if (direction == RIGHT)
		{
			int max = FindMaxX();
			while (max >= 0)
			{
				for (list<Point>::iterator p = place.begin(); p != place.end(); p++)
				{
					if (p->getX() == max)
					{
						p->draw(' ', sign); // draw spcae in the current place before moving to the next place
						game->insertCharToBoard(p->getX(), p->getY(), ' '); // update the board

						p->move(direction); // move the item according to the direction
						setTextColor(color);  // set the item color for printing on the screen
						p->draw('*', sign);// in the place draw the item
						game->insertCharToBoard(p->getX(), p->getY(), '*');//update the board 
						setTextColor(Color::LIGHTGREY); // change color back to normal
					}
				}
				max--;
			}
		}
		if (direction == LEFT)
		{
			int min = FindMinX();
			 
			while (min <= 80)
			{
				for (list<Point>::iterator p = place.begin(); p != place.end(); p++)
				{
					if (p->getX() == min)
					{
						p->draw(' ', sign); // draw spcae in the current place before moving to the next place
						game->insertCharToBoard(p->getX(), p->getY(), ' '); // update the board

						p->move(direction); // move the item according to the direction
						setTextColor(color);  // set the item color for printing on the screen
						p->draw('*', sign);// in the place draw the item
						game->insertCharToBoard(p->getX(), p->getY(), '*');//update the board 
						setTextColor(Color::LIGHTGREY); // change color back to normal
					}
				}
				min++;
			}
		}
		if (direction == DOWN)
		{
			int max = FindMaxY();

			while (max >= 0)
			{
				for (list<Point>::iterator p = place.begin(); p != place.end(); p++)
				{
					if (p->getY() == max)
					{
						p->draw(' ', sign); // draw spcae in the current place before moving to the next place
						game->insertCharToBoard(p->getX(), p->getY(), ' '); // update the board

						p->move(direction); // move the item according to the direction
						setTextColor(color);  // set the item color for printing on the screen
						p->draw('*', sign);// in the place draw the item
						game->insertCharToBoard(p->getX(), p->getY(), '*');//update the board 
						setTextColor(Color::LIGHTGREY); // change color back to normal
					}

				}
				max--;
			}
		}
	}
	setItemDirection(DOWN); // item always wants to fall down!!! gravity does its job
	return true;

}
int Item::FindMaxX()const// finding the maximum X  point
{
	int MAX = -1;
	for (Point p : place)
	{
		if (p.getX() > MAX)
			MAX = p.getX();
	}

	return MAX;
}
int Item::FindMinX()const// finding the minimum X point
{
	int MIN = 10000000;
	for (Point p : place)
	{
		if (p.getX() < MIN)
			MIN = p.getX();
	}

	return MIN;

}
int Item::FindMaxY()const // finding the maximum Y  point
{
	int MAX = -1;
	for (Point p : place)
	{
		if (p.getY() > MAX)
			MAX = p.getY();
	}

	return MAX;
}
int Item::FindMinY()const // finding the minimum Y point
{
	int MIN = 10000000;
	for (Point p : place)
	{
		if (p.getY() < MIN)
			MIN = p.getY();
	}

	return MIN;
}
void Item::setVolume()
{
	volume = place.size();
}
int Item::getVolume()const
{
	return volume;
}
void Item::setItemPosition(int y, int x) // set item's point
{
	temp.set(x, y);
	place.push_back(temp);
}






