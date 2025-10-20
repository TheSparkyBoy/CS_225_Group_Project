

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
		for (i = 0; i < h; ++i) {
			if (i == 0 || i == h - 1) {
				for (j = 0; j < w; ++j) {
					if (j == 0 || j == w - 1)
						cout << "|";
					else
						cout << "_";
				}
				cout << "\n";
			}
			for (j = 0; j < w; ++j) {
				if (j == 0 || j == w - 1)
					cout << "|";
				else
					cout << " ";
			}
			cout << "\n";
		}
	}
	
};




