#include <iostream>
#include "WindowManager.hpp"

int main() {
	// Initalize Window Size
	const int width = 80;
	const int height = 80;
	
	// Spawn snake in the middle
	Snake s(width/2, height/2);
	s.addSegment();
	//create a window mangager
	snkWndw w(width, height);

	//update the frame by printing the snake in the frame
	w.UpdateFrame(s);
	
	return 0;
}