/****************************************************************************
* File:   WindowsManager.cpp
* Author:   Konnor Barnes, Yunfeng Nie
* Purpose:  
* Version:  1.0 Oct 22, 2025
* Resources: NA
*******************************************************************************/
#include "WindowManager.hpp"
#include <iostream>
#include <Windows.h>

using std::cout;


//Use initialization list to initialize constant member variables
snkWndw::snkWndw(int width, int height) :w(width), h(height) {}


bool snkWndw::UpdateFrame(const Snake& s) {
	cout << "\033[2J" << "\033[H" << std::flush; //This ANSI escape sequence should clear the screen... but doesn't
	system("cls"); // This windows command should also clear the screen...but doesn't

	// i = rows, j = columns
	int i, j = 0;
	bool hasCollided = false;
	for (i = 1; i <= h; ++i) {
		// Print the top and bottom borders
		if (i == 1 || i == h) {
			for (j = 1; j <= w; ++j) {
				//Print '|' at the horizontal boundaries instead of '_'.
				if (j == 1 || j == w)
					cout << "|";
				else
					cout << "_";
			}
			cout << "\n";
		}
		// Print the vertical boundaries and the snake
		// TODO: Print the apples/fruits/whatever in here as well.
		for (j = 1; j <= w; ++j) {
			if (j == 1 || j == w) //Prints vertical boundaries
				cout << "|";
			else if (j == s.headX() && i == s.headY()) // Print the head
				cout << "O";
			else if (s.checkCoords(j, i) == 1) // Print any segments
				cout << "o";
			else if (s.checkCoords(i, j) > 1)
				hasCollided = true; //If more than one segment is in a location, return hasCollided = true;
			else
				cout << " ";
		}
		cout << "\n";
	}
	return hasCollided;  
}




