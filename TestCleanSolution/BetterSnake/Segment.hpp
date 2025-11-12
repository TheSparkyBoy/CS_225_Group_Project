/****************************************************************************
 * File:   Segment.hpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	 Headers and description for the Segment Class
 * Version:  1.0 Oct 22, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/
#pragma once
#define SEGMENT_HPP
 // For NULL ptr
#pragma once
#include <cstdlib>

/*
 * Direction constants used throughout the codebase
 * UP, RIGHT, DOWN, LEFT are in clockwise order; STOP is used to halt movement.
 */
enum {
	UP,
	RIGHT,
	DOWN,
	LEFT,
	STOP
};

/*
 * Segment represents one cell of the snake. Segments are linked using the
 * protected pointer 'next' so the Snake/Segment implementation forms a linked list
 * from head to tail.
 * 
 * The Segment class implements movement on the grid and provides
 * functions capable of iterating through the linked list for growth or collision detection
 */
class Segment {
private:
	int x; // grid X coordinate of this segment
	int y; // grid Y coordinate of this segment
protected: // Only to be called by one segment on another segment
	Segment* next; // Points to the next segment on the snake. Tail segment points to NULL.

	/* updateXY
	 * Move this segment to coordinates (a,b). Called recursively on next segments
	 * so tail segments follow their predecessor.
	 */
	void updateXY(int a, int b);
public:
	// Segment constructor: New segment created at (x,y) points to NULL (aka, becomes the tail segment)
	Segment(int x, int y);

	/* createNewSegment
	 * Create a new segment at the end of the snake linked list. The 'dir' parameter
	 * indicates which direction the snake is moving so the new tail is placed on the correct side.
	 */
	virtual void createNewSegment(int dir);

	/* addXY
	 * Move the head segment by adding (x,y) (only one of x or y will be non-zero).
	 * After moving the head, propagate previous head position to the next segment so body follows.
	 */
	void addXY(int x, int y);

	/* checkCoords
	 * Count the number of segments that occupy coordinates (a,b). Useful for detecting collisions.
	 * Returns number of segments at that location (0, 1, or more).
	 */
	int checkCoords(int a, int b);

	// Check whether any segment is out of the bounds [0,width) x [0,height)
	bool outOfBounds(int width, int height);

	// Destructor: free the remaining linked list of segments
	~Segment();

	// Getter functions for segment coordinates
	int getX() const;
	int getY() const;
};