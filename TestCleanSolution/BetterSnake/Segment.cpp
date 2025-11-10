/****************************************************************************
 * File:   Segment.cpp
 * Author:   Konnor Barnes
 * Purpose:	 Implements the Segment Class
 * Version:  1.0 Oct 22, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/

#include "Segment.hpp"

// Implementation notes:
// - Each Segment object owns a pointer to the next Segment in the snake (tail direction).
// - updateXY, createNewSegment, addXY and checkCoords walk/modify that linked list.

void Segment::updateXY(int a, int b) {
	// If there is a next segment, tell it to take this segment's current position.
	if (next != NULL)
		next->updateXY(x, y); // Call on the next segment to move to this segment's coordinates

	// Update this segment's coordinates to the provided values
	x = a;
	y = b;
}

Segment::Segment(int x, int y) {
	// Initialize position and mark this as the tail (next == NULL)
	this->x = x;
	this->y = y;
	next = NULL;
}

void Segment::createNewSegment(int dir) {
	// If not a tail, propagate the create operation to the next segment
	
	if (next != NULL) {
		next->createNewSegment(dir); // Call this function on the next segment
	}
	else { // Reached tail: create a new segment adjacent to the current tail based on movement direction
		switch (dir) {
		case UP:
			next = new Segment(x, y - 1);
			break;
		case RIGHT:
			next = new Segment(x - 1, y);
			break;
		case DOWN:
			next = new Segment(x, y + 1);
			break;
		case LEFT:
			next = new Segment(x + 1, y);
			break;
		}
	}
}

void Segment::addXY(int x, int y) {
	// Store current coordinates in temporaries so the next segment can follow
	int a = this->x; // old x
	int b = this->y; // old y

	// Move only in one axis to avoid diagonal movement

		if (x != 0)
			this->x += x;
		else
			this->y += y;

		if (next != NULL && this->x == next->getX() && this->y == next->getY()) {
			this->x = a;
			this->y = b;
		}
		else if (next != NULL)
			next->updateXY(a, b);
	// Propagate previous head location to the next segment so the body follows
		
}

int Segment::checkCoords(int a, int b) {
	// Count whether this segment occupies (a,b)
	int count = 0;
	if (a == x && b == y)
		++count; // increment if this segment matches

	// If there is no next segment, return the current count; otherwise add the next segment's result
	if (next == NULL)
		return count; // Stop when there are no more segments
	else
		return count + next->checkCoords(a, b); // recursive check on next segments
}

bool Segment::outOfBounds(int width, int height) {
	// Determine if this segment is outside the grid rectangle
	bool isOut = false;
	if (x < 0 || x >= width)
		isOut = true;
	if (y < 0 || y >= height)
		isOut = true;

	// If this segment is out of bounds return true, otherwise check remaining segments
	if (isOut)
		return isOut;
	if (next != NULL) {
		return isOut || next->outOfBounds(width, height);
	}
	else {
		return isOut;
	}
}

Segment::~Segment() {
	// Recursively delete the next segment(s) to free the entire linked list
	if (next != NULL) {
		delete next;
		next = NULL;
	}
}

// Getter functions
int Segment::getX() const { return x; };
int Segment::getY() const { return y; };