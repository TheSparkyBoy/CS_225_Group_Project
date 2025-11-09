/****************************************************************************
 * File:	GameOver.hpp
 * Author:  Quentin Trull, Yunfeng Nie
 * Purpose:	Handles the game over state and records scores.
 * Version:  1.0 Nov 9, 2025
 * Resources: NA
 *******************************************************************************/
#include <string>

using namespace std;

class GameOver {
private:
	string FileName;

public:
	void recordToFile(int score);
};