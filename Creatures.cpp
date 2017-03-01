#include "Creatures.h"
#include "TheMathGame.h"

enum { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, STOP = 4, ROTATE = 5 };


bool ColFlyers::move()
{
	if (theGame->isSpaceshipAhead(position.next(direction), sign)) // if there is a spaceship int the next position and the item is falling down
	{
		if (theGame->HowManyLivesLeft() > 0)
		{
			theGame->decreaseLives(); // lives -- 
			theGame->SetItemHitSpaceshipToTrue();

			theGame->init(); // init the board to start over 
			theGame->SetItemHitSpaceshipToFalse();
			return false;
		}
	}
	else
	{
		if (theGame->isItemInTheWay(position.next(direction)))
		{
			theGame->removeTheItemFromTheListBasedOnSpecificPoint(position.next(direction));
		}
		if (theGame->isNumOnBoard(position.next(direction))) // there is number 
		{
			theGame->insertCharInsteadOfNumForItem(position.next(direction));
		}
		position.draw(' ', sign);
		theGame->insertCharToBoard(position.getX(), position.getY(), ' ');

		position.move(direction);
		setTextColor(color);  // set the item color for printing on the screen
		position.draw(sign, sign);// in the place draw the item
		theGame->insertCharToBoard(position.getX(), position.getY(), sign);//update the board 
		setTextColor(Color::LIGHTGREY); // change color back to normal
		return true;
	}

}
bool RowFlyers::move()
{
	if (theGame->isSpaceshipAhead(position.next(direction), sign)) // if there is a spaceship int the next position and the item is falling down
	{
		if (theGame->HowManyLivesLeft() > 0)
		{
			theGame->decreaseLives(); // lives -- 
			//game->decreaseLevel();
			theGame->SetItemHitSpaceshipToTrue();

			theGame->init(); // init the board to start over 
			theGame->SetItemHitSpaceshipToFalse();
			return false;
		}
	}
	else
	{
		if (theGame->isItemInTheWay(position.next(direction)))
		{
			theGame->removeTheItemFromTheListBasedOnSpecificPoint(position.next(direction));
		}
		if (theGame->isNumOnBoard(position.next(direction))) // there is number 
		{
			theGame->insertCharInsteadOfNumForItem(position.next(direction));
		}
		position.draw(' ', sign);
		theGame->insertCharToBoard(position.getX(), position.getY(), ' ');

		position.move(direction);
		setTextColor(color);  // set the item color for printing on the screen
		position.draw(sign, sign);// in the place draw the item
		theGame->insertCharToBoard(position.getX(), position.getY(), sign);//update the board 
		setTextColor(Color::LIGHTGREY); // change color back to normal
		return true;
	}
}
bool NumEaters::getDeadStatus(){ return isDead; }
bool NumEaters::operator==(const NumEaters& other) const
{
	return other.position == position;
}
bool NumEaters::move()
{
	theGame->updateListOfNums();//delete numbers that not exist at the game

	if ((theGame->numsOnBoardGetList().size()) > 0 && isDead == false)
	{
		Point MinDistance = *(theGame->numsOnBoardGetList().begin());
		for (Point temp : theGame->numsOnBoardGetList())//find the point of the nearst number
		{
			if (theGame->isThisCloser(position, MinDistance, temp))
				MinDistance = temp;
		}

		if (MinDistance.getX() < position.getX())
			direction = LEFT;

		else if (MinDistance.getX() > position.getX())
			direction = RIGHT;

		else if (MinDistance.getY() < position.getY())
			direction = UP;

		else if (MinDistance.getY() > position.getY())
			direction = DOWN;

		if (theGame->IsOkNextOfNumEaters(position.next(direction)))
		{
			if (theGame->isNumOnBoard(position.next(direction))) // there is number 
			{
				theGame->insertCharInsteadOfNumForItem(position.next(direction));
			}
			position.draw(' ', sign);
			theGame->insertCharToBoard(position.getX(), position.getY(), ' ');

			position.move(direction);
			setTextColor(color);  // set the item color for printing on the screen
			position.draw(sign, sign);// in the place draw the item
			theGame->insertCharToBoard(position.getX(), position.getY(), sign);//update the board 
			setTextColor(Color::LIGHTGREY); // change color back to normal
			return true;
		}
		else
		{
			position.draw(' ', sign);
			theGame->insertCharToBoard(position.getX(), position.getY(), ' ');
			isDead = true;
			return false;
		}

	}
	else
	{
		direction = STOP;
		return true;
	}
}