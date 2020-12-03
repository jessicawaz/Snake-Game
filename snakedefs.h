/**********************************************************
* file: snakedefs.h
* by: Jessica Wazbinski
* org: COP 2001, Spring 2020
* for: Global configuration data for the snake game
**********************************************************/
#pragma once

#include "console.h"

//----------------------------------------------------
//Global Constants and Data Structures
//----------------------------------------------------
//console window config
const int SCREEN_HEIGHT = 50;
const int SCREEN_WIDTH = 50;
const int DRAW_SCALE = 10;
//starting position for snake
const int X_START = 25;
const int Y_START = 25;

const Color BACKGROUND_COLOR = Black;
const Color SNAKE_COLOR = Magenta;
const Color FOOD_COLOR = Cyan;

enum Direction {
	None,
	Left,
	Right,
	Up,
	Down,
	Quit = -1
};