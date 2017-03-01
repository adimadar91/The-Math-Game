#ifndef _THEMATHGAME_H_
#define _THEMATHGAME_H_


//#include"board.h"

#include "Io_Utils.h"
#include "Spaceship.h"
#include "question.h"
#include "Item.h"
#include "ISpecificGame.h"
#include "state.h"
#include <list>


#include "Creatures.h"
/*
class RowFlyers;
class ColFlyers;
class NumEaters;
*/
using namespace std;

;enum { ROWS = 24, COLS = 80 };
enum { ESC = 27 };

struct MyNode
{
	int currentClock;
	char keys[2];
};

class TheMathGame : public ISpecificGame {

	Spaceship bigS = Spaceship('B'); // the big Spaceship
	Spaceship smallS = Spaceship('S'); // the small Spaceship
	Point solution; // the Point of the solution
	question q; // the question
	state st; // the state
	
	bool isAutoReplay = false;
	bool isReplayLevel = false;
	bool isReplayFromSavedGame = false;
	bool levelDone = false;
	bool ItemHitSpaceship = false;
	bool PlayGameOption2 = false;
	bool flagForSmall = false;
	bool flagForBig = false;
	bool flagForItem = false;
	bool isLevelDone = false; // flag that indicates if the level is done

	list<MyNode> listOfKeyHits;
	list<MyNode> listOfRecord;
	list<MyNode>::iterator iteratorForRepaly;
	list<Item> it; // the item 
	list<Point> numsOnBoard;
	list<RowFlyers> rowflyers;
	list<ColFlyers> colflyers;
	list<NumEaters> numeaters;
	
	char TempExcercise[256];
	char key; // key (for the run func this gets the char pressed by the user.
	char defaultBoard[ROWS][COLS + 1];
	char originalBoard[ROWS][COLS + 1];	// this is the original board that we got (we need COLS+1 because we handle it as null terminated char*)
	char board[ROWS][COLS + 1];	// this is the actual board we play on, i.e. changes on board are done here

	int clockForReplay;
	int lives = 3;
	int clock = 0, savedGameClock = 0;
	int id = 0;
	int currentLevel = 1;
	int numOfLevels = 1;
	
	void handleObjectCreationFromBoard(int row, int col);
public:
	void insertCharToBoard(int x, int y, char ch) // inserts specific char to board this functions updates the board as seen on the screen 
	{
		board[y][x] = ch;
	}
	bool isFail(const Point& p) // this func checks if a spaceship or item steped on number and this is not the solution
	{
		//here we need to insert function that checks the real num
		return ((board[p.getY()][p.getX()] >= '0') && (board[p.getY()][p.getX()] <= '9') && !solutionFound(p));
	}
	bool isNum(const Point& p)const // smae as above but this checks only if there is a number 
	{
		return ((board[p.getY()][p.getX()] >= '0') && (board[p.getY()][p.getX()] <= '9'));
	}
	void decreaseLevel() // decrease the current level - we use this function mainly if we need to restart the same level 
	{
		currentLevel--;
	}
	bool isWall(const Point& p)const // checks if this specific point is wall
	{
		return board[p.getY()][p.getX()] == '+';
	}
	bool theNextIsCreature(const Point& p) const
	{
		return board[p.getY()][p.getX()] == '!' || board[p.getY()][p.getX()] == '$';
	}
	void setKeyToESC() // just changes the pressed key to ESC
	{
		key = ESC;
	}
	int HowManyLivesLeft()const // returns lives 
	{
		return lives;
	}
	bool isItemInTheWay(const Point& p)const // checks if in this position there is item
	{
		return board[p.getY()][p.getX()] == '*';
	}
	void relaseListOfKeyHits() { listOfKeyHits.clear(); }
	bool isLevelDone1()const;
	bool hasNextLevel()const;
	void startLevel();
	// get a list with keyHits and returns a list with the keys that were used
	virtual void doIteration(const list<char>& keyHits);
	virtual void doSubIteration() {}
	void Instruction();
	bool moreFauilre();
	void setLivesTo3();
	void ForNextGamMe();
	void printCurrBoard();
	void setBoard(const char* name);
	void setBoard(string name);
	void setBoard2(const char* name);
	void getDir(string d, list<string> & f);
	void initBoard();
	void handleTargil(int row, int col);
	void handleState(int row, int col);
	void init();
	int getWholeNumFromBoard(const Point& p);
	void decreaseLives(); // change the lives - because walked on num or item hit the spaceship
	void printCurrentClock(); // prints the clock in the dedicated place
	void changeIsLevelDoneStatus(); // changes is level done to true
	void printCurrentLives(); // prints the current lives in the dedicated place
	void insertCharInsteadOfNumForItem(const Point& p);
	bool isSpaceshipAhead(const Point& p, char sign)const; // this func checks if the current point is part of a spaceship
	bool solutionFound(const Point& p);
	bool moveItemWithSpaceship(int SpaceshipDirection, char sign, const Point& p);
	void findTheCorrectItemFromTheListByPoint(const Point& p, list<Item>::iterator& it);
	void setTheItemsList();
	void printTargil();
	void printStatus();
	void setDirectionToStopForReplayLevel();
	int getScreenIdFromFile(const char* line);
	int getScreenIdFromFile(string line);
	int getScreenIdFromFile2(string s);
	void readScreenFromFile(const char * filename);
	void readScreenFromFile(string filename);
	int getNumberfromChars(const char* line, int& i);
	bool OnlyOneIsKnown()const;
	bool thisIsSolution(int number);
	bool screenChecker(const char*filename);
	bool screenChecker(string filename);
	bool checkIfScreenIDIsOK(const char*c)const;
	bool checkIfExerciseIsOK(const char*c)const;
	bool checkTheBoard(const char board[ROWS + 256][COLS + 256], int numOfRows);
	bool checkIfNumBeforeOperator(const char*c, int&i)const;
	bool checkIfNumBeforeEq(const char*c, int&i)const;
	bool checkIfFinalNum(const char* c, int& i)const;
	bool checkTheListOfBigSpaceship(list <Point> lst)const;
	bool checkTheListOfSmallSpaceship(list <Point> lst)const;
	void saveGame();
	char getKeyBasedOnDirection(char sign, int dir)const;
	void insertDataToMyNode(char key, int currClock, char sign);
	bool alreadyExistInListInCurrClock(int x)const;
	void insertTheDataToExistingNode(char key, int currClock);
	void readScreenFrom_save_File(const char* filename);
	void autoReplay();
	bool getAutoReplayFlag()const;
	void changeAutoReplayToTrue() { isAutoReplay = true; }
	void changeAutoReplayToFalse() { isAutoReplay = false; }
	void setReplayLevelToTrue() { isReplayLevel = true; }
	void setReplayLevelToFalse() { isReplayLevel = false; }
	void setPlayGameToTrue() { PlayGameOption2 = true; }
	void setPlayGameToFalse() { PlayGameOption2 = false; }
	void setisReplayFromSavedGameToTrue() { isReplayFromSavedGame = true; }
	void setisReplayFromSavedGameToFalse() { isReplayFromSavedGame = false; }
	void FreeListOfPointsOfEverything();
	void initClock();
	void SetTargilForAutoReplay() { q.setTargilForAutoReplay(); }
	void resetForAutoRepaly();
	int getClockIterationsFromFile(const char*line)const;
	bool checkIfThisIsTheBest();
	void insertDataToRecordFile();
	void getTheNameToTheRecordFile(char name[256])const;
	bool checkIfThereIsRecord();
	void setListOfRecord();
	void insertToEndListTheData(const char myline[256]);
	void releaseListOfRecord() { listOfRecord.clear(); }
	const char** getDefaultBoardToRun();
	void RemoveSpaceshipsData();
	void insertDataFromLineToListOfKeyHits(const char*myline);
	int getCurrentID() { return id; }
	void initIteratorForRepaly() { iteratorForRepaly = listOfRecord.begin(); }
	void InitClockForReplay() { clockForReplay = 0; clock = 0; st.initClock(); }
	bool SetForScreenChecker();
	bool sendToCheckIfScreenIDisOK(string s);
	bool checkIsDoubleScreenID(const char*fname, int idToCheck);
	bool checkIsDoubleScreenID(string fname, int idToCheck);
	void setNumOfLevels(int t) { numOfLevels = t; }
	void SetIsLevelDoneToFalse() { levelDone = false; }
	bool checkIfTheFileNameIsSup(const char* name)const;
	void initCurrLevel() { currentLevel = 0; }
	bool checkIfThereIsManyItems(char board[ROWS + 256][COLS + 256], Point& p,int numOfRows);
	void CheckTheListOfItem(const Point&p, list<Point>& lst);
	void SetItemHitSpaceshipToTrue() { ItemHitSpaceship = true; }
	void SetItemHitSpaceshipToFalse() { ItemHitSpaceship = false; }
	void combineItems(const Point& p_other, Item& original);
	void removeTheItemFromTheListBasedOnSpecificPoint(const Point& p);
	bool IsOkNextOfNumEaters(const Point& p) {	return (board[p.getY()][p.getX()] == ' ' || (board[p.getY()][p.getX()] <= '9' && board[p.getY()][p.getX()] >= '0'));	}
	bool isNumOnBoard(const Point& p) { return  (board[p.getY()][p.getX()] <= '9' && board[p.getY()][p.getX()] >= '0'); }
	list<Point>& numsOnBoardGetList() { return numsOnBoard; }
	void updateListOfNums();
	bool isThisCloser(const Point& position, const Point& MinDistance, const Point& pToCheck)const;
	void removeDeadFromNumEaters();
};

#endif