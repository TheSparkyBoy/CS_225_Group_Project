

#include "Snake.cpp"
#include <iostream>
#include <Windows.h>

using std::cout;


class snkWndw {
private:
	const int w;
	const int h;

public:
	//Use initialization list to initialize constant member variables
	snkWndw(int width, int height) :w(width), h(height) {}


	bool UpdateFrame(const Snake& s) {
		/* Function: UpdateFrame
		 * Purpose: Draw the snake (and eventually apples) in the game window. Will track collision in the future [WIP].
		 * Inputs: A reference to a snake. the snkWndw must have access to the snake in order to display snake coordinates on screen.
		 * Outputs: Whether a collision has occurred (true) or not (false)
		 */
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
	
};




