//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// GameManager.cpp
// ---------------
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

#include <list>
#include "GameManager.h"
#include "ISpecificGame.h"
#include "Io_Utils.h"

using namespace std;

list<string>NamesOfFiles;
list<string>NamesOfFiles_saves;

void GameManager::run()
{
	bool userWantsToPlay = true;
	// we run as long as the user wants
	int i = 0;
	string levelSelect;
	char * Select;
	while(userWantsToPlay) {
		char menuSelection = mainMenu();
		actualGame.setPlayGameToFalse();
		// TODO: handle here all the different menu options
		switch(menuSelection)
		{
		case GameManager::MainMenuOptions::PLAY_GAME:
			updateListOfFiles();
			if (NamesOfFiles.size() > 0)
			{
				setForPlayGame();
				curr_file = NamesOfFiles.front();
				userWantsToPlay = playGame();
				break;
			}
			else
			{
				cout << "there are no files or there are no legal files therefore we can not continue" << endl;
				Sleep(4000);
				clear_screen();
				break;
			}
			break;
		case GameManager::MainMenuOptions::EXIT_APPLICATION:
			userWantsToPlay = false;
			break;
		case GameManager::MainMenuOptions::PRESENT_INSTRUCTIONS:
			doInst();
			break;
		case GameManager::MainMenuOptions::SCREEN_CHECKER:
			clear_screen();
			
			cout << "\nPlease enter the file name for the screen checker: ";
						
			if (actualGame.SetForScreenChecker())
				doForScreenChecker();
			else
				notDoForScreenChecker();
			break;

		case GameManager::MainMenuOptions::LOAD_GAME:
			flagOfLoadGame = true;
			updateListOfSavedFiles();
			clear_screen();
			cout << "List of Saved Games:\n";
			if (NamesOfFiles_saves.size() != 0)
			{
				Select = LoadTheGame(levelSelect);
				userWantsToPlay = playGame();
				delete[] Select;
				break;
			}
			else
			{
				NotLoadTheGame();
				break;
			}
		case GameManager::MainMenuOptions::PLAY_FROM_SELECTED_SCREEN:
			clear_screen();
			updateListOfFiles();
			actualGame.initCurrLevel();
			cout << endl;
			if (NamesOfFiles.size() > 0)
			{
				Select = doPlayFromSelected(levelSelect);
				userWantsToPlay = playGame();
				delete []Select;
				break;
			}
			else
			{
				notDoPlayFromSelected();
				break;
			}
			break;

		default: // normally we shouldn't get to here...
			userWantsToPlay = false;
		};
	}
}
bool GameManager::for_next_level()
{
	if (flagOfLoadGame == true)
		return true;

	ifstream temp;

	size_t size = NamesOfFiles.size();

	for (size_t i = 0; i < size; i++)
	{
		for (string s : NamesOfFiles)
		{
			temp.open(s);
			if (!temp.is_open())
			{
				NamesOfFiles.remove(s);
				break;
			}
			else
				temp.close();
		}
	}
	return (NamesOfFiles.size() > 0);


}
bool GameManager::playGame()
{
	// we assume that we have multiple levels so there is a need to loop through levels
	// till all levels were played or till user quits - if user quits we do not continue to next level
	//-------------------------------------------------------------
	// this is the game LEVELS loop
	//-------------------------------------------------------------
	char action = GameManager::LevelOptions::NEXT_LEVEL;
	while(actualGame.hasNextLevel() && action == GameManager::LevelOptions::NEXT_LEVEL && for_next_level()) 
	{
		action = playNextLevel();
		actualGame.SetIsLevelDoneToFalse();
		actualGame.initClock();
	}
	//-------------------------------------------------------------
	// END of game LEVELS loop
	//-------------------------------------------------------------
	
	actualGame.ForNextGamMe();
	// return true if the user wants to keep playing
	return (action != GameManager::LevelOptions::EXIT_APPLICATION);
}
// return action to take in case of ESC
char GameManager::playNextLevel()
{
	++currentLevel;
	if (flagOfLoadGame)
	{
		actualGame.startLevel();
		flagOfLoadGame = false;
	}
	else if (NamesOfFiles.size() > 0)
	{
		curr_file = NamesOfFiles.front();
		actualGame.setBoard(curr_file);
		actualGame.startLevel();
		NamesOfFiles.remove(curr_file);
	}
	
	//------------------------------------------------------------------------------
	// here we control the ESC menu
	//------------------------------------------------------------------------------
	bool keepRunning = true;
	char action = 0;
	while(keepRunning)
	{
		//=============================================================================================
		// this is the actual call to play game iterations
		action = doLevelIterations();
		//=============================================================================================
		
		// check action based on game ended (action==BACK_TO_MAIN_MENU) or input from user on ESC menu
		switch(action) {
		case GameManager::LevelOptions::CONTINUE:
			// keepRunning is true, so we only need to set thing right and then - keepRunning!
			//--------------------------------------------------------------------------------
			doForContinue();
			break;
		case GameManager::LevelOptions::REPLAY_LEVEL:
			// keepRunning is true, so we only need to set thing right and then - keepRunning!
			//--------------------------------------------------------------------------------
			doForReplayLevel();
			break;
		case GameManager::LevelOptions::BACK_TO_MAIN_MENU:
			doForBackToMainMenu();
			keepRunning = false;
		case GameManager::LevelOptions::EXIT_APPLICATION:
			// get out from the loop
			clear_screen();
			keepRunning = false;
			break;
		case GameManager::LevelOptions::SAVE_GAME:
			clear_screen();
			actualGame.saveGame();
			clear_screen();
			keepRunning = false;
			break;

		case GameManager::LevelOptions::AUTO_REPLAY_THE_RECORD:
			if (actualGame.checkIfThereIsRecord())
			{
				DoAutoReplay();
				break;
			}
			else
			{
				notDoAutoReplay();
				keepRunning = false;
			}
			break;
		case GameManager::LevelOptions::NEXT_LEVEL:
			// get out from the loop so we get to the next level
			keepRunning = false;
			break;
		}
		//---------------------------------------
		// END of sub menu action switch
		//---------------------------------------
	}
	//------------------------------------------
	// END of Level keepRunning loop (sub menu)
	//------------------------------------------

	// the actions that may get out from this method are: BACK_TO_MAIN_MENU and EXIT_APPLICATION
	return action;
}
// return action to take in case of ESC
char GameManager::doLevelIterations()
{
	char action;
	bool escapePressed = false;
	//---------------------------------------
	// this is the game iterations loop
	//---------------------------------------
	while(!actualGame.isLevelDone1() && !escapePressed) {
		escapePressed = !doIteration();
	}
	//-----------------------------------------------------
	// END of game iterations loop or exit on ESC
	//-----------------------------------------------------

	// check why we are here
	if(actualGame.isLevelDone1() && actualGame.moreFauilre()==true) 
	{
		clear_screen();
		setTextColor(Color::YELLOW);
		cout << endl << "\n\n\n\n\n\n\t\t\t\tWELL DONE" << endl;
		actualGame.setPlayGameToFalse();
		bool returnToMain = actualGame.getAutoReplayFlag();
		actualGame.changeAutoReplayToFalse();
		actualGame.setReplayLevelToFalse();
		Sleep(500);
		
		if (actualGame.checkIfThisIsTheBest())
		{
			setTextColor(Color::LIGHTBLUE);
			cout << "\t\t\tCongratulations!! New Record!\n";
			setTextColor(Color::LIGHTGREY);
			Sleep(1500);
			actualGame.insertDataToRecordFile();
		}
		
		clear_screen();
		if (!returnToMain)
			action = GameManager::LevelOptions::NEXT_LEVEL;
		else
			action = GameManager::LevelOptions::BACK_TO_MAIN_MENU;
	}
	else if(escapePressed) {
		clear_screen();
		action = 0;

		setTextColor(Color::RED);
		cout << "\n\n\t\t   =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << endl;
		cout << "\t\t       T H E     M A T H     G A M E" << endl;
		cout << "\t\t   =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << endl << endl << endl ;
		setTextColor(Color::GREEN);
		cout << "\t\t   =-=-=-=-=-=-=GAME PAUSED=-=-=-=-=-=-=-" << endl << endl << endl ;
		setTextColor(Color::LIGHTCYAN);
		cout << "\t\t\t         1. EXIT" << endl << endl;
		cout << "\t\t\t    2. BACK TO MAIN MENU" << endl << endl;
		cout << "\t\t\t       3. CONTINUE" << endl << endl;
		cout << "\t\t\t     4. REPLAY LEVEL" << endl << endl;
		cout << "\t\t\t       5. SAVE GAME" << endl << endl;
		cout << "\t\t\t 6. AUTO REPLAY THE RECORD" << endl << endl;
		setTextColor(Color::LIGHTGREY);


		do {
			action = _getch();
		} while(!GameManager::LevelOptions::isValidOption(action));
	}
	else if (actualGame.isLevelDone1() && actualGame.moreFauilre()==false)
	{
		clear_screen();
		cout << endl << "No More Lives! Loser :(" << endl;
		
		Sleep(1000);
		action = GameManager::LevelOptions::BACK_TO_MAIN_MENU;
	}

	// end of esc pressed
	// TODO: clear the sub menu options from screen
	return action;
}

bool GameManager::doIteration()
{
	unsigned int sleepLength = clockCycleInMillisec/iterationsPerClockCycle;
	for(unsigned int i=0; i<iterationsPerClockCycle-1; ++i) {
		Sleep(sleepLength);
		actualGame.doSubIteration();
	}
	return doInputIteration();
}


void GameManager::doInst()
{
	clear_screen();
	actualGame.Instruction();
	setTextColor(Color::LIGHTGREEN);
	cout << "Please Wait, Loading The Main Menu...\n";
	setTextColor(Color::LIGHTGREY);
	Sleep(3000);
	clear_screen();
}
void GameManager::doForBackToMainMenu()
{
	clear_screen();
	actualGame.FreeListOfPointsOfEverything();
	actualGame.setisReplayFromSavedGameToFalse();
	actualGame.setReplayLevelToFalse();
	actualGame.changeAutoReplayToFalse();
	actualGame.initClock();
	actualGame.setDirectionToStopForReplayLevel();
	actualGame.setLivesTo3();
}
void GameManager::DoAutoReplay()
{
	actualGame.InitClockForReplay();
	actualGame.initIteratorForRepaly();
	actualGame.setisReplayFromSavedGameToFalse();
	actualGame.changeAutoReplayToTrue();
	clear_screen();
	actualGame.resetForAutoRepaly();
	int x = actualGame.getCurrentID();
	updateListOfFiles();
	for (string s : NamesOfFiles)
	{
		if (actualGame.getScreenIdFromFile2(s) == x)
			actualGame.setBoard(s);
	}
	actualGame.changeAutoReplayToFalse();
	actualGame.init();
	actualGame.changeAutoReplayToTrue();
	actualGame.startLevel();
	

}
void GameManager::notDoAutoReplay()
{
	clear_screen();
	cout << "Error! there is no record to replay! exiting to main menu...\n";
	actualGame.setReplayLevelToFalse();
	actualGame.setisReplayFromSavedGameToFalse();
	actualGame.changeAutoReplayToFalse();
	Sleep(2000);
	clear_screen();
}
bool GameManager::doInputIteration()
{
	list<char> keyHits(KEYBOARD_HIT_LOOP); // set vector initialize size to the max key strokes that we may collect
	bool shouldContinue = true;
	// get keyboard input in a loop, to get enough keyboard hits
	unsigned int sleepLength = clockCycleInMillisec/iterationsPerClockCycle / KEYBOARD_HIT_LOOP;
	for(unsigned int i=0; i<KEYBOARD_HIT_LOOP; ++i) {
		//Sleep(sleepLength);
		Sleep(14);
		if (_kbhit()) {
			char ch = _getch();
			if(ch == ESC) {
				// ESC pressed
				shouldContinue = false;
			}
			else {
				// we got keyHits byref, so we actually set input into the original list sent here
				keyHits.push_front(ch); // we want recent keys to come first, thus we use push_front
			}
		}
	}
	// send the keystrokes to the game
	// (even if ESC was hit, we may still have something in the keystrokes vector and should use it)
	actualGame.doIteration(keyHits);

	return shouldContinue;
}
void GameManager::doForContinue()
{
	clear_screen();
	actualGame.setDirectionToStopForReplayLevel();
	actualGame.printCurrBoard();
}
void GameManager::doForReplayLevel()
{
	clear_screen();
	actualGame.SetTargilForAutoReplay();
	actualGame.setReplayLevelToTrue();
	actualGame.setDirectionToStopForReplayLevel();
	actualGame.setLivesTo3();
	actualGame.FreeListOfPointsOfEverything();
	actualGame.releaseListOfRecord();
	actualGame.relaseListOfKeyHits();
	actualGame.initClock();
	actualGame.startLevel();
}
void GameManager::doForScreenChecker()
{
	setTextColor(Color::CYAN);
	cout << "\nThe Screen Is Legal!\n";
	setTextColor(Color::LIGHTGREY);
	Sleep(1500);
	clear_screen();
}
void GameManager::notDoForScreenChecker()
{
	setTextColor(Color::CYAN);
	cout << "\nThe Screen Is Not Legal!\n";
	setTextColor(Color::LIGHTGREY);
	cout << "\nplease wait, exiting to main menu\n";
	Sleep(8500);
	clear_screen();
}
char* GameManager::doPlayFromSelected(string& levelSelect)
{
	char *Select;
	int i = 1, x;
	for (string s : NamesOfFiles)
	{
		cout << i << ". " << s << "\t the ScreenID is: " << actualGame.getScreenIdFromFile2(s) << "\t choose: " << i << endl;
		i++;
	}
	cout << "\nPlease choose the ScreenID you wish to play\n(note that the screen checker removed the ilegal files): ";
	cin >> x;
	cout << endl;
	i = 1;
	for (string s : NamesOfFiles)
	{
		if (i == x) {
			levelSelect = s;
		}
		i++;
	}
	Select = new char[levelSelect.length() + 1];

	for (size_t r = 0; r < levelSelect.length() + 1; r++)
		Select[r] = levelSelect[r];

	clear_screen();
	init();
	clear_screen();
	updateTheListForSelected(x - 1);
	actualGame.setNumOfLevels(NamesOfFiles.size());
	return Select;
}
void GameManager::notDoPlayFromSelected()
{
	clear_screen();
	cout << "there are no screen files!" << endl;
	Sleep(2000);
	clear_screen();
}
void  GameManager::NotLoadTheGame()
{
	clear_screen();
	setTextColor(Color::RED);
	cout << "\n\n \tThere are no saved files!\n\tExiting to Main Menu! Please wait...\n";
	setTextColor(Color::LIGHTGREY);
	Sleep(3800);
	clear_screen();
}
char* GameManager::LoadTheGame(string & levelSelect)
{

	char * Select;
	cout << endl;
	int i = 0;
	size_t x;
	for (string s : NamesOfFiles_saves)
	{
		cout << "Name of file: " << s;
		if (s.size() < 8)
		{
			for (int j = s.size(); j < 9; j++)
				cout << " ";
		}
		cout << "\t the ScreenID is: " << actualGame.getScreenIdFromFile2(s) << " \t press: " << i + 1 << endl;
		i++;
	}
	cout << "\nPlease choose the ScreenID you wish to play: ";

	i = 0;
	do
	{
		cin >> x;
		cout << endl;
		if (x <= NamesOfFiles_saves.size() && x>0)
		{
			i = 0;
			for (string s : NamesOfFiles_saves)
			{
				if (i == x - 1)
					levelSelect = s;

				i++;
			}
		}
		else
			cout << "worng input, try again: ";

	} while (x > NamesOfFiles_saves.size() || x <= 0);


	Select = new char[levelSelect.length() + 1];

	int size = levelSelect.length() + 1;
	for (int i = 0; i < size; i++)
		Select[i] = levelSelect[i];

	clear_screen();
	actualGame.setisReplayFromSavedGameToTrue();
	init();
	actualGame.setBoard2(Select);

	return Select;

}
void GameManager::setForPlayGame() { clear_screen();actualGame.initCurrLevel();init();actualGame.setNumOfLevels(NamesOfFiles.size()); }
char GameManager::mainMenu()const
{
	// TODO: you may want to improve the menu appearance
	setTextColor(Color::RED);
	cout << "\n\n\t\t   =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << endl;
	cout << "\t\t       T H E     M A T H     G A M E" << endl;
	cout << "\t\t   =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-" << endl << endl << endl << endl;
	setTextColor(Color::LIGHTCYAN);
	cout << "\t\t\t       1. INSTRUCTIONS" << endl << endl;
	cout << "\t\t\t        2. PLAY GAME" << endl << endl;
	cout << "\t\t\t      3. SCREEN CHECKER" << endl << endl;
	cout << "\t\t\t        4. LOAD GAME" << endl << endl;
	cout << "\t\t\t5. START FROM A SPECIFIC LEVEL" << endl << endl;
	cout << "\t\t\t          9. EXIT" << endl << endl;
	setTextColor(Color::LIGHTGREY);

	char selection = 0;
	do {
		selection = _getch();
	} while (!GameManager::MainMenuOptions::isValidOption(selection));
	return selection;
}
void GameManager::updateListOfSavedFiles()
{
	NamesOfFiles_saves.clear();
	actualGame.getDir("*spp*.*", NamesOfFiles_saves);
	clear_screen();
}
int sendToGetScreenID(string s)
{
	int res = 0, sizeOfLine;

	char line[256];
	ifstream file;
	file.open(s);

	file.getline(line, 256);

	sizeOfLine = strlen(line);

	for (int i = 9; i < sizeOfLine; ++i)
		res = res * 10 + line[i] - '0';

	file.close();

	return res;

}
bool MyCompare(const string & s1, const string& s2)
{
	int id1 = sendToGetScreenID(s1);
	int id2 = sendToGetScreenID(s2);

	return (id1 < id2);
}
void GameManager::sortTheList()
{

	int t = 0;
	string*arr, temp;;
	arr = new string[NamesOfFiles.size()];
	for (string s : NamesOfFiles)
	{
		arr[t] = s;
		t++;
	}

	for (int i = 0;i < NamesOfFiles.size();i++)
	{
		for (int j = 0; j < NamesOfFiles.size() - 1;j++)
		{
			if (actualGame.getScreenIdFromFile2(arr[j]) > actualGame.getScreenIdFromFile2(arr[j + 1]))
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}

		}
	}

	int size = NamesOfFiles.size();
	NamesOfFiles.clear();
	for (int i = 0; i < size;i++)
		NamesOfFiles.push_back(arr[i]);

	delete[] arr;

}
void GameManager::updateListOfFiles()
{

	NamesOfFiles.clear();
	actualGame.getDir("*spg*.*", NamesOfFiles);
	removeIlegalScreensFromList();

	NamesOfFiles.sort(MyCompare);

	clear_screen();

}
void GameManager::removeIlegalScreensFromList()
{
	int size = NamesOfFiles.size() + 1;

	setTextColor(Color::BLACK);

	for (int i = 0; i < size; i++)
	{
		for (string s : NamesOfFiles)
		{
			if (!(actualGame.screenChecker(s)))
			{
				NamesOfFiles.remove(s);
				break;
			}
		}
	}
	setTextColor(Color::LIGHTGREY);
}
void GameManager::updateTheListForSelected(int place)
{
	for (int i = 0; i < place;i++)
	{
		for (string s : NamesOfFiles)
		{
			NamesOfFiles.remove(s);
			break;
		}
	}
}