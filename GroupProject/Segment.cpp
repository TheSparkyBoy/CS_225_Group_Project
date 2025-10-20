#include <cstdlib>
#include <iostream>
using std::cout;

class Segment {
private:
	Segment* next = NULL;
	int x;
	int y;
protected:
	// The segment class will call this function on the next segment, moving it to it's former location.
	void updateXY(int x, int y) {
		this->x = x;
		this->y = y;
	}
public:
	Segment(int x, int y) {
		this->x = x;
		this->y = y;
		next = NULL;
	}
	void createNewSegment() {
		if (next != NULL) {
			next->createNewSegment();
		}
		else {
			next = new Segment(x - 1, y);
		}
		
	}
	// The snake class will use this function to move it's head segment by one "block" in one direction at a time.
	void addXY(int x, int y) {
		int a = this->x;
		int b = this->y;
		this->x += x;
		this->y += y;
		next->updateXY(a, b);
	}
	int checkCoords(int a, int b) {
		int count = 0;
		if (a == x && b == y)
			++count;
		if (next == NULL)
			return count;
		else
			return count + next->checkCoords(a, b);		
	}
	bool areCoordsShared() {
		bool yes = false;
		if (x == next->getX() && y == next->getY())
			yes = true;
		if (next != NULL)
			return yes || next->areCoordsShared();
		else
			return yes;
	}
	/*void dispCoords() {
		cout << "( " << x << ", " << y << " )\n";
		if (next != NULL)
			next->dispCoords();
	} */

	~Segment() {
		if (next != NULL) {
			delete next;
			next = NULL;
		}
	}
	int getX() const { return x; };
	int getY() const { return y; };

};