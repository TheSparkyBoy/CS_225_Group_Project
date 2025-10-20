#include <iostream>
#include "WindowManager.cpp"

int main() {
	const int width = 80;
	const int height = 80;
	Snake s(width/2, height/2);
	s.addSegment();
	s.addSegment();
	s.addSegment();
	snkWndw w(width, height);
	w.UpdateFrame(s);
	Sleep(1000);
	//system("cls");
	//s.printCoords();
	return 0;
}