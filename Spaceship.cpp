#include "TheMathGame.h"

void Spaceship::setPosition (int y, int x) // sets the position of the head
{
	head.set(x, y);
}
void Spaceship::addPointToList(int y, int x)
{
	templst.push_back(Point(x, y)); // add this point to list, we want to check the rotation status
}
void Spaceship::findRotationStatusFromList()
{
	int loopCounter=0;
	int xforcheck,tempx;
	

	xforcheck = -1; // just for check, we don't want to check the first and second iteration of the for loop 
	for (Point p : templst)
	{
		if (loopCounter == 0)
			setPosition(p.getY(), p.getX()); // this is the top left point! we set the head to this point!
		
		loopCounter++; // i want to check the 3rd iteration
		tempx = xforcheck; // set the temp to be the point in the prev node 
		xforcheck = p.getX();// get the current x of the node.
		if (loopCounter == 3) // when we are in the 3rd iteration
		{
			if (xforcheck > tempx) // checking if it is vertical or horizontal
				setRotationStatusFromTheStart('N');
			else
				setRotationStatusFromTheStart('Y');
		}
	}
}
bool Spaceship::checkIfRotationIsPossible()const // this func is very important , it checks if there is no walls and the rotation is possible
{
	Point temp;
	if (sign == 'S') // if it is the Small Spaceship
	{
			for (int i = 0; i < 3; i++) // the small spaceship has only 3 chars 
			{
				if (rotateStatus == 'N') 
					temp.set(head.getX(), head.getY() + i);

				if (rotateStatus == 'Y')
					temp.set(head.getX() + i, head.getY() );

				if (theGame->isWall(temp)) // checks if in the future place if we will do rotation there are no walls
					return false;
				
				if (theGame->isItemInTheWay(temp)) // checks if there is item in the future place 
					return false;
			}
	}
	if (sign == 'B') // if it is the big spaceship
	{
		for (int i = 0; i < 3; i++) // 3 cols (if rotated - 3 rows)
		{
			for (int j = 0; j < 2; j++) //2 rows (if rotated - 3 cols)
			{
				if (rotateStatus == 'N')
					temp.set(head.getX() + j, head.getY() + i);

				if (rotateStatus == 'Y')
					temp.set(head.getX() + i, head.getY() + j);

				if (theGame->isWall(temp)) //same
					return false;
				 
				if (theGame->isItemInTheWay(temp))//same
					return false;
			}
		}
	}
	return true;
}
void Spaceship::setRotationStatus()
{
	Point tempforcheck;

	if (checkIfRotationIsPossible())
	{
		insertSpacesForRotation(); // updates the board and cleans the screen the points that had spaceship before rotation

		if (sign == 'S')
		{
			if (rotateStatus == 'N')
			{
				for (int i = 0; i < SPACESHIP_COLS; i++)
				{
					tempforcheck.set(head.getX()  , head.getY()+i); // set the point to check

					if (theGame->isFail(tempforcheck)) // if there is a number in the future place this means lives-1
					{
						theGame->insertCharToBoard((tempforcheck).getX(), (tempforcheck).getY(), ' ');
						theGame->decreaseLives();
						theGame->printCurrentLives();
					}
				}
				rotateStatus = 'Y'; // change the status
			}

			else if (rotateStatus == 'Y') // same
			{
				for (int i = 0; i < SPACESHIP_COLS; i++)
				{
					tempforcheck.set(head.getX() + i, head.getY());

					if (theGame->isFail(tempforcheck))
					{
						theGame->insertCharToBoard((tempforcheck).getX(), (tempforcheck).getY(), ' ');
						theGame->decreaseLives();
						theGame->printCurrentLives();
					}
				}
				rotateStatus = 'N';
			}
		}
		if (sign == 'B') // same for big spaceship
		{
			if (rotateStatus == 'N')
			{
				for (int i = 0; i < SPACESHIP_ROWS; i++)
				{
					for (int j = 0; j < SPACESHIP_COLS; j++)
					{
						tempforcheck.set(head.getX() + j, head.getY() + i);

						if (theGame->isFail(tempforcheck))
						{
							theGame->insertCharToBoard((tempforcheck.next(direction)).getX(), (tempforcheck.next(direction)).getY(), ' ');
							theGame->decreaseLives();
							theGame->printCurrentLives();
						}
					}
				}
						
				rotateStatus = 'Y';
			}

			else if (rotateStatus == 'Y') // same
			{
				for (int i = 0; i < SPACESHIP_ROWS; i++)
				{
					for (int j = 0; j < SPACESHIP_COLS; j++)
					{
							tempforcheck.set(head.getX() + j, head.getY() + i);

						if (theGame->isFail(tempforcheck))
						{
							theGame->insertCharToBoard(tempforcheck.next(direction).getX(), tempforcheck.next(direction).getY(), ' ');
							theGame->decreaseLives();
							theGame->printCurrentLives();
						}
					}
				}
				rotateStatus = 'N';
			}
		}
	}
}
void Spaceship::move() 
{
	if (sign == 'S') // small spaceship
	{
		head.draw(' ', sign,rotateStatus); // prints the spaces before printing the spaceship again
		for (int i = 0; i < SPACESHIP_COLS; i++) // this loop updates the board to be the same as the output - screen
		{
			if (rotateStatus == 'N')
				theGame->insertCharToBoard(head.getX()+i, head.getY(), ' ');
			else if (rotateStatus == 'Y')
				theGame->insertCharToBoard(head.getX(), head.getY()+i, ' ');
		}
		
		Point tempForCheck; // this point checks us the next point by the direction
		
		for (int i = 0; i < SPACESHIP_COLS; i++)
		{
			if (rotateStatus == 'N')
				tempForCheck.set(head.getX() + i, head.getY());

			if (rotateStatus == 'Y')
				tempForCheck.set(head.getX(), head.getY() + i);
			
			if (theGame->theNextIsCreature(tempForCheck.next(direction)))
			{
				theGame->decreaseLives(); // lives -- 
				theGame->SetItemHitSpaceshipToTrue();
				theGame->init(); // init the board to start over 
				theGame->SetItemHitSpaceshipToFalse();
				break;
			}
			
			if (theGame->solutionFound(tempForCheck.next(direction))) // checks if walked on solution
				theGame->changeIsLevelDoneStatus(); // level has done

			if (theGame->isWall(tempForCheck.next(direction))) // checks if walked on wall
				direction = STOP; // if yes stop!

			if (theGame->isSpaceshipAhead(tempForCheck.next(direction), sign)) // checks if there is spaceship collision
				direction = STOP; // if yes stop

			if (theGame->isItemInTheWay(tempForCheck.next(direction))) // checks if the next position is item 
			{
				if (direction == DOWN) // if the spaceship is moving down and the item is beneath - STOP
					direction = STOP;
				else 
				{

					bool chck = theGame->moveItemWithSpaceship(direction, sign, tempForCheck.next(direction)); // move item with Spaceship
					if (chck == false)
						direction = STOP;
				}
			}

			if (theGame->isFail(tempForCheck.next(direction))) // if walked on num that is not a solution
			{
				theGame->insertCharInsteadOfNumForItem(tempForCheck.next(direction));
				theGame->decreaseLives(); // lives -1 because we steped on not good number 
				theGame->printCurrentLives(); // prints the current lives in the dedicated place
			}
		}

		head.move(direction); // after all the ifs now the point will move to the next direction
		setTextColor(color); 
		head.draw('@', sign, rotateStatus);//prints the spaceship on the screen
		for (int i = 0; i < SPACESHIP_COLS; i++) // updates the board! based on rotation status and position on screen
		{
			if (rotateStatus == 'N')
				theGame->insertCharToBoard(head.getX() + i, head.getY(), '@');
			else if (rotateStatus == 'Y')
				theGame->insertCharToBoard(head.getX(), head.getY() + i, '@');
		}

		setTextColor(Color::LIGHTGREY); // set the text color back to normal after printing on the screen the spaceship
	}

	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	// THESE FUNCTIONS HAVE THE SAME DESCRIPTION AS THE SMALL SPACESHIP
	//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	if (sign == 'B') // big spaceship
	{
		head.draw(' ', sign, rotateStatus);
		for (int i = 0; i < SPACESHIP_ROWS; i++)
		{
			for (int j = 0; j < SPACESHIP_COLS; j++)
			{
				if (rotateStatus == 'N')
					theGame->insertCharToBoard(head.getX() + j, head.getY() + i, ' ');
				else if (rotateStatus == 'Y')
					theGame->insertCharToBoard(head.getX() + i, head.getY() + j, ' ');
			}
		}
		Point tempForCheck;

		for (int i = 0; i < SPACESHIP_ROWS; i++)
		{
			for (int j = 0; j < SPACESHIP_COLS; j++)
			{
				if (rotateStatus == 'N')
					tempForCheck.set(head.getX() + j, head.getY() + i);
				if (rotateStatus == 'Y')
					tempForCheck.set(head.getX() + i, head.getY() + j);

			if (theGame->theNextIsCreature(tempForCheck.next(direction)))
				{
					theGame->decreaseLives(); // lives -- 
					theGame->SetItemHitSpaceshipToTrue();
					theGame->init(); // init the board to start over 
					theGame->SetItemHitSpaceshipToFalse();
					break;
				}

				if (theGame->solutionFound(tempForCheck.next(direction)))
					theGame->changeIsLevelDoneStatus();
				
				if (theGame->isWall(tempForCheck.next(direction)))
					direction = STOP;

				if (theGame->isItemInTheWay(tempForCheck.next(direction)))
				{
					if (direction == DOWN) 
						direction = STOP;
					else
					{
						bool chck = theGame->moveItemWithSpaceship(direction, sign, tempForCheck.next(direction)); // move item with Spaceship
						if (chck == false)
							direction = STOP;
					}
				}

				if (theGame->isSpaceshipAhead(tempForCheck.next(direction), sign))
					direction = STOP;

				if (theGame->isFail(tempForCheck.next(direction)))
				{
					theGame->insertCharInsteadOfNumForItem(tempForCheck.next(direction));
					theGame->decreaseLives();
					theGame->printCurrentLives();
				}
			}
		}

		head.move(direction);
		setTextColor(color);
		head.draw('#', sign, rotateStatus);
		for (int i = 0; i < SPACESHIP_ROWS; i++)
		{
			for (int j = 0; j < SPACESHIP_COLS; j++)
			{
				if (rotateStatus == 'N')
					theGame->insertCharToBoard(head.getX() + j, head.getY() + i, '#');
				else if (rotateStatus == 'Y')
					theGame->insertCharToBoard(head.getX() + i, head.getY() + j, '#');
			}
		}
		setTextColor(Color::WHITE);
	}
}
void Spaceship::insertSpacesForRotation() // when doing rotation we need to delete the parts of the spaceship before the rotation 
{
	if (rotateStatus == 'N')
	{
		if (sign == 'S')
		{
			for (int i = 1; i < SPACESHIP_COLS; i++) // insert spaces for the small spaceship 
			{
				gotoxy(head.getX() + i, head.getY());// prints on the screen
				cout << ' ';
				theGame->insertCharToBoard(head.getX() + i, head.getY(), ' '); // updates the board
			}
		}
		if (sign == 'B')// insert spaces for the big spaceship 
		{
			for (int i = 1; i < SPACESHIP_COLS; i++)
			{
				for (int j = 0; j < SPACESHIP_COLS; j++)
				{
					gotoxy(head.getX() + i, head.getY() + j);// prints on the screen
					cout << ' ';
					theGame->insertCharToBoard(head.getX() + i, head.getY() + j, ' ');// updates the board
				}
			}
		}
	}
	if (rotateStatus == 'Y')
	{
		if (sign == 'S')// insert spaces for the small spaceship 
		{
			for (int i = 1; i < SPACESHIP_COLS; i++)
			{
				gotoxy(head.getX(), head.getY() + i);// prints on the screen
				cout << ' ';
				theGame->insertCharToBoard(head.getX(), head.getY() + i, ' ');// updates the board
			}
		}
		if (sign == 'B')// insert spaces for the big spaceship 
		{
			for (int i = 1; i < SPACESHIP_COLS; i++)
			{
				for (int j = 0; j < SPACESHIP_COLS; j++)
				{
					gotoxy(head.getX() + j, head.getY() + i); // prints on the screen
					cout << ' ';
					theGame->insertCharToBoard(head.getX() + j, head.getY() + i, ' ');// updates the board
				}
			}
		}
	}

}
int Spaceship::getDirection(const list<char>& keyHits)const 
{
	for (char key : keyHits)
	{
		for (int i = 0; i < 6; ++i) 
		{
			if (key == arrowKeys[i]) 
				return i;
		}
	}
	return -1;
}
int Spaceship::getDirectionForReplay(char c, char sign)const
{
	if (c == '~')
		return -1;

	else if (sign == 'B')
	{
		char str[] = "adwxsz";

		for (int i = 0; str[i] != '\0';i++)
		{
			if (str[i] == c)
				return i;
		}

	}
	else if (sign == 'S')
	{
		char str[] = "jlimkn";

		for (int i = 0; str[i] != '\0';i++)
		{
			if (str[i] == c)
				return i;
		}
	}
}