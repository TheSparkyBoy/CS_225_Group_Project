/****************************************************************************
 * File:   Segment.cpp
 * Author:   Konnor Barnes
 * Purpose:	 Implements the Segment Class
 * Version:  1.0 Oct 22, 2025
 * Resources: NA
 *******************************************************************************/

#include "Segment.hpp"

Segment* next = NULL;
void Segment::updateXY(int a, int b) {
	if (next != NULL)
		next->updateXY(x, y); //Call on the next segment to move to this segment's coordinates
	x = a; //Move up to the given coordinates.
	y = b;
	
}

Segment::Segment(int x, int y) {
	this->x = x;
	this->y = y;
	next = NULL;
}

void Segment::createNewSegment(int dir) {
	if (next != NULL) { //If this is not the tail of the snake
		next->createNewSegment(dir); // Call this function on the next segment
	} 
	else { //By the time we get here, we have reached the end of the snake
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
	int a = this->x; //Create temp variables to hold the old coordinates (a,b)
	int b = this->y;
	if (x != 0) //This if-else structure prevents the snake from moving diagonally (two directions simultaneously)
		this->x += x;
	else
		this->y += y;
	next->updateXY(a, b); //Move the next segment to (a,b).
}
int Segment::checkCoords(int a, int b) {
	int count = 0;
	if (a == x && b == y)
		++count; //If the given coordinates match the segment coordinates, increment the counter.
	if (next == NULL)
		return count; // Stop when there are no more segments
	else
		return count + next->checkCoords(a, b); //check the next segment, return the total # of segments at this location.
}
bool Segment::outOfBounds(int width, int height) {
	bool isOut = false;
	if (x < 0 || x >= width)
		isOut = true;
	if (y < 0 || y >= height)
		isOut = true;
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
	//Should iterate down the linked list and free all allocated memory.
	if (next != NULL) {
		delete next;
		next = NULL;
	}
}
//Getter functions
int Segment::getX() const { return x; };
int Segment::getY() const { return y; };