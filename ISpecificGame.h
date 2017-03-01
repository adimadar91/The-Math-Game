//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ISpecificGame.h
// ---------------
// ISpecificGame is an abstract base class without any implementation.
// (We call such a class "interface" though there are no interfaces in C++).
// This class is the base class for any actual game that wants to be managed by GameManager.
// All relevant operations that GameManager needs from an actual game will be declared here, however specific operations
// which are specific for a certain game and are not generic will not be declared here.
// NOTE that this class doesn't have any cpp file as there are NO actual implemenations, only signatures.
//
// Authors: Amir Kirsh, Iris Gaber
// First version: 2014-12-09
// 
// This code is part of a solution for "the math game" excercise in C++ course, Semester A 2016
// at the Academic College of Tel-Aviv-Yaffo.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Changes and additions:
// ------------------------
// DATE           Authors                 Change / Addition
// ----           --------                -----------------
// In the file itself, add above each change/addition a remark saying: "NEW CODE EX1, author=<name>, date=<YYYY-MM-DD>"
// and close it at the end with a remark saying "END of NEW CODE EX1" 
//
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef _ISPECIFIC_GAME_H_
#define _ISPECIFIC_GAME_H_

#include <list>

; using namespace std;

// this is a base class for actual games
// actual games will inherit from this class and implement the required methods
class ISpecificGame
{
public:
	// virtual fucnction with =0 is called an "abstract method"
	// abstract method must be implemented by non-abstract inherited class
	virtual bool isLevelDone1()const=0;
	virtual bool hasNextLevel()const=0;
	virtual void startLevel()=0;
	virtual bool moreFauilre() = 0;
	virtual void setLivesTo3() = 0;
	virtual void Instruction()=0;
	virtual void printCurrBoard() = 0;
	// get a list with keyHits and returns a list with the keys that were used
	virtual void doIteration(const list<char>& keyHits)=0;
	virtual void doSubIteration()=0;
	virtual void ForNextGamMe() = 0;
	virtual void setDirectionToStopForReplayLevel() = 0;
	virtual void getDir(string d, list<string> & f) = 0;
	virtual void setBoard(const char* name = NULL) = 0;
	virtual void setBoard(string name) = 0;
	virtual void setBoard2(const char* name = NULL) = 0;
	virtual void initBoard() = 0;
	virtual void saveGame() = 0;	
	//virtual void choose_screen_from_list(list<string>lst,char* res)=0;
	virtual void readScreenFrom_save_File(const char* filename) = 0;
	virtual bool getAutoReplayFlag()const = 0;
	virtual void changeAutoReplayToTrue() = 0;
	virtual void changeAutoReplayToFalse() = 0;
	virtual void setReplayLevelToTrue() = 0;
	virtual void setReplayLevelToFalse() = 0;
	virtual void setPlayGameToTrue() = 0;
	virtual void setPlayGameToFalse() = 0;
	virtual void setisReplayFromSavedGameToTrue() = 0;
	virtual void setisReplayFromSavedGameToFalse() = 0;
	virtual void init() = 0;
	virtual void FreeListOfPointsOfEverything() = 0;
	virtual void 	initClock()=0;
	virtual void SetTargilForAutoReplay() = 0;
	virtual void	resetForAutoRepaly() = 0;
	virtual bool checkIfThisIsTheBest() = 0;
	virtual void insertDataToRecordFile() = 0;
	virtual bool checkIfThereIsRecord()  =0;
	virtual void releaseListOfRecord() = 0;
	virtual int getScreenIdFromFile2(string s) = 0;
	virtual bool screenChecker(const char*filename) = 0;
	virtual bool screenChecker(string filename) = 0;
	virtual int getCurrentID() = 0;
	virtual void RemoveSpaceshipsData()=0;
	virtual void initIteratorForRepaly() = 0;
	virtual void InitClockForReplay() = 0;
	virtual bool SetForScreenChecker() = 0;
	virtual void setNumOfLevels(int t) = 0;
	virtual void SetIsLevelDoneToFalse() = 0;
	virtual void initCurrLevel() = 0;
	virtual void handleTargil(int row, int col) = 0;
	virtual void relaseListOfKeyHits() = 0;
};

#endif