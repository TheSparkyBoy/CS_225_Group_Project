
#include "Segment.cpp"
#include <iostream>

class Snake {
private:
	Segment* head = NULL;
	int length;

public:
	Snake(int x, int y) {
		head = new Segment(x,y);
		length = 1;
	}
	void addSegment() {
		head->createNewSegment();
		++length;
	}
	int getLength() { return length; };
	~Snake() {
		delete head;
		head = NULL;
		std::cout << "Oops! Your snake ate itself! Length: " << length << ".\n";
	}
	int checkCoords(int a, int b) const {
		return head->checkCoords(a, b);
	}
	//void printCoords() { head->dispCoords(); };
	bool hasCollision() {
		return head->areCoordsShared();
	}
public:
	int headX() const { return head->getX(); };
	int headY() const { return head->getY(); };

};