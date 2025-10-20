#include <iostream>
#include "Snake.cpp"


int main() {
	const int width = 100;
	const int height = 100;
	Snake s(width/2, height/2);
	s.addSegment();
	return 0;
}