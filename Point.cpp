#include "TheMathGame.h"
#include "Point.h"

enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, STOP = 4, ROTATE = 5 };

void Point::draw(char ch, char sign, char rotateStatus ) // prints to the screen the specific item(Small Spaeship, Big spaceship, item)
{
	if (sign == 'S') // if it is S (Small Spaceship)
	{
		if (rotateStatus == 'N') // if the spaceship os not in rotate mode
		{
			for (int i = 0; i < 3; i++) // prints the small on the screen
			{
				gotoxy(x + i, y);
				cout << ch; // the char of the small spaceship - @
				cout.flush();
			}
		}
		else // if it is in rotate status
		{
			for (int i = 0; i < 3; i++)
			{
				gotoxy(x, y + i); // prints the small spaceship as needed
				cout << ch;
				cout.flush();
			}
		}
	}
	else if (sign == 'B') // the big spaceship - same description as the small spaceship
	{
		if (rotateStatus == 'N')
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					gotoxy(x + i, y + j);
					cout << ch;
					cout.flush();
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					gotoxy(x + j, y + i);
					cout << ch;
					cout.flush();
				}
			}
		}

	}
	else if (sign == 'I') // item
	{

		gotoxy(x, y);
		cout << ch;
		cout.flush();
	}
	else if (sign == '!' || sign == '$' || sign == '%')
	{
		gotoxy(x, y);
		cout << ch;
		cout.flush();
	}
}
void Point::move(int dir) //TO DO TOTATE&STOP
{
	switch (dir) 
	{
	case UP: 
		y--;
		if (y == 0 )
			y = 23;
		break;
	
	case DOWN: 
		y++;
		if (y == 24) 
			y = 1;
		break;
	
	case LEFT: 
		x--;
		if (x < 1) 
			x = 79;
		break;
	
	case RIGHT: 
		x++;
		if (x > 79) 
			x = 1;
		break;

	case STOP:
	//does nothing
		;

	}
}
Point Point::next(int direction) // sends the next place by the direction - helps us in many funcs to determine our next step!
{
	Point next = *this;
	next.move(direction);
	return next;
}