//#include "Creatures.h"
#include "TheMathGame.h"
#include "question.h"
#include <fstream>
#include <string> 
#include <math.h>

void TheMathGame::setBoard(const char* name)
{
	
	RemoveSpaceshipsData(); //clearing the list for the spaceships
	FreeListOfPointsOfEverything(); // same for item
	listOfKeyHits.clear(); // same for key hits
	readScreenFromFile(name); 

	// this is a one time job, same as setBoard

	list<Item>::iterator itr;

	bigS.setArrowKeys("adwxsz");
	smallS.setArrowKeys("jlimkn");
	bigS.setColor(Color::YELLOW);
	smallS.setColor(Color::LIGHTRED);
	for (itr = it.begin(); itr != it.end();itr++)
		itr->setItemColor(Color::LIGHTGREEN);
	bigS.setGame(this);
	smallS.setGame(this);
	//for (itr = it.begin(); itr != it.end();itr++)
		//itr->setgame(this);
}
void TheMathGame::setBoard(string name)
{
	
	RemoveSpaceshipsData(); // same as setboard
	FreeListOfPointsOfEverything();
	listOfKeyHits.clear();
	readScreenFromFile(name);
	
	// this is a one time job, same as setBoard
	list<Item>::iterator itr;

	bigS.setArrowKeys("adwxsz");
	smallS.setArrowKeys("jlimkn");
	bigS.setColor(Color::YELLOW);
	smallS.setColor(Color::LIGHTRED);
	for (itr = it.begin(); itr != it.end();itr++)
		itr->setItemColor(Color::LIGHTGREEN);
	bigS.setGame(this);
	smallS.setGame(this);
	//for (itr = it.begin(); itr != it.end();itr++)
	//	itr->setgame(this);
}
void TheMathGame::setBoard2(const char* name)
{
	
	FreeListOfPointsOfEverything(); // same as set board 2 the difference is the parameter
	RemoveSpaceshipsData();
	listOfKeyHits.clear();
	readScreenFrom_save_File(name);

	// this is a one time job, same as setBoard
	list<Item>::iterator itr;

	bigS.setArrowKeys("adwxsz");
	smallS.setArrowKeys("jlimkn");
	bigS.setColor(Color::YELLOW);
	smallS.setColor(Color::LIGHTRED);
	for (itr = it.begin(); itr != it.end();itr++)
		itr->setItemColor(Color::LIGHTGREEN);
	bigS.setGame(this);
	smallS.setGame(this);
	//for (itr = it.begin(); itr != it.end();itr++)
	//	itr->setgame(this);
}
void TheMathGame::init()
{
	clear_screen();
	initBoard();
	printTargil();
	printStatus();

}
void TheMathGame::initBoard()
{
	// copy original board to actual board and build all the relevant objects
	// this shall be called any time we Start the level
	if (isAutoReplay == false)
		q.initQ();

	setDirectionToStopForReplayLevel();
	auto iter = it.begin();
	for (iter = it.begin(); iter != it.end();iter++)
		iter->freeListOfPoints();
	it.clear();
	numeaters.clear();
	rowflyers.clear();
	colflyers.clear();
	numsOnBoard.clear();

	for (int i = 0; i < ROWS;++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			board[i][j] = originalBoard[i][j];
		}
	}
	for (int i = 0; i < ROWS;++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			if (board[i][j] == '*')
			{
				handleObjectCreationFromBoard(i, j);
				break;
			}
		}
	}
	flagForItem = false;

	for (int i = 0; i<ROWS;)
	{
		for (int j = 0; j<COLS; ++j)
		{

			iter = it.begin();
			if (originalBoard[i][j] != '$' && originalBoard[i][j] != '!' && originalBoard[i][j] != '%' && originalBoard[i][j] != '+' && originalBoard[i][j] != ' ' && originalBoard[i][j] != '@' && originalBoard[i][j] != '#' && originalBoard[i][j] != '*' && originalBoard[i][j] != 'S' && originalBoard[i][j] != 'X')
			{
				if (originalBoard[i][j]<'0' || originalBoard[i][j]>'9')
					originalBoard[i][j] = '+';
			}

			if (originalBoard[i][j] == '+') // set different color for the wall
				setTextColor(Color::LIGHTCYAN);

			if (originalBoard[i][j] == '*')
				setTextColor(iter->getColor());

			if (originalBoard[i][j] == '!')
				setTextColor(Color::LIGHTMAGENTA);

			if (originalBoard[i][j] == '$')
				setTextColor(Color::LIGHTMAGENTA);


			cout << originalBoard[i][j];
			setTextColor(Color::LIGHTGREY);//set the color back to normal
			cout.flush();

			if (originalBoard[i][j] != '*')
				handleObjectCreationFromBoard(i, j);

		}

		board[i][COLS] = '\0';
		++i;
	}
	for (int i = 0; i < ROWS;++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			board[i][j] = originalBoard[i][j];
		}
	}
	smallS.findRotationStatusFromList(); // here we want to find what is the starting rotation status 
	bigS.findRotationStatusFromList(); // same as above
	for (auto i = colflyers.begin(); i != colflyers.end(); i++)
	{
		i->setGame(this);
	}
	for (auto i = rowflyers.begin(); i != rowflyers.end(); i++)
	{
		i->setGame(this);
	}
	for (auto i = numeaters.begin(); i != numeaters.end();i++)
	{
		i->setGame(this);
	}
	flagForBig = false; flagForSmall = false;
}
void TheMathGame::handleObjectCreationFromBoard(int row, int col)
{
	char ch = board[row][col];

	if (ch == '@')
	{
		if (flagForSmall == false)
		{
			smallS.setPosition(row, col); // this is the head of the spaceship
			flagForSmall = true;
		}
		smallS.addPointToList(row, col);
	}

	else if (ch == '#')
	{
		if (flagForBig == false)
		{
			bigS.setPosition(row, col);// this is the head of the spaceship
			flagForBig = true;
		}
		bigS.addPointToList(row, col);
	}


	else if (ch == '*' && flagForItem == false)
	{
		//it.setItemPosition(row, col); // - this was before changing to multiple items

		// we need to call a function here that collects all the points of item from the tempboard and not the original board.
		setTheItemsList();
		flagForItem = true;
	}
	else if (ch == '%')
	{
		NumEaters temp;
		temp.setPosition(row, col);
		numeaters.push_back(temp);
	}
	else if (ch == '$')
	{
		ColFlyers temp;
		temp.setPosition(row, col);
		colflyers.push_back(temp);
	}

	else if (ch == '!')
	{
		RowFlyers temp;
		temp.setPosition(row, col);
		rowflyers.push_back(temp);
	}
	else if (ch == 'X')
	{
		if (isAutoReplay == false && isReplayLevel == false)
			handleTargil(row, col);

	}

	else if (ch >= '0' && ch <= '9')
	{
		numsOnBoard.push_back(Point(col, row));
	}

	else if (ch == 'S')
		handleState(row, col);
}
void TheMathGame::initClock()
{
	if (isReplayFromSavedGame == false)
	{
		st.initClock();
		clock = 0;
	}
	else
	{
		st.getClock(savedGameClock);
		clock = savedGameClock;
	}

}
void TheMathGame::decreaseLives() // change the lives - because walked on num or item hit the spaceship
{
	lives--;
	if (lives == 0)
		levelDone = true;
	printCurrentLives();
}
void TheMathGame::setLivesTo3() // set lives back to 3 if we want to play the same level again
{
	lives = 3;
}
void TheMathGame::changeIsLevelDoneStatus() // changes is level done to true
{
	levelDone = true;
	currentLevel++;
	lives = 3;
}
void TheMathGame::setDirectionToStopForReplayLevel()
{
	bigS.setDirection(4);// stop = 4
	smallS.setDirection(4); // stop = 4
}
void TheMathGame::setTheItemsList()
{
	list<Item>::iterator iter;

	for (int i = 0; i < ROWS; i++) // running on the board after removing the item(s)
	{
		for (int j = 0; board[i][j] != '\0';j++)
		{
			if (board[i][j] == '*') // after we removed an item we need to check if there is another one. if there is it means there are multiple items
			{
				it.push_back(Item());
				iter = it.end();
				iter--;
				CheckTheListOfItem(Point(j, i), *(iter->getlist()));
				iter->setItemColor(Color::LIGHTGREEN);
				iter->setVolume();
				iter->setgame(this);
			}
		}
	}
}
void TheMathGame::setListOfRecord()
{
	char fname[256], myline[256];

	ifstream MyFile;
	if (listOfRecord.size() != 0) // if we already load the list of records we need to release them - they belong to the previous replay
		releaseListOfRecord();

	getTheNameToTheRecordFile(fname);
	MyFile.open(fname);

	for (int r = 0; r < 3; r++) // skip the first 3 lines;
		MyFile.getline(myline, 256);

	while (!MyFile.eof())
	{
		MyFile.getline(myline, 256);
		insertToEndListTheData(myline);
	}
	MyFile.close();
}
void TheMathGame::updateListOfNums()
{

	for (size_t i = 0; i < numsOnBoard.size(); i++)
	{
		for (Point temp : numsOnBoard)
		{
			if (board[temp.getY()][temp.getX()]<'0' || board[temp.getY()][temp.getX()] > '9')
			{
				numsOnBoard.remove(temp);
				break;
			}
		}
	}
	/*
	for (auto iter = numsOnBoard.begin(); iter != numsOnBoard.end(); iter++)
	{
	if (board[iter->getY()][iter->getX()]<'0' || board[iter->getY()][iter->getX()] > '9')
	{
	numsOnBoard.remove(*iter);
	}
	}
	*/
}
void TheMathGame::FreeListOfPointsOfEverything()
{
	list<Item>::iterator itr;

	for (list<Item>::iterator itr = it.begin();itr != it.end();itr++)
	{
		itr->freeListOfPoints();
	}
	numeaters.clear();
	rowflyers.clear();
	colflyers.clear();

}
void TheMathGame::ForNextGamMe()  // set conditions to play again
{
	//currentLevel = 1;
	setLivesTo3();
	levelDone = false;
}
void TheMathGame::resetForAutoRepaly()
{
	// here we are doing reset for the replay
	setDirectionToStopForReplayLevel();
	FreeListOfPointsOfEverything();
	SetTargilForAutoReplay();
	setLivesTo3();
	initClock();
}

void TheMathGame::printCurrBoard() // prints the current board to the screen 
{
	list<Item>::iterator iter;
	iter = it.begin();
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLS; ++j)
		{
			if (board[i][j] == '+')
				setTextColor(Color::LIGHTCYAN);
			if (board[i][j] == '*')
				setTextColor(iter->getColor());
			cout << board[i][j];
			setTextColor(Color::LIGHTGREY);

		}

	printStatus();
	printTargil();
}
void TheMathGame::printCurrentClock() // prints the clock in the dedicated place
{
	gotoxy(st.GetPos().getY() + 12, st.GetPos().getX());
	for (int i = 0; i < 3; i++)
		cout << " ";

	gotoxy(st.GetPos().getY() + 12, st.GetPos().getX());
	setTextColor(Color::LIGHTMAGENTA);
	cout << st.getClock();
	setTextColor(Color::LIGHTGREY);
}
void TheMathGame::printTargil() // print the question in the right place
{
	//if (q.getFlag() == true)
	{

		gotoxy(q.GetPos().getY() - 1, q.GetPos().getX());
		cout << " ";
		for (int i = q.GetPos().getY() - 1; i < q.GetPos().getY() + 15; i++)
		{
			gotoxy(q.GetPos().getY() + i, q.GetPos().getX());
			cout << " ";

		}

		gotoxy(q.GetPos().getY(), q.GetPos().getX());

		if (q.getLeft() == -1)
			cout << "_ ";
		else
			cout << q.getLeft();

		cout << q.Op() << " ";

		if (q.getRight() == -1)
			cout << "_ ";
		else
			cout << q.getRight();

		cout << " = ";

		if (q.getRes() == -1)
			cout << "_ ";
		else
			cout << q.getRes();

	}
}
void TheMathGame::printCurrentLives() // prints the current lives in the dedicated place
{
	printStatus();
}

void TheMathGame::getDir(string d, list<string> & f)
{
	// in this function we get all the requested files from a folder
	FILE* pipe = NULL;
	//windows cmd command
	string pCmd = "dir /b " + d;
	char buf[256];
	size_t ln;
	if (NULL == (pipe = _popen(pCmd.c_str(), "rt")))
	{
		cout << "Shit" << endl;
		return;
	}
	while (!feof(pipe))
	{
		if (fgets(buf, 256, pipe) != NULL)
		{
			ln = strlen(buf) - 1;
			//eliminate new line
			if (buf[ln] == '\n')
				buf[ln] = '\0';
			f.push_back(string(buf));
		}
	}
	_pclose(pipe);
}
void TheMathGame::getTheNameToTheRecordFile(char name1[256])const
{
	char name[256] = "Record_";
	char screenid[256];
	int numofdigits = 0, tempid = id;


	while (tempid > 0) // finding the num of digits
	{
		tempid = tempid / 10;
		numofdigits++;
	}
	screenid[numofdigits] = '\0';
	tempid = id;
	int temp;
	while (numofdigits > 0)
	{
		numofdigits--;
		temp = tempid % 10;
		screenid[numofdigits] = temp + '0'; // setting the screen id
		tempid = tempid / 10;
	}


	strcat_s(name, screenid);
	strcat_s(name, ".spd"); // setting the name with the spd

	int j = 0;
	while (name[j] != '\0')
	{
		name1[j] = name[j];
		j++;
	}
	name1[j] = '\0';

}
char TheMathGame::getKeyBasedOnDirection(char sign, int dir)const
{
	//returning the correct key for the replay based on the arrows of each spaceship
	char Skeys[] = "jlimkn"; // chars of small spaceship
	char Bkeys[] = "adwxsz";// chars of big spaceship

	if (sign == 'S')
		return Skeys[dir];

	else 
		return Bkeys[dir];

}
int TheMathGame::getClockIterationsFromFile(const char line[256])const
{
	int res = 0;

	for (int i = 16; line[i] != '\0'; i++) // this func gets the clock iterations from the record file or the saved game
	{
		res = res * 10 + line[i] - '0';
	}
	return res;

}
int TheMathGame::getWholeNumFromBoard(const Point& p)
{
	int x = p.getX();
	int y = p.getY();
	int res = 0;
	int digit;
	int counter = 0;

	while (board[y][x] >= '0' && board[y][x] <= '9')
	{
		digit = board[y][x] - '0';
		res = res * 10 + digit;
		x++;
		counter++;
	}
	x = p.getX() - 1;
	y = p.getY();
	while (board[y][x] >= '0' && board[y][x] <= '9')
	{
		digit = board[y][x] - '0';
		res = (int)(digit * pow(10, counter) + res);
		x--;
		counter++;
	}
	return res;
}
int TheMathGame::getScreenIdFromFile(const char* line)
{
	int sizeOfLine = strlen(line), res = 0;

	for (int i = 9; i < sizeOfLine; ++i) // running on each char and the result is the int number
		res = res * 10 + line[i] - '0';

	return res;
}
int TheMathGame::getScreenIdFromFile(string line)
{
	// same with different parameters
	int sizeOfLine = (line).size(), res = 0;

	for (int i = 9; i < sizeOfLine; ++i)
		res = res * 10 + line[i] - '0';

	return res;
}
int TheMathGame::getScreenIdFromFile2(string s)
{
	//here we opening a file with the name s, and fetting the correct screen id
	int res;

	char line[256];
	ifstream file;
	file.open(s);

	file.getline(line, 256);

	res = getScreenIdFromFile(line);
	file.close();

	return res;
}
int TheMathGame::getNumberfromChars(const char* line, int& i)
{
	//running on the chars ang getting the correct number
	int res = 0;

	while (line[i] != '+' && line[i] != '-' && line[i] != '*' && line[i] != '/' && line[i] != '\0' &&  line[i] != '=')
	{
		res = res * 10 + line[i] - '0';
		i++;
	}
	return res;
}
bool TheMathGame::getAutoReplayFlag()const
{
	return isAutoReplay;
}
bool TheMathGame::isSpaceshipAhead(const Point& p, char sign)const // this func checks if the current point is part of a spaceship
{
	if (sign == 'S') // if it is the small spaceship 
		return board[p.getY()][p.getX()] == '#'; // checks if it is the big spaceship

	if (sign == 'B')// if it is the big spaceship 
		return board[p.getY()][p.getX()] == '@';// checks if it is the small spaceship

	else
		return ((board[p.getY()][p.getX()] == '#') || (board[p.getY()][p.getX()] == '@')); // checks if there is some kind of spaceship 

	
}
bool TheMathGame::OnlyOneIsKnown()const
{
	if (q.getLeft() == -1 && q.getRight() == -1)
		return true;
	else if (q.getLeft() == -1 && q.getRes() == -1)
		return true;
	else if (q.getRes() == -1 && q.getRight() == -1)
		return true;
	else
		return false;

}
bool TheMathGame::solutionFound(const Point& p) // this func checks if the Point has the number that cracks the question
{
	int left, right, res, num;
	bool Bleft, Bright, Bres;

	left = q.getLeft();
	right = q.getRight();
	res = q.getRes();

	Bleft = q.getLeftFlag();
	Bright = q.getRightFlag();
	Bres = q.getEqFlag();

	if ((board[p.getY()][p.getX()] >= '0' && board[p.getY()][p.getX()] <= '9'))
	{
		num = getWholeNumFromBoard(p);

		if (!OnlyOneIsKnown())
		{
			if (thisIsSolution(num))
			{
				insertCharInsteadOfNumForItem(p);
				return true;
			}
			else
			{
				insertCharInsteadOfNumForItem(p);

				if (q.numOfKnownElems() == 1)
				{

					if (Bleft == false)
						q.setLeft(num);

					else if (Bright == false)
						q.setRight(num);
				}
				printTargil();

				return false;
			}
		}


		else if (OnlyOneIsKnown())
		{


			if (q.getLeft() == -1)
				q.setLeft(num);

			else if (q.getRight() == -1)
				q.setRight(num);

			else if (q.getRes() == -1)
				q.setRes(num);

			printTargil();
			insertCharInsteadOfNumForItem(p);
			return false;
		}

	}
	else
		return false;
}
bool TheMathGame::thisIsSolution(int num)
{
	int left, right, res;
	char op1;

	op1 = q.Op();
	left = q.getLeft();
	right = q.getRight();
	res = q.getRes();

	if (left == -1)
	{
		if (op1 == '+') // if the question is +
		{
			if (num + right == res)
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

		 if (op1 == '-') // // if the question is -
		{
			if ((num - right == res))
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

		 if (op1 == '*')// if the question is * 
		{
			if (num * right == res)
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

		 if (op1 == '/') // if the question is /
		{
			if (((num / right == res) && (num%right == 0)))
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}
	
	}

	if (right == -1)
	{
		if (op1 == '+') // if the question is +
		{
			if (num + left == res)
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

		else if (op1 == '-') // // if the question is -
		{
			if ((left - num == res))
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

		else if (op1 == '*')// if the question is * 
		{
			if (num * left == res)
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

		else if (op1 == '/') // if the question is /
		{
			if (((left / num == res) && (left%num == 0)))
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

	}
	if (res == -1)
	{
		if (op1 == '+') // if the question is +
		{
			if (right + left == num)
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

		else if (op1 == '-') // // if the question is -
		{
			if ((left - right == num))
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

		else if (op1 == '*')// if the question is * 
		{
			if (right * left == num)
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}

		else if (op1 == '/') // if the question is /
		{
			if (((left / right == num) && (left%right == 0)))
				return true;
			else
			{
				decreaseLives();
				return false;
			}
		}
	}
}
bool TheMathGame::moveItemWithSpaceship(int SpaceshipDirection, char sign, const Point& p) // this func calls to move item when spaceship push it 
{
	list<Item>::iterator iter;

	//findTheCorrectItemFromTheListByPoint(p, iter);
	int counter = 0;
	bool flag = false;
	while (flag==false)
	for (iter = it.begin(); (iter != it.end()) && (flag == false);iter++)
	{
		for (Point _p : *(iter->getlist()))
		{
			if (_p == p)
			{
				flag = true;
				break;
			}
		}
		counter++;
	}
	iter = it.begin();
	advance(iter, counter-1);



	if (sign == 'B')
	{
		if (iter->theNextOfItemIsWall(SpaceshipDirection) || iter->getVolume() > 4)
		{
			iter->setItemDirection(3);
			return false;
		}
		else
		{
			iter->setItemDirection(SpaceshipDirection); // set the direction that the item will go (same as spaceship)
			iter->moveItem(); // move the item
			return true;
		}
	}
	else// (sign == 'S')
	{

		if (iter->theNextOfItemIsWall(SpaceshipDirection) || iter->getVolume() > 2)
		{
			iter->setItemDirection(3);
			return false;
		}
		else
		{
			iter->setItemDirection(SpaceshipDirection); // set the direction that the item will go (same as spaceship)
			iter->moveItem(); // move the item
			return true;
		}
	}

}
bool TheMathGame::moreFauilre()
{
	return lives > 0;
}
bool TheMathGame::isLevelDone1()const // return True or False
{
	return levelDone;
}
bool TheMathGame::hasNextLevel()const // checks if has next level 
{
	return currentLevel <= numOfLevels;
}
bool TheMathGame::checkIsDoubleScreenID(const char*fname, int idToCheck)
{
	//this function is part of the screen checker and checks if there is more than one file with the same id
	list<string> files;
	getDir("*spg*.*", files);
	string TheDup;
	bool res = true;

	string Fname(fname);
	cout << "hello" << endl;
	for (string str : files)
	{
		if (idToCheck == getScreenIdFromFile2(str) && str != Fname)
		{
			res = false;
			TheDup = str; // this is the file that contains the same id
			break;
		}

	}
	if (!res)
	{
		cout << "Error! there is another file using the same screenID, the file is: " << TheDup << endl;

		Sleep(1500);
	}


	return res;

}
bool TheMathGame::checkIsDoubleScreenID(string fname, int idToCheck)
{
	//same but with different parameters
	list<string> files;
	getDir("*spg*.*", files);
	string TheDup;
	bool res = true;

	for (string str : files)
	{
		if (idToCheck == getScreenIdFromFile2(str) && str != fname)
		{
			res = false;
			TheDup = str;
			break;
		}
	}
	if (!res)
	{
		cout << "Error! there is another file using the same screenID, the file is: " << TheDup << endl;
		Sleep(1500);
	}

	return res;
}
bool TheMathGame::screenChecker(string filename)
{
	//this func is the screen checker
	clear_screen();
	ifstream file;
	char MyLine[256], boardToCheck[ROWS + 256][COLS + 256];
	bool checkScreenID, checkDoubleScreenID, checkExercise, checkBoard;
	int rowCount = 0, temp_id;

	char *temptosend; // we need to send char * and not string
	temptosend = new char[filename.size() + 1];
	for (int i = 0;filename[i] != '\0';i++)
		temptosend[i] = filename[i];

	if (checkIfTheFileNameIsSup(temptosend)) // this func checks if the end of the name is .spg
	{
		delete[]temptosend;
		file.open(filename);
		if (file.is_open()) // if the file exist and we open it
		{
			file.getline(MyLine, 256);
			checkScreenID = checkIfScreenIDIsOK(MyLine); // getting the line of the screen id and checking it with a boolean func.

			if (checkScreenID == false)
				cout << "The ScreenID is incorect" << endl;
			else
			{
				temp_id = getScreenIdFromFile(MyLine);
				checkDoubleScreenID = checkIsDoubleScreenID(filename, temp_id); // checking if there is duplicate screen id
			}

			file.getline(MyLine, 256);
			checkExercise = checkIfExerciseIsOK(MyLine); // sending the excercise line to check

			if (checkExercise == false)
				cout << "therefore: The Exercise is incorect" << endl;

			while (!file.eof()) // this loop copies the board from the file to our board to check
			{
				file.getline(MyLine, 256);
				strcpy_s(boardToCheck[rowCount], MyLine);
				rowCount++;
			}
			checkBoard = checkTheBoard(boardToCheck, rowCount); // checking if the board is legal with this bool func.

			if (checkScreenID == true && checkExercise == true && checkBoard == true && checkDoubleScreenID == true) // if there are no problems return true
			{
				file.close();
				return true;

			}
			else
			{
				file.close();
				return false;
			}
		}
		else
		{
			cout << "FILE NOT FOUND!!!" << endl;
			return false;
		}
	}
	else
	{
		delete[]temptosend;
		cout << "the format of the file is not spg!" << endl;
		return false;
	}

}
bool TheMathGame::screenChecker(const char*filename)
{
	//same with different parameters
	clear_screen();
	ifstream file;
	char MyLine[256], boardToCheck[ROWS + 256][COLS + 256];
	bool checkScreenID, checkDoubleScreenID, checkExercise, checkBoard;
	int rowCount = 0, temp_id;

	if (checkIfTheFileNameIsSup(filename))
	{
		file.open(filename);
		if (file.is_open())
		{
			file.getline(MyLine, 256);
			checkScreenID = checkIfScreenIDIsOK(MyLine);

			if (checkScreenID == false)
				cout << "The ScreenID is incorect" << endl;
			else
			{
				temp_id = getScreenIdFromFile(MyLine);
				checkDoubleScreenID = checkIsDoubleScreenID(filename, temp_id);
			}

			file.getline(MyLine, 256);
			checkExercise = checkIfExerciseIsOK(MyLine);

			if (checkExercise == false)
				cout << "therefore: The Exercise is incorect" << endl;

			while (!file.eof())
			{
				file.getline(MyLine, 256);
				strcpy_s(boardToCheck[rowCount], MyLine);
				rowCount++;
			}
			checkBoard = checkTheBoard(boardToCheck, rowCount);

			if (checkScreenID == true && checkExercise == true && checkBoard == true && checkDoubleScreenID == true)
			{
				file.close();
				return true;
			}
			else
			{
				file.close();
				return false;
			}
		}
		else
		{
			cout << "FILE NOT FOUND!!!" << endl;
			return false;
		}
	}
	else
	{
		cout << "the format of the file is not spg!" << endl;
		return false;
	}
}
bool TheMathGame::checkIfTheFileNameIsSup(const char*name)const
{
	//this func checks if it is a spg file
	int temp, i;


	for (i = 0; name[i] != '\0';++i)
	{
		if (name[i] == '.')
			temp = i;
	}
	if (name[temp + 1] == 's' && name[temp + 2] == 'p' && name[temp + 3] == 'g')
		return true;
	else
		return false;
}
bool TheMathGame::checkIfScreenIDIsOK(const char* c)const
{
	//checking if the screen id is as the format
	int i = 0;
	char s[] = "ScreenID=";

	while (s[i] != '\0') // checking the string " ScreenID= "
	{
		if (s[i] != c[i])
			return false;
		i++;
	}

	if ((c[9] == '\0') || (c[9]<'0' || c[9]>'9')) // if there is no number of screen id return false
		return false;

	else
	{
		i = 9;
		while (c[i] != '\0') // run until the end of line
		{
			if (c[i] <'0' || c[i]>'9') // if this is not a number, return false;
				return false;
			i++;
		}
	}
	return true;
}
bool TheMathGame::checkIfExerciseIsOK(const char*c)const
{
	// this func. checks the excercise line
	int i = 0;
	char s[] = "Exercise=";
	bool flag1 = false, var1 = false, var2 = false, var3 = false;

	while (s[i] != '\0') // here we check the string " Excercise="
	{
		if (s[i] != c[i])
		{
			cout << "the string before the Exercise is not as the format, it should be: Exercise= ... \n";
			return false;
		}
		i++;
	}

	if (c[i] != '_') // checking the left var
	{
		flag1 = true;
		if (checkIfNumBeforeOperator(c, i) == false)
			return false;
		else
			var1 = true;
	}


	if (flag1 == false) // if the left var is "neelam"
	{
		i++;
		if (c[i] != '+' && c[i] != '-' && c[i] != '*' && c[i] != '/') // checking if after that there is operator
		{
			cout << "there is no legal operator\n";
			return false;
		}
	}
	flag1 = false;
	i++;
	if (c[i] != '_') // cecking the right var
	{
		flag1 = true;
		if (checkIfNumBeforeEq(c, i) == false)
			return false;
		else
			var2 = true;
	}

	if (flag1 == false) // checking if after the var there is = sign
	{
		i++;
		if (c[i] != '=')
		{
			cout << "Error, there is no equal sign (=)\n";
			return false;
		}
	}
	i++;
	if (c[i] != '_')
	{
		if (checkIfFinalNum(c, i) == false) // checking if there is number on the result
		{
			cout << "there is no result: not number and to variable\n";
			return false;
		}
		else
			var3 = true;
	}


	if (var1 == true && var2 == true && var3 == true) // if they are all true it means that there are no neelamim so the equation is solved
	{
		cout << "Error, there are no variables, the equation is already solved\n";
		return false;
	}

	return true;

}
bool TheMathGame::checkIfFinalNum(const char* c, int& i)const
{

	if (c[i]< '0' || c[i] >'9') // if there is no number there is a "neelam"
		return false;

	else
	{
		while (1)
		{
			if (c[i] >= '0' && c[i] <= '9') // if it is a number it is good continue
				i++;
			else if (c[i] != '\0') // if it is not number and not the end of the line something is wrong
				return false;
			else
				return true; // if we reached the end and everything was ok
		}
	}

	return true;
}
bool TheMathGame::checkIfNumBeforeOperator(const char*c, int&i)const
{
	if (c[i] <'0' || c[i] >'9') // if this isn't a number return false
	{
		cout << "there is no number or variable (_) before operator\n";
		return false;
	}

	else
	{
		while (1) // run until the end or unknown char
		{
			if (c[i] >= '0' && c[i] <= '9')
				i++;
			else if (c[i] == '+' || c[i] == '-' || c[i] == '*' || c[i] == '/')
				return true;
			else
			{
				cout << "the excercise equation is deffected, there are unknown chars before the operator" << endl;
				return false;
			}

		}
	}

	return false;
}
bool TheMathGame::checkIfNumBeforeEq(const char*c, int&i)const
{
	if (c[i] <'0' || c[i] >'9') // if not num return false
	{
		cout << "Error, there is no number before ''=''\n";
		return false;

	}
	else
	{
		while (1) // run untill the end, if unknown char return false
		{
			if (c[i] >= '0' && c[i] <= '9')
				i++;
			else if (c[i] == '=')
				return true;
			else
				return false;
		}
	}

	return false;
}
bool TheMathGame::checkTheBoard(const char board[ROWS + 256][COLS + 256], int numOfRows)
{
	int i, colCounter;
	bool res1 = true, res2 = true, res3 = true, res4 = true, res5 = true;;
	list <Point> lstOfBigSpaceship;
	list <Point> lstOfSmallSpaceship;
	Point temp;
	bool flagForPoint = false;

	if (numOfRows < ROWS || numOfRows > ROWS) // checking if the num of rows is legal
	{
		cout << "ERROR! there are: " << numOfRows << " rows! " << endl;
		res1 = false;
	}

	for (i = 0; i < numOfRows; i++) // running on the board 
	{
		for (colCounter = 0; board[i][colCounter] != '\0'; colCounter++)
		{
			if (board[i][colCounter] == '#') // big spaceships will be points in list
			{
				lstOfBigSpaceship.push_back(Point(colCounter, i));
			}
			else if (board[i][colCounter] == '@')// small spaceships will be points in list
			{
				lstOfSmallSpaceship.push_back(Point(colCounter, i));
			}
			else if (board[i][colCounter] == '*') // getting one point of item to check if there are multiple items
			{
				if (flagForPoint == false)
				{
					flagForPoint = true;
					temp.set(colCounter, i);
				}
			}
			else if (board[i][colCounter] == '+') // do nothing
			{

			}
			else if (board[i][colCounter] >= '0' && board[i][colCounter] <= '9') // do nothing
			{

			}
			else if (board[i][colCounter] == ' ' || board[i][colCounter] == 'X' || board[i][colCounter] == 'S') // do nothing
			{

			}
			else if (board[i][colCounter] == '!' || board[i][colCounter] == '$' || board[i][colCounter] == '%') // do nothing
			{

			}
			else
			{
				cout << "the char: " << board[i][colCounter] << " is unknown, it will be considered as a wall" << endl; // this will be unknown char - wall
			}
		}
		if (colCounter > COLS || colCounter < COLS) // if the num of cols is wrong
		{
			cout << "Error! in line number " << i + 1 << " there are: " << colCounter << " cols" << endl;
			res2 = false;
		}

	}

	res3 = checkTheListOfBigSpaceship(lstOfBigSpaceship); // checking the big space ship
	res4 = checkTheListOfSmallSpaceship(lstOfSmallSpaceship);// checking the small space ship

	if (res1 == true && res2 == true && res3 == true && res4 == true)
		return true;
	else
		return false;

}
bool TheMathGame::checkTheListOfBigSpaceship(list<Point> lst)const
{
	// here we check if the shape and the size of the big space ships is correct
	bool flag1 = true;
	if (lst.size() != 6) // first if the size of the big space ship is not 6 we can not continue
	{
		cout << "Error! in this board the Big spaceship size is: " << lst.size() << " it should be 6." << endl;
		flag1 = false;
	}

	else // if it is in the correct size, let's chack the shape
	{
		bool flag = false;
		//6 iterators for 6 points of spaceship
		list<Point>::iterator it1;
		list<Point>::iterator it2;
		list<Point>::iterator it3;
		list<Point>::iterator it4;
		list<Point>::iterator it5;
		list<Point>::iterator it6;

		it1 = lst.begin();
		it2 = lst.begin();
		it3 = lst.begin();
		it4 = lst.begin();
		it5 = lst.begin();
		it6 = lst.begin();

		//setting the iterators:
		advance(it1, 0);
		advance(it2, 1);
		advance(it3, 2);
		advance(it4, 3);
		advance(it5, 4);
		advance(it6, 5);

		if (it1->getX() == it3->getX() && it3->getX() == it5->getX()) // checking in one rotation status
		{
			if (it1->getY() == it3->getY() - 1 && it3->getY() == it5->getY() - 1)
			{
				if (it2->getX() == it4->getX() && it4->getX() == it6->getX())
				{
					if (it2->getY() == it4->getY() - 1 && it4->getY() == it6->getY() - 1)
					{
						if (it1->getY() == it2->getY() && it3->getY() == it4->getY() && it5->getY() == it6->getY())
							flag = true;
					}
				}
			}
		}
		else if (it1->getY() == it2->getY() && it2->getY() == it3->getY()) // checking in different rotation status
		{
			if (it1->getX() == it2->getX() - 1 && it2->getX() == it3->getX() - 1)
			{
				if (it4->getY() == it5->getY() && it5->getY() == it6->getY())
				{
					if (it4->getX() == it5->getX() - 1 && it5->getX() == it6->getX() - 1)
					{
						if (it1->getX() == it4->getX() && it2->getX() == it5->getX() && it3->getX() == it6->getX())
							flag = true;
					}
				}
			}
		}

		if (flag == false)
		{
			cout << "error, the big spaceship is not in the correct shape" << endl;
			flag1 = false;
		}

	}

	return flag1;
}
bool TheMathGame::checkTheListOfSmallSpaceship(list<Point> lst)const
{
	// here we check the small spaceship
	bool res = true;

	if (lst.size() != 3) // needs to be 3
	{
		cout << "Error! in this board the Small spaceship size is: " << lst.size() << " it should be 3." << endl;
		res = false;
	}
	else // if the size is ok we check the shape
	{
		list<Point>::iterator it1;
		list<Point>::iterator it2;
		list<Point>::iterator it3;

		//setting the iterators:
		it1 = lst.begin();
		it2 = lst.begin(); it2++;
		it3 = lst.end(); it3--;

		//the following terms checking the shape of the spaceship
		if (it1->getX() == it2->getX())
		{
			if (it1->getY() != it2->getY() - 1)
			{
				cout << "error, the small spaceship is not in the correct shape" << endl;
				res = false;
			}
			else
			{
				if (it2->getX() == it3->getX())
				{
					if (it2->getY() != it3->getY() - 1)
					{
						cout << "error, the small spaceship is not in the correct shape" << endl;
						res = false;
					}
				}
				else
				{
					cout << "error, the small spaceship is not in the correct shape " << endl;
					res = false;
				}
			}
		}
		else if (it1->getY() == it2->getY())
		{
			if (it1->getX() != it2->getX() - 1)
			{
				cout << "error, the small spaceship is not in the correct shape " << endl;
				res = false;
			}
			else
			{
				if (it2->getY() == it3->getY())
				{
					if (it2->getX() != it3->getX() - 1)
					{
						cout << "error, the small spaceship is not in the correct shape" << endl;
						res = false;
					}
				}
				else
				{
					cout << "error, the small spaceship is not in the correct shape" << endl;
					res = false;
				}
			}
		}
		else
		{
			cout << "error, the small spaceship is not in the correct shape" << endl;
			res = false;
		}
	}

	return res;
}
bool TheMathGame::alreadyExistInListInCurrClock(int x)const
{
	//find the correct node and 
	for (MyNode temp : listOfKeyHits)
	{
		if (x == temp.currentClock)
			return true;
	}


	return false;
}
void TheMathGame::findTheCorrectItemFromTheListByPoint(const Point& p, list<Item>::iterator& iter)
{
	bool flag = false;
	for (iter = it.begin(); (iter != it.end()) && (flag == false) ;iter++)
	{
		for (Point _p : *(iter->getlist()))
		{
			if (_p == p)
			{
				flag = true;
				break;
			}
		}
	}

}
void TheMathGame::handleTargil(int row, int col) // set the question
{
	int lft, rgt, rs, numOfElem;
	bool lflag, rflag, resflag;
	char oper;

	if (ItemHitSpaceship)
	{
		q.setExercise(lft, rgt, rs, oper, numOfElem, lflag, rflag, resflag,TempExcercise);
	}
	else
	{
		q.setExercise(lft, rgt, rs, oper, numOfElem, lflag, rflag, resflag);
	}

	if (PlayGameOption2 == false)
		q = question(Point(row, col), oper, lft, rgt, rs, numOfElem, lflag, rflag, resflag);

	else if (PlayGameOption2 == true)
		q = question(Point(row, col), '+', 238, -1, 240, 2, 1, 0, 1);

}
void TheMathGame::handleState(int row, int col) // set the state (Level, Lives, Clock)
{
	st = state(Point(row, col), clock, currentLevel);
}
void TheMathGame::printStatus() // print state
{

	gotoxy(st.GetPos().getY() - 1, st.GetPos().getX());
	for (int i = 0;i < 16;i++)
		cout << " ";
	gotoxy(st.GetPos().getY() - 1, st.GetPos().getX());
	setTextColor(Color::LIGHTGREEN);
	cout << " ID:" << id;
	setTextColor(Color::LIGHTRED);
	cout << " " << '\x3' << ":" << lives << " ";
	setTextColor(Color::LIGHTGREY);
}
bool TheMathGame::checkIfThisIsTheBest()
{
	// this func checks if we have a record

	char name[256] = "Record_";
	ifstream file;
	int bestClockFromFile;
	char MyLine[256];

	getTheNameToTheRecordFile(name); // setting the correct name of the file

	if (ifstream(name)) // if the file already exist we need to check if we beat the record
	{
		file.open(name);
		file.getline(MyLine, 256);
		file.getline(MyLine, 256);
		bestClockFromFile = getClockIterationsFromFile(MyLine); // getting the record from the file
		file.close();
		if (bestClockFromFile > st.getClock()) // if it is the record
		{
			file.close();
			return true;
		}
		else
		{
			file.close();
			return false;
		}
	}

	else
		return true;



}
bool TheMathGame::checkIfThereIsRecord()
{
	char name[256];

	getTheNameToTheRecordFile(name);

	if (ifstream(name))
	{
		setListOfRecord(); // if there is a record file already we load the key hits 
		return true;
	}
	else
		return false;


}
bool TheMathGame::SetForScreenChecker()
{
	//scanning the screen name and sending it to the screen checker

	string s;
	cin >> s;
	return (screenChecker(s));
}
bool TheMathGame::isThisCloser(const Point& position, const Point& MinDistance, const Point& pToCheck)const
{
	double a, b, c, a1, b1;
	double d, e, f, d1, e1;

	a = MinDistance.getX() - position.getX();
	b = MinDistance.getY() - position.getY();

	a1 = pow(a, 2);
	b1 = pow(b, 2);

	c = pow(a1 + b1, 0.5);

	d = pToCheck.getX() - position.getX();
	e = pToCheck.getY() - position.getY();

	d1 = pow(d, 2);
	e1 = pow(e, 2);

	f = pow(d1 + e1, 0.5);

	return f < c;

}

void TheMathGame::doIteration(const list<char>& keyHits) {

	if (isAutoReplay == true)
		autoReplay();
	else
	{
		int dir;
		char key;
		if ((dir = smallS.getDirection(keyHits)) != -1)
		{
			key = getKeyBasedOnDirection('S', dir);
			insertDataToMyNode(key, st.getClock(), 'S');
			if (dir != 5)
				smallS.setDirection(dir);
			else
				smallS.setRotationStatus();
		}
		else if ((dir = bigS.getDirection(keyHits)) != -1)
		{
			key = getKeyBasedOnDirection('B', dir);
			insertDataToMyNode(key, st.getClock(), 'B');
			if (dir != 5)
				bigS.setDirection(dir);
			else
				bigS.setRotationStatus();
		}
	}

	if (!isLevelDone && lives>0)
	{
		smallS.move();
		bigS.move();

		bool f=true;
		auto iter = it.begin();
		while (f==true && iter != it.end())
		{
			f= iter->moveItem();
			if (f) iter++;
		}

		f = true;
		auto i = colflyers.begin();
		for (int l = 0;l < 2;l++)
		{
			i = colflyers.begin();
			while (f == true && i != colflyers.end())
			{
				f = i->move();
				if (f) i++;
			}
		}
		f = true;
		auto i2 = rowflyers.begin();
		while (f == true && i2 != rowflyers.end())
		{
			f = i2->move();
			if (f) i2++;
		}
		removeDeadFromNumEaters();
		f = true;
		auto v = numeaters.begin();
		for (int l = 0;l < 2;l++)
		{
			v = numeaters.begin();
			while (v != numeaters.end())
			{
				f = v->move();
				v++;
			}
		}
		st.updateclock();
		clock = st.getClock();
		printCurrentClock();
	}

}
void TheMathGame::removeDeadFromNumEaters()
{
	for (size_t i = 0; i < numeaters.size();i++)
	{
		for (NumEaters temp : numeaters)
		{
			if (temp.getDeadStatus() == true)
			{
				numeaters.remove(temp);
				break;
			}
		}
	}
}
void TheMathGame::startLevel() // init the game (from TheMathGame)
{
	init();
}
void TheMathGame::Instruction()
{
	ifstream file("ins.txt");
	string line;
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			cout << line << endl;
		}
		file.close();
	}
	else
	{
		cout << "Unable to open file" << endl << endl;
	}
}
void TheMathGame::insertCharInsteadOfNumForItem(const Point& p)
{
	int x = p.getX();
	int y = p.getY();
	while (board[y][x] >= '0' && board[y][x] <= '9')
	{
		gotoxy(x, y);
		cout << " ";
		board[y][x] = ' ';
		x++;
	}
	x = p.getX() - 1;
	y = p.getY();
	while (board[y][x] >= '0' && board[y][x] <= '9')
	{
		gotoxy(x, y);
		cout << " ";
		board[y][x] = ' ';
		x--;
	}
}
void TheMathGame::readScreenFromFile(const char* filename)
{
	ifstream file;
	char MyLine[256];

	file.open(filename);

	file.getline(MyLine, 256); // scanning the line with the screen id to the char* myline
	id = getScreenIdFromFile(MyLine);

	file.getline(MyLine, 256);// scanning the line with the excercise to the char* myline
	strcpy_s(TempExcercise, MyLine);
	q.getExercise(MyLine); // sending the exercise to the qusetion class

	int i = 0;
	while (!file.eof())
	{
		file.getline(MyLine, 256);
		strcpy_s(originalBoard[i], MyLine); // copying the board from the file to the original board
		i++;
	}
	file.close();

}
void TheMathGame::readScreenFromFile(string filename)
{
	//same as above only different parameters
	ifstream file;
	char MyLine[256];

	file.open(filename);

	file.getline(MyLine, 256);
	id = getScreenIdFromFile(MyLine);

	file.getline(MyLine, 256);
	strcpy_s(TempExcercise, MyLine);
	q.getExercise(MyLine);

	int i = 0;
	while (!file.eof())
	{
		file.getline(MyLine, 256);
		strcpy_s(originalBoard[i], MyLine);
		i++;
	}
	file.close();

}
void TheMathGame::CheckTheListOfItem(const Point&p, list<Point>& lst)
{
	if (board[p.getY()][p.getX()] != '*') // if it is not item - return
		return;

	board[p.getY()][p.getX()] = ' ';  // if it is item insert space in there.
	lst.push_back(Point(p.getX(), p.getY()));

	CheckTheListOfItem(Point(p.getX(), p.getY() - 1), lst);    // go to the left
	CheckTheListOfItem(Point(p.getX(), p.getY() + 1), lst);    // go to the right
	CheckTheListOfItem(Point(p.getX() - 1, p.getY()), lst);    // go to the down
	CheckTheListOfItem(Point(p.getX() + 1, p.getY()), lst);    // go to the up
	CheckTheListOfItem(Point(p.getX()+1, p.getY() - 1), lst);  // go to the up-left
	CheckTheListOfItem(Point(p.getX()+1, p.getY() + 1), lst);  // go to the up-right
	CheckTheListOfItem(Point(p.getX() - 1, p.getY()-1), lst);  // go to the down-left
	CheckTheListOfItem(Point(p.getX() - 1, p.getY()+1), lst);  // go to the down-right

}
void TheMathGame::saveGame()
{
	// this is the func for save game:
	ofstream file;

	char str[256];

	cout << "please enter the file name to be saved (without .spp): "; // getting the name of the file to be saved
	cin >> str;
	cout << endl;

	strcat_s(str, ".spp"); // 

	file.open(str); // openning the file;

	file << "ScreenID=" << id << endl; //inserting the correct id

	file << "ClockIterations=" << clock << endl; // the clock

	file << "Exercise="; // now we will put the equation of excercise

	if (q.getLeft() == -1) 
		file << "_";
	else
		file << q.getLeft();

	file << q.Op();
	if (q.getRight() == -1)
		file << "_";
	else
		file << q.getRight();

	file << "=";

	if (q.getRes() == -1)
		file << "_" << endl;
	else
		file << q.getRes() << endl;

	file << "Life=" << lives << endl; // inserting the life

	for (int i = 0; i < ROWS; ++i) // here we insert the board
	{
		file << board[i];
		if (i < 23)
		{
			file << endl;
		}
	}

	list<MyNode>::iterator it;
	file << endl;
	int i = 1;
	for (it = listOfKeyHits.begin();it != listOfKeyHits.end();it++) // here we insert the key hits 
	{
		file << it->currentClock << ": ";

		if (it->keys[0] != '~')
			file << it->keys[0];
		if (it->keys[1] != '~')
			file << it->keys[1];

		it++;
		if (it != listOfKeyHits.end())
			file << endl;
		it--;


		i++;
	}

	file.close();
}
void TheMathGame::insertDataToMyNode(char key, int currClock, char sign)
{
	if (!alreadyExistInListInCurrClock(currClock))  // checking if there is another movement of another space ship in the same iteration
	{
		MyNode* node; // create new node
		node = new MyNode;

		node->currentClock = currClock; // set this clock to the node

		//setting the key hits:

		if (sign == 'B')
		{
			node->keys[0] = key;
			node->keys[1] = '~';
		}
		else if (sign == 'S')
		{
			node->keys[0] = '~';
			node->keys[1] = key;
		}

		listOfKeyHits.push_back(*node);
		delete node;
	}
	else // if already exist:
	{
		insertTheDataToExistingNode(key, currClock);
	}
}
void TheMathGame::insertTheDataToExistingNode(char key, int currClock)
{
	list<MyNode>::iterator it;

	for (it = listOfKeyHits.begin(); it != listOfKeyHits.end(); it++)
	{
		if (it->currentClock == currClock) // getting to the correct node
		{
			if (it->keys[0] == '~') // setting the keys
				it->keys[0] = key;

			else if (it->keys[1] == '~')
				it->keys[1] = key;
		}
	}
}
void TheMathGame::readScreenFrom_save_File(const char* filename)
{
	//this function opens the file in the name file name and load the save game
	ifstream file;
	char MyLine[256];

	file.open(filename);

	file.getline(MyLine, 256); 
	id = getScreenIdFromFile(MyLine);// getting the screen id 

	file.getline(MyLine, 256);
	int sizeOfLine = strlen(MyLine);
	for (int i = 16; i < sizeOfLine; ++i)
		clock = clock * 10 + MyLine[i] - '0'; // getting the clock iterations

	savedGameClock = clock; // setting the clock of the saved game

	file.getline(MyLine, 256);
	strcpy_s(TempExcercise, MyLine);
	q.getExercise(MyLine); // setting the excercise

	file.getline(MyLine, 256);
	sizeOfLine = strlen(MyLine); 
	lives = 0;
	for (int i = 5; i < sizeOfLine; ++i)
		lives = lives * 10 + MyLine[i] - '0'; // getting the lives

	for (size_t i = 0; i < 24; i++) // copying the board
	{
		file.getline(MyLine, 256);
		strcpy_s(originalBoard[i], MyLine);
	}


	while (!file.eof())
	{
		file.getline(MyLine, 256);
		insertDataFromLineToListOfKeyHits(MyLine); // scanning the keys pressed in the saved game loading it to the list
	}


	file.close();
}
void TheMathGame::autoReplay()
{
	//this is the function of the replay the solution
	int dir;
	char BigKey, SmallKey;

	if (iteratorForRepaly != listOfRecord.end()) // if it is not the end of the list
	{
		if (clockForReplay == iteratorForRepaly->currentClock) // if we reached the correct clock of the specific key
		{
			BigKey = iteratorForRepaly->keys[0]; // getting the key hits of the spaceships
			SmallKey = iteratorForRepaly->keys[1];

			if ((dir = smallS.getDirectionForReplay(SmallKey, 'S')) != -1) // getting the key pressed
			{
				if (dir != 5)
					smallS.setDirection(dir);
				else
					smallS.setRotationStatus();
			}
			else if ((dir = bigS.getDirectionForReplay(BigKey, 'B')) != -1) // same for big spaceship
			{
				if (dir != 5)
					bigS.setDirection(dir);
				else
					bigS.setRotationStatus();
			}
			iteratorForRepaly++;
		}
		clockForReplay++;
	}
	Sleep(135);
	
}
void TheMathGame::insertDataToRecordFile()
{
	setReplayLevelToFalse();
	setisReplayFromSavedGameToFalse();
	changeAutoReplayToFalse();

	char fname[256];
	string nameOfSolver;
	ofstream file;
	list<MyNode>::iterator it;

	getTheNameToTheRecordFile(fname); // getting the name of the record file


	setTextColor(Color::LIGHTGREEN);
	cout << "\n\n\n\t\t\t  Please enter your name: ";
	setTextColor(Color::LIGHTGREY);
	file.open(fname);
	cin >> nameOfSolver;
	cout << endl;

	file << "ScreenID=" << id << endl;
	file << "ClockIterations=" << st.getClock() << endl;
	file << "NameOfSolver=" << nameOfSolver << endl;

	for (it = listOfKeyHits.begin(); it != listOfKeyHits.end();it++) // here we insert the key hits with the clock
	{
		file << it->currentClock << ": ";
		if (it->keys[0] != '~')
		{
			file << it->keys[0];
		}
		if (it->keys[1] != '~')
		{
			file << it->keys[1];
		}
		it++;
		if (it != listOfKeyHits.end())
			file << endl;
		it--;
	}

	file.close();
}
void TheMathGame::insertToEndListTheData(const char myline[256])
{
	int _clock = 0, i = 0;
	char Bkey = '~', Skey = '~';

	while (myline[i] != ':')
	{
		_clock = _clock * 10 + myline[i] - '0'; // getting the clock
		i++;
	}
	i++;i++; // skip the space char
	if (myline[i] == 'a' || myline[i] == 'd' || myline[i] == 'w' || myline[i] == 'x' || myline[i] == 's' || myline[i] == 'z')
	{
		Bkey = myline[i]; // setting the key that belongs to the big
		i++;
	}
	if (myline[i] == 'j' || myline[i] == 'l' || myline[i] == 'i' || myline[i] == 'm' || myline[i] == 'k' || myline[i] == 'n')
	{
		Skey = myline[i]; // getting the key of the small
	}

	MyNode* node;

	node = new MyNode; // creating new node

	node->currentClock = _clock;
	node->keys[0] = Bkey;
	node->keys[1] = Skey;

	listOfRecord.push_back(*node);
	delete node;
}
void TheMathGame::RemoveSpaceshipsData()
{
	//init the spaceships
	bigS.setPosition(-1, -1);
	smallS.setPosition(-1, -1);

	bigS.clearList();
	smallS.clearList();
}
void TheMathGame::insertDataFromLineToListOfKeyHits(const char*myline)
{
	//same as above, we insert to different list
	int _clock = 0, i = 0;
	char Bkey = '~', Skey = '~', check1;

	while (myline[i] != ':')
	{
		_clock = _clock * 10 + myline[i] - '0';
		i++;
	}
	i++;i++; // skip the space char
	if (myline[i] == 'a' || myline[i] == 'd' || myline[i] == 'w' || myline[i] == 'x' || myline[i] == 's' || myline[i] == 'z')
	{
		Bkey = myline[i];
		i++;
	}
	check1 = myline[i];
	if (myline[i] == 'j' || myline[i] == 'l' || myline[i] == 'i' || myline[i] == 'm' || myline[i] == 'k' || myline[i] == 'n')
	{
		Skey = myline[i];
	}

	MyNode* node;

	node = new MyNode;

	node->currentClock = _clock;
	node->keys[0] = Bkey;
	node->keys[1] = Skey;

	listOfKeyHits.push_back(*node);
	delete node;
}
void TheMathGame::combineItems(const Point& p_other, Item& original)
{
	auto iter = it.begin();
	bool flag = false;

	for (iter = it.begin(); iter != it.end(); iter++)
	{
		for (Point temp : *(iter->getlist()))
		{
			if (temp.getX() == p_other.getX() && temp.getY() == p_other.getY()) // if we found
			{
				flag = true;
				break;
			}

			
		}
		if (flag == true)
			break;
	}


	auto templst = iter->getlist();
	auto iterOfPoints = templst->begin();
	auto listOfOriginal = original.getlist();
	Point * tempToSend;
	for (iterOfPoints = templst->begin();iterOfPoints != templst->end();iterOfPoints++) // running on the points and combining them with the new item
	{
		tempToSend = new Point;
		tempToSend->set(iterOfPoints->getX(), iterOfPoints->getY());
		listOfOriginal->push_back(*tempToSend);
		delete tempToSend;
	}
	
	bool flag2 = false;
	
	it.remove(*iter); // removing the item from the list

	/*
	for (Item tempItem : it)
	{
		for (Point tempPoint : *(tempItem.getlist()))
			if (p_other.getX() == tempPoint.getX() && p_other.getY() == tempPoint.getY())
			{
				it.remove(tempItem);
				flag2 = true;
			}
		if (flag2)
			break;

	}
	*/
}
void TheMathGame::removeTheItemFromTheListBasedOnSpecificPoint(const Point& p) // this function is for when 2 items are meeting and combined into 1 item
{
	bool flag = false;
	auto iter1 = it.begin();
	for (iter1 = it.begin(); iter1 != it.end(); iter1++)
	{
		for (auto iter2 = iter1->getlist()->begin(); iter2 != iter1->getlist()->end(); iter2++)
		{
			if (p == *iter2) // check if the points are similar
			{
				flag = true;
				break;
			}
			
		}
		if (flag == true)
		{
			break;
		}
		flag = false;
	}
	for (Point p : *iter1->getlist())
	{
		insertCharToBoard(p.getX(), p.getY(), ' ');
		gotoxy(p.getX(), p.getY());
		cout << ' ';
	}
	it.remove(*iter1); // remove the item that is combined.

}





