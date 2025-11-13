/****************************************************************************
 * File:	 Fruit.hpp
 * Author:   Konnor Barnes, Yunfeng Nie
 * Purpose:	 Headers and description for the Fruit Class
 * Version:  1.0 Nov 06, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/
#pragma once
#define FRUIT_HPP
#include "Snake.hpp"

/*
 * Fruit represents a consumable item placed on the grid.
 * - x,y: coordinates on the grid
 * - gc: growth coefficient (how much the snake should grow/shrink on consumption)
 *	- This represents an area for future feature development. Like "poison fruits" that shrink the snake or have other effects
 * - cv: color/alpha bytes [R,G,B,alpha]
 *
 * The constructor ensures fruit does not spawn on the snake by querying snk.checkCoords.
 */
class Fruit {
private: 
	int x;
	int y;
	int gc; // Growth coefficient - how much the snake grows or shrinks when it consumes the fruit.
	char cv[4]; // [R,G,B, alpha] for rendering the color of the fruit. Values from 0 - 255 only so stored in char (8-bits)

public:
	// Create a fruit within width (w) x height (h) that does not overlap the provided snake.

	// Default color is red (255,0,0) and opaque. 
	Fruit(int w, int h, Snake& snk, int f = 1, char r = 255, char g = 0, char b = 0, char alpha = 255);



	// Return the RGBA component (index 0..3)
	char getRGBA(int i) const;

	// Growth coefficient accessor
	int getGC() const;

	// coordinate accessors
	int getX() const;
	int getY() const;
};