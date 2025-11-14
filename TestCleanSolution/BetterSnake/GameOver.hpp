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
	bool loggedScore = false; // Track if score has been logged in this playthrough
public:
	// Getter function for loggedScore flag
	bool getLoggedScore();

	// Set or clear loggedScore flag
	void setLoggedScore(bool ls);

	// Record the current score to a file in (score,time) format
	void recordToFile(Snake* s);

	// Parses scores file to calculate high score and print it along with the time it was achieved
	void displayHighScore();
};
#endif