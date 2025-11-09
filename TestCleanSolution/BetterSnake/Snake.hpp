/****************************************************************************
 * File:   Snake.hpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	 Snake hpp file defining the Snake class and supporting functions to the segments.
 * Version:  1.0 Oct 22, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/
#pragma once
#include "Segment.hpp"
#include <iostream>

/*
 * Snake extends Segment to represent the player-controlled snake head.
 * The snake itself uses a linked-list of Segment objects (head -> ... -> tail)
 * Operations implemented:
 *  - createNewSegment(): append a new tail segment
 *  - moveSnake(): move the head and propagate motion down the list; detect collisions
 *  - set/get direction, length accessors
 */
class Snake: public Segment {
private:
	// The head is the segment that will be directly controlled by the player.
	int length; //# of Segments in the Snake.
	int direction; // current movement direction: UP, RIGHT, DOWN, LEFT, STOP

public:
	// Construct a snake with its head at (x,y). Initial length = 1 (head only)
	Snake(int x, int y);

	// Grow the snake by adding a new segment at the tail
	void createNewSegment();

	// Move the snake one step in its current direction. Returns true if a collision occurred.
	bool moveSnake(int w, int h);

	// Destructor (prints score on destruction in this implementation)
	~Snake();

	// Get how many segments the snake currently has
	int getLength() const;

	// Change movement direction (with logic to prevent reversing into itself)
	void setDirection(int dir);

	// Read current direction
	int getDirection() const;
};