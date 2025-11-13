//Quentin implementation of your methods for recording game data

/****************************************************************************
 * File:	GameOver.cpp
 * Author:  Quentin Trull, Yunfeng Nie
 * Purpose:	Implements the GameOver.hpp methods and records scores.
 * Version:  1.0 Nov 9, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/

#include "GameOver.hpp"
#include "Snake.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void GameOver::recordToFile(Snake* s) {
	fstream scoreFile("scores.txt", ios::app);
	scoreFile << *s;
}

void GameOver::displayHighScore() {
    int highScore = 0;
    string highScoreTime = "00:00:00";

    ifstream file("scores.txt");
    string tempLine;
    stringstream ss(tempLine);

    while (getline(file, tempLine)) {
        stringstream ss(tempLine); // Convert string to a stream for getline
        string tempScore;
        string tempTime;

        getline(ss, tempScore, ','); // grab score

        if (stoi(tempScore) > highScore) {
            highScore = stoi(tempScore);
            getline(ss, tempTime); // Grab time
            highScoreTime = tempTime;
        }
    }

    file.close();

    cout << "High Score: " << highScore << endl;
    cout << "High Score Achieved: " << highScoreTime << endl;
}