/****************************************************************************
 * File:   Snake.cpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	Implements the snake class and supporting functions to the segments.
 * Version:  1.0 Oct 22, 2025
 * Resources: NA
 *******************************************************************************/
#include "Snake.hpp"

	// The head is the segment that will be directly controlled by the player. See Segment.cpp for details.
	int length; //# of Segments in the Snake.

	Snake::Snake(int x, int y): Segment(x,y) {
		length = 1;
		direction = RIGHT;
	}

	void Snake::createNewSegment() {
		++length;
		if (next != NULL) { //If this is not the tail of the snake
			next->createNewSegment(direction); // Call this function on the next segment
		}
		else { //By the time we get here, we have reached the end of the snake
			next = new Segment(getX() - 1, this->getY()); //Create a new segment next to the tail segment. That segment becomes the new tail.
		}

	}

	bool Snake::moveSnake(int w, int h) {
		bool hasCollided = false;
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
			//Nothing happens, snake doesn't move.
			break;
		}
		
		if (checkCoords(getX(), getY()) > 1) {
			hasCollided = true;
		}
		else 
			hasCollided = outOfBounds(w, h);
		return hasCollided;
	}

	Snake::~Snake() {
		std::cout << "Oops! Your snake ate itself! Length: " << length << ".\n"; // Print score statement.
	}
	int Snake::getLength() const { return length; }

	void Snake::setDirection(int dir) {
		if (dir % 2 == direction % 2) {
			direction = direction;
		}
		else {
			direction = dir;
		}
		if (dir == STOP) {
			direction = STOP;
		}
	}

	int Snake::getDirection() const {
		return direction;
	}