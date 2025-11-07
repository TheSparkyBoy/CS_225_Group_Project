/****************************************************************************
 * File:	 Fruit.cpp
 * Author:   Konnor Barnes
 * Purpose:	 Implements the Fruit class
 * Version:  1.0 Nov 06 2025
 * Resources: NA
 *******************************************************************************/
#include<ctime>
#include <cstdlib>
#include "Fruit.hpp"

int x;
int y;
int gc;
char cv[4];

Fruit::Fruit(int w, int h, Snake& snk, int f,  char r, char g, char b, char alpha) :gc(f) {
	srand(time(NULL));
	do {
		x = rand() % w + 1;
		y = rand() % h + 1;
	} while (snk.checkCoords(x, y) > 0);
	cv[0] = r; cv[1] = g; cv[2] = b; cv[3] = alpha;
}

char Fruit::getRGBA(int i) { return cv[i]; };
int Fruit::getGC() { return gc; };
