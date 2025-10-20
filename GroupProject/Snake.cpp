
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
		head->createNewSegment(head->getX(), head->getY());
		++length;
	}
	int getLength() { return length; };
	~Snake() {
		delete head;
		head = NULL;
		std::cout << "Oops! Your snake ate itself! Length: " << length << ".\n";
	}

};