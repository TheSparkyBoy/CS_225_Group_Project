/****************************************************************************
 * File:	 Fruit.cpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	 Implements the Fruit class
 * Version:  1.0 Nov 06 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/
#include<ctime>
#include <cstdlib>
#include "Fruit.hpp"

// Fruit constructor:
// - randomly place fruit within [1..w] and [1..h] until a free cell (not occupied by the snake) is found
// - initialize color bytes and growth coefficient
Fruit::Fruit(int w, int h, Snake& snk, int f,  char r, char g, char b, char alpha) :gc(f) {
	do {
		x = rand() % w; //grid is indexed (0, w -1). This returns a number in that range.
		y = rand() % h; // 
	} while (snk.checkCoords(x, y) > 0); // avoid spawning on the snake
	cv[0] = r; cv[1] = g; cv[2] = b; cv[3] = alpha;
}

// Accessors
char Fruit::getRGBA(int i) const { return cv[i]; };
int Fruit::getGC() const { return gc; };
int Fruit::getX() const { return x; };
int Fruit::getY() const { return y; };
