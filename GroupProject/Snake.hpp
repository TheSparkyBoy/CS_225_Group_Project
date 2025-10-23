/*
 * Snake.cpp
 * Implements the snake class and supporting functions to the segments.
 */
#include "Segment.hpp"
#include <iostream>

class Snake {
private:
	// The head is the segment that will be directly controlled by the player. See Segment.cpp for details.
	Segment* head = NULL;
	int length; //# of Segments in the Snake.

public:
	Snake(int x, int y);
	void addSegment();
	~Snake();
	int checkCoords(int a, int b) const;
	int headX() const;
	int headY() const;
	int getLength() const;
};