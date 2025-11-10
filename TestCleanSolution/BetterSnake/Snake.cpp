/****************************************************************************
 * File:   Snake.cpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	Implements the snake class and supporting functions to the segments.
 * Version:  1.0 Oct 22, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/
#include "Snake.hpp"

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
	}
}

// Move the snake one step in the current direction and detect collisions.
// Returns true if the snake collided with itself or went out of bounds.
bool Snake::moveSnake(int w, int h) {
	bool hasCollided = false;

	// Move according to direction enum
	switch (direction) {
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
	if (checkCoords(getX(), getY()) > 1) {
		hasCollided = true;
	}
	else
		// Otherwise check for out-of-bounds
		hasCollided = outOfBounds(w, h);

	return hasCollided;
}

Snake::~Snake() {
	// Print score on destruction (this is the current behavior)
	std::cout << "Oops! Your snake ate itself! Length: " << length << ".\n";
}

int Snake::getLength() const { return length; }

// setDirection enforces simple rules:
// - STOP overrides everything
// - cannot reverse direction immediately (prevents 180-degree turn)
void Snake::setDirection(int dir) {
	if (dir == STOP) {
		direction = STOP;
	} else if (dir % 2 == direction % 2 || direction == STOP) {
		// same axis (e.g. LEFT/RIGHT or UP/DOWN) => ignore to prevent reversing into itself
		direction = direction;
	}
	else {
		direction = dir;
	}
}

int Snake::getDirection() const {
	return direction;
}