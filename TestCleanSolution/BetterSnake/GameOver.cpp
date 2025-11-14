//Quentin implementation of your methods for recording game data

/****************************************************************************
 * File:	GameOver.cpp
 * Author:  Quentin Trull, Yunfeng Nie
 * Purpose:	Implements the GameOver.hpp methods and records scores.
 * Version:  1.0 Nov 9, 2025
 * Resources: cplusplus.com for unfamiliar functions
 *******************************************************************************/

#include "GameOver.hpp"
#include "Snake.hpp"
#include "Segment.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// Getter function for loggedScore flag
bool GameOver::getLoggedScore() { 
    return loggedScore;
}

// Function to set or clear flag
void GameOver::setLoggedScore(bool ls) { 
    loggedScore = ls;
}

// Record current score to file 
void GameOver::recordToFile(Snake* s) {
    if (!getLoggedScore()) { // Make sure score hasn't been logged already
        fstream scoreFile("scores.txt", ios::app); // Open file for appending
        scoreFile << *s; // Overloaded insertion operator to print to the file
        scoreFile.close(); // Close file
        displayHighScore(); // Determine and display high score on console
        setLoggedScore(true); // Set loggedScore flag
    }
}

// Determine and display high score
void GameOver::displayHighScore() {
    int highScore = 0; // Store high score 
    string highScoreTime = "00:00:00"; // Store time achieved

    ifstream file("scores.txt", ios::in); // Open scores file
    string tempLine; // Temporary variable to store file line

    // While there are lines to read...
    while (getline(file, tempLine)) { 
        stringstream ss(tempLine); // Convert string to a stream for getline
        string tempScore; // Temporary variable for score in this line
        string tempTime; // Temporary variable for timestamp in this line

        getline(ss, tempScore, ','); // grab score

        if (stoi(tempScore) > highScore) { // See if score is higher than current highscore
            highScore = stoi(tempScore); // Store high score after converting string to int
            getline(ss, tempTime); // Grab time from the remainder of the line
            highScoreTime = tempTime; // Store high score time
        }
    }

    file.close(); // Close scores file

    cout << "High Score: " << highScore << endl; // Print score to console
    cout << "High Score Achieved: " << highScoreTime << endl; // print time achieved to console
}