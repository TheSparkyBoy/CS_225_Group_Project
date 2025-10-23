/****************************************************************************
 * File:   Snake.cpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	Implements the snake class and supporting functions to the segments.
 * Version:  1.0 Oct 22, 2025
 * Resources: NA
 *******************************************************************************/
#include "Snake.hpp"

	// The head is the segment that will be directly controlled by the player. See Segment.cpp for details.
	Segment* head = NULL;
	int length; //# of Segments in the Snake.

	Snake::Snake(int x, int y) {
		head = new Segment(x,y); //Creates a head / controller segment at the given coords.
		length = 1;
	}
	void Snake::addSegment() {
		head->createNewSegment(); //calls the createNewSegment() function in Segment.cpp. See Segment.cpp for details.
		++length;
	}

	Snake::~Snake() {
		delete head; //Release dynamically allocated memory
		head = NULL; // Stop pointing at memory
		std::cout << "Oops! Your snake ate itself! Length: " << length << ".\n"; // Print score statement.
	}
	int Snake::checkCoords(int a, int b) const {
		//returns the # of segments at a given location.
		return head->checkCoords(a, b); // See Segment.cpp for details
	}

	int Snake::headX() const { return head->getX(); }
	int Snake::headY() const { return head->getY(); }
	int Snake::getLength() const { return length; }