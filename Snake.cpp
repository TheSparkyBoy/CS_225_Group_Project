#include "Segment.cpp"

class Snake {
private:
	Segment* head = NULL;
	int length;

public:
	Snake(int x, int y) {
		Segment* head = new Segment(x,y);
		length = 1;
	}
	void addSegment() {
		head->createNewSegment(head->getX(), head->getY());
		++length;
	}
	int getLength() { return length; };
	~Snake() {
		delete head;
	}

};