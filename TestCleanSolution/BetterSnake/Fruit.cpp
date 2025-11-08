/****************************************************************************
 * File:	 Fruit.cpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	 Implements the Fruit class
 * Version:  1.0 Nov 06 2025
 * Resources: NA
 *******************************************************************************/
#include<ctime>
#include <cstdlib>
#include "Fruit.hpp"


Fruit::Fruit(int w, int h, Snake& snk, int f,  char r, char g, char b, char alpha) :gc(f) {
	do {
		x = rand() % w + 1;
		y = rand() % h + 1;
	} while (snk.checkCoords(x, y) > 0);
	cv[0] = r; cv[1] = g; cv[2] = b; cv[3] = alpha;
}

char Fruit::getRGBA(int i) const { return cv[i]; };
int Fruit::getGC() const { return gc; };
int Fruit::getX() const { return x; };
int Fruit::getY() const { return y; };
