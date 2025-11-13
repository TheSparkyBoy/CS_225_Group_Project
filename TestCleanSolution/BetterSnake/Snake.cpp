/****************************************************************************
 * File:   Snake.cpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	Implements the snake class and supporting functions to the segments.
 * Version:  1.0 Oct 22, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/
#include "Snake.hpp"
#include <time.h>
//#pragma warning(disable : 4996)

// NOTE: length and direction are member variables of Snake declared in Snake.hpp.
// The implementation uses the Segment linked-list behavior inherited from Segment.

Snake::Snake(int x, int y): Segment(x,y) {
	// Start with single-segment snake (the head)
	length = 1;
	direction = RIGHT; // default starting direction
}

// Append a new segment to the tail of the snake.
// This increments the logical length and either delegates to the existing
// tail or creates the first tail segment adjacent to current tail.
void Snake::createNewSegment() {
	++length;
	if (next != NULL) { // If this is not the tail of the snake, propagate
		next->createNewSegment(direction); // Call this function on the next segment
	}
	else { // Reached tail: place a new segment immediately behind the tail based on direction
		// Place new tail one cell behind current tail position
		next = new Segment(getX() - 1, this->getY());
		if (next == NULL)
			throw 1;
	}
}

// Move the snake one step in the current direction and detect collisions.
// Returns true if the snake collided with itself or went out of bounds.
bool Snake::moveSnake(int w, int h) {
	bool hasCollided = false;
	switch (direction) { //This switch statement detects if the requested direction is opposite the current direction
		// and prevents the snake from reversing into itself.
	case UP:
		if (this->getY() - 1 == next->getY())
			direction = DOWN;
		break;
	case LEFT:
		if (this->getX() - 1 == next->getX())
			direction = RIGHT;
		break;
	case DOWN:
		if (this->getY() + 1 == next->getY())
			direction = UP;
		break;
	case RIGHT:
		if (this->getX() + 1 == next->getX())
			direction = LEFT;
		break;

	}
	// Move according to direction enum
	switch (direction) { // Note that the coordinate system origin (0,0) is in the top left corner of the window
		// with the y-axis pointing down. Therefore moving up is a "negative" change in y.
	case UP:
		addXY(0, -1); 
		break;
	case RIGHT:
		addXY(1, 0);
		break;
	case DOWN:
		addXY(0, 1);
		break;
	case LEFT:
		addXY(-1, 0);
		break;
	case STOP:
		// Nothing happens when stopped.
		break;
	}

	// If more than one segment occupies the head coordinate it's a self-collision.
	if (checkCoords(getX(), getY()) > 1) { //See Segment.cpp for implementation
		hasCollided = true;
		return hasCollided; //No need to perform boundary checking, snake is already "dead".
	}
	else {
		// Otherwise check for out-of-bounds
		hasCollided = outOfBounds(w, h); //see Segment.cpp for implementation
	}
	return hasCollided;
}

Snake::~Snake() {
	// Print score on destruction of the snake, which occurs at restart or app quit.
	std::cout << "Oops! Your snake ate itself! Length: " << length << ".\n";
}

int Snake::getLength() const { return length; } //getter function for scoring

// setDirection enforces simple rules:
// - STOP overrides everything
// - cannot reverse direction immediately (prevents 180-degree turn)
void Snake::setDirection(int dir) {
	if (dir == STOP) { //Stop overrides other direction requests
		direction = STOP;
	} else if (dir % 2 == direction % 2 || direction == STOP) { // direction == STOP prevents the snake from moving after death
		// same axis (e.g. LEFT/RIGHT or UP/DOWN) => ignore to prevent reversing into itself
		direction = direction;
	}
	else {
		direction = dir;
	}
}

int Snake::getDirection() const { //getter  function for direction
	return direction;
}

ostream& operator<<(ostream& os, const Snake& s) {
	time_t t = (time(NULL) - 25200);
	struct tm gmt;
	char timeStr[26];

	gmtime_s(&gmt, &t);
	asctime_s(timeStr, sizeof(timeStr), &gmt);

	os << s.getLength() << "," << timeStr;

	return os;
}