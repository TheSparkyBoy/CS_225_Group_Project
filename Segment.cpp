#include <cstdlib>


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
	void createNewSegment(int x, int y) {
		if (next != NULL) {
			next->createNewSegment(x, y);
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
	~Segment() {
		if (next != NULL) {
			delete next;
			next = NULL;
		}
	}
	int getX() const { return x; };
	int getY() const { return y; };

};