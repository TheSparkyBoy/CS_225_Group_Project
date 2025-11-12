//Quentin implementation of your methods for recording game data

/****************************************************************************
 * File:	GameOver.cpp
 * Author:  Quentin Trull, Yunfeng Nie
 * Purpose:	Implements the GameOver.hpp methods and records scores.
 * Version:  1.0 Nov 9, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/

#include "GameOver.hpp"
#include <fstream>
#include <iostream>
using namespace std;

void GameOver::recordToFile(int score) {

}


std::ostream& operator<<(std::ostream& out, Snake& s) {
	out << "Score: " << s.getLength() << "\n";
	return out;
}

