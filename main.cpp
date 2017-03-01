//check
#include <windows.h> 
// we put this in comment because the MAMA compiler gives us errors,
// please remove it while running this in visual stusio, thanks
// please note that also in Config.h the #define windows also in comment 
// because of the same problem



#include <iostream>
#include "TheMathGame.h"
#include "Color.h"
#include "GameManager.h"
#include "Board.h"
using namespace std;



int main() 
{
	clear_screen();
	hideCursor();

	//const char** boardToRun = board_example;
	TheMathGame theMathGame;
	//theMathGame.screenChecker("text.txt");
	//theMathGame.setBoard(boardToRun);

	GameManager(theMathGame, 200, 1).run();

	return 0;
}