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
#include <string>
#include "Snake.hpp"


using namespace std;

class GameOver {
	friend std::ostream& operator<<(std::ostream& out, Snake& s);
private:
	string FileName;
	Snake& s;

public:
	void recordToFile(int score);
};
#endif