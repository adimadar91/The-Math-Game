//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// GameManager.h
// -------------
// GameManager is the top level manager, the game starts from here.
// This class manages the game iterations, menus etc., but WITHOUT knowing anything about the actual game itself.
//
// Author: Amir Kirsh
// First version: 2014-12-09
// 
// This code is part of "the math game" excercise in C++ course, Semester A 2016
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

#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "ISpecificGame.h"
#include <string>
#include <list>
#include <fstream>


using namespace std;

int sendToGetScreenID(string s);

class GameManager
{
	ISpecificGame& actualGame;
	bool flagOfLoadGame = false;
	unsigned int currentLevel;
	unsigned int clockCycleInMillisec;
	unsigned int iterationsPerClockCycle;
	const static unsigned int KEYBOARD_HIT_LOOP = 10;
	const static char ESC = 27; // the ESC key
	string curr_file;
public:
	GameManager(ISpecificGame& theSpecificGame, unsigned int clockCycleLengthInMillisec,unsigned int numIterationsPerClockCycle)
		:actualGame(theSpecificGame), currentLevel(0), 	clockCycleInMillisec(clockCycleLengthInMillisec), 	iterationsPerClockCycle(numIterationsPerClockCycle) 
	{}
	void run();
	void setForPlayGame();
	char* LoadTheGame(string & levelSelect);
	void  NotLoadTheGame();
	void notDoPlayFromSelected();
	void doForScreenChecker();
	void notDoForScreenChecker();
	void doForReplayLevel();
	void doForContinue();
	void doForBackToMainMenu();
	void DoAutoReplay();
	void notDoAutoReplay();
	void doInst();
	char* doPlayFromSelected(string& levelSelect);
	bool for_next_level();
	void removeIlegalScreensFromList();
	void updateTheListForSelected(int place);
	
	friend bool MyCompare(const string & s1, const string& s2);
private:
	// helper methods
	void init()
	{
		currentLevel = 0;
	}
	char mainMenu()const;
	void sortTheList();
	
	bool playGame();
	char playNextLevel();
	char doLevelIterations();
	bool doIteration();
	bool doInputIteration();
public:
	void updateListOfSavedFiles();
	void updateListOfFiles();
	class MainMenuOptions
	{
	public:
		static bool isValidOption(char option) {
			string validOptions = "123459";
			return (validOptions.find(option) != string::npos);
		}
		enum
		{
			PRESENT_INSTRUCTIONS = '1',
			PLAY_GAME = '2',
			SCREEN_CHECKER = '3',
			LOAD_GAME           =   '4',
			PLAY_FROM_SELECTED_SCREEN	=	'5',
			EXIT_APPLICATION		=	'9'
		};
	};
	class LevelOptions
	{
	public:
		static bool isValidOption(char option) {
			string validOptions = "1234569";
			return (validOptions.find(option) != string::npos);
		}
		enum
		{
			EXIT_APPLICATION = '1',
			BACK_TO_MAIN_MENU = '2',
			CONTINUE = '3',
			REPLAY_LEVEL = '4',
			SAVE_GAME = '5',
			AUTO_REPLAY_THE_RECORD = '6',
		    NEXT_LEVEL = '9'
		};
	};
};

#endif