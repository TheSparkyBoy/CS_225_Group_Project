/*
 * Snake.cpp
 * Implements the snake class and supporting functions to the segments.
 */
#include "Segment.cpp"
#include <iostream>

class Snake {
private:
	// The head is the segment that will be directly controlled by the player. See Segment.cpp for details.
	Segment* head = NULL;
	int length; //# of Segments in the Snake.

public:
	Snake(int x, int y) {
		head = new Segment(x,y); //Creates a head / controller segment at the given coords.
		length = 1;
	}
	void addSegment() {
		head->createNewSegment(); //calls the createNewSegment() function in Segment.cpp. See Segment.cpp for details.
		++length;
	}

	~Snake() {
		delete head; //Release dynamically allocated memory
		head = NULL; // Stop pointing at memory
		std::cout << "Oops! Your snake ate itself! Length: " << length << ".\n"; // Print score statement.
	}
	int checkCoords(int a, int b) const {
		//returns the # of segments at a given location.
		return head->checkCoords(a, b); // See Segment.cpp for details
	}

	int headX() const { return head->getX(); };
	int headY() const { return head->getY(); };
	int getLength() { return length; };

};