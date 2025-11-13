/****************************************************************************
 * File:	GameOver.hpp
 * Author:  Quentin Trull, Yunfeng Nie
 * Purpose:	Handles the game over state and records scores.
 * Version:  1.0 Nov 9, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/
#pragma once
#ifndef GAMEOVER_HPP
#define GAMEOVER_HPP
#include "Snake.hpp"
#include <string>


using namespace std;

class GameOver {
private:
	string FileName;
public:
	void recordToFile(Snake* s);
	void displayHighScore();
};
#endif