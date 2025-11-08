/****************************************************************************
 * File:	 Fruit.hpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	 Headers and description for the Fruit Class
 * Version:  1.0 Nov 06, 2025
 * Resources: NA
 *******************************************************************************/
#pragma once
#define FRUIT_HPP
#include "Snake.hpp"

class Fruit {
private: 
	int x;
	int y;
	int gc; //Growth coefficient - how much the snake grows or shrinks when it consumes the fruit.
	char cv[4]; // [R,G,B, alpha] for rendering the color of the fruit. Values from 0 - 255 only so stored in char (8-bits)

public:
	Fruit(int w, int h, Snake& snk, int f = 1, char r = 255, char g = 0, char b = 255, char alpha = 255);
	char getRGBA(int i) const;
	int getGC() const;
	int getX() const;
	int getY() const;
};