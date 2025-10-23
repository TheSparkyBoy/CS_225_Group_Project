/****************************************************************************
* File:   WindowsManager.cpp
* Author:   Konnor Barnes, Yunfeng Nie
* Purpose:	Header file for WindowManager class
* Version:  1.0 Oct 22, 2025
* Resources: NA
*******************************************************************************/
#include "Snake.hpp"
#include <iostream>
#include <Windows.h>

using std::cout;


class snkWndw {
private:
	const int w;
	const int h;

public:
	//Use initialization list to initialize constant member variables
	snkWndw(int width, int height);

	/* Function: UpdateFrame
 * Purpose: Draw the snake (and eventually apples) in the game window. Will track collision in the future [WIP].
 * Inputs: A reference to a snake. the snkWndw must have access to the snake in order to display snake coordinates on screen.
 * Outputs: Whether a collision has occurred (true) or not (false)
 */
	bool UpdateFrame(const Snake& s);

};