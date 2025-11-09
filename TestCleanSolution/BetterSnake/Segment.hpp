/****************************************************************************
 * File:   Segment.hpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	 Headers and description for the Segment Class
 * Version:  1.0 Oct 22, 2025
 * Resources: NA
 *******************************************************************************/
#pragma once
#define SEGMENT_HPP
 // For NULL ptr
#pragma once
#include <cstdlib>

enum {
	UP,
	RIGHT,
	DOWN,
	LEFT,
};

class Segment {
private:
	int x;
	int y;
protected: // Only to be called by one segment on another segment
	Segment* next; //Points to the next segment on the snake. 'Tail' segment points to NULL.
	/* Function: updateXY
	 * Purpose: Move the next segment's location to this segment's location
	 * Inputs: New location (a,b) for the called segment = former location of the calling segment.
	 */
	void updateXY(int a, int b);
public:
	//Segment constructor: New segment created at (x,y) points to NULL (aka, becomes the tail segment)
	Segment(int x, int y);
	/* Function: CreateNewSegment
	 * Purpose: create a new Segment at the end of the snake.
	 */
	virtual void createNewSegment(int dir);
	/* Function: addXY
	 * Purpose: move the head segment in one direction at a time
	 * Inputs: (x,y) coordinates to add, x: [-1,1], y: [-1,1]
	 */
	void addXY(int x, int y);
	/* Function: checkCoords
	 * Purpose: Count the number of segments at a given location.
	 * Input: A coordinate point (a,b)
	 * Output: The # of segments at location (a,b)
	 */
	int checkCoords(int a, int b);
	//Should iterate down the linked list and free all allocated memory.

	bool outOfBounds(int width, int height);
	~Segment();
	//Getter functions
	int getX() const;
	int getY() const;
};