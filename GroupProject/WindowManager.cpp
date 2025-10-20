

#include "Snake.cpp"
#include <iostream>
#include <Windows.h>


using std::cout;




class snkWndw {
private:
	const int w;
	const int h;

public:
	snkWndw(int width, int height) :w(width), h(height) {}
	void UpdateFrame(const Snake& s) {
		system("cls");
		int i, j = 0;
		for (i = 1; i <= h; ++i) {
			if (i == 1 || i == h) {
				for (j = 1; j <= w; ++j) {
					if (j == 1 || j == w)
						cout << "|";
					else
						cout << "_";
				}
				cout << "\n";
			}
			for (j = 1; j <= w; ++j) {
				if (j == 1 || j == w)
					cout << "|";
				else if (j == s.headX() && i == s.headY())
					cout << "O";
				else if (s.checkCoords(j,i) == 1) {
					cout << "o";
				}
				else
					cout << " ";
			}
			cout << "\n";
		}
	}
	
};




