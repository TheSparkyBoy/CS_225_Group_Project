/****************************************************************************
 * File:   Snake.hpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	 Snake hpp file defining the Snake class and supporting functions to the segments.
 * Version:  1.0 Oct 22, 2025
 * Resources: NA
 *******************************************************************************/
#pragma once
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