/**********************************************************
* file: snakemain.cpp
* by: Jessica Wazbinski
* org: COP 2001, Spring 2020
* for: Main application file for the snake game, performs
*	   initialization, game startup and shutdown
**********************************************************/

#include <iostream>
#include <time.h>		//time
#include <stdlib.h>		//srand, rand

#include "snake.h"

using namespace std;

//--------------------------------------------------------------
//function prototypes
//--------------------------------------------------------------
void update(Snake& snake, Block& food);
void render(HDC hdc, Snake snake, Block food);
Block nextFood(Snake snake);

int main() {	//start here

	HDC console = initConsole(SCREEN_WIDTH, SCREEN_HEIGHT, DRAW_SCALE);

	drawBorders(console, SCREEN_WIDTH, SCREEN_HEIGHT, DRAW_SCALE, Red);
	drawBorders(console, SCREEN_WIDTH, SCREEN_HEIGHT, DRAW_SCALE, Red);

	//initialize random numbers
	srand(time(NULL));

	Snake snake = Snake(X_START, Y_START);

	//current food location
	Block food = nextFood(snake);

	//main game loop
	do {
		//process player input
		if (snake.getNextDirection() == None) {
			snake.setDirections(getGameKeys());	//function chaining
		}

		//update game objects
		if (snake.getNextDirection() != Quit) {
			update(snake, food);
		}

		//draw the game
		render(console, snake, food);

		Sleep(.05 * 1000); //pause for 5th of a second (Sleep included in windows)

	} while (snake.getNextDirection() != Quit); //end main game loop

	drawBorders(console, SCREEN_WIDTH, SCREEN_HEIGHT, DRAW_SCALE, Red);

	return 0;
} //end main

/**********************************************************
Check if snake collides with walls or itself
Parameters:
	snake - (out) snake structure
	food  - (out) food block
Returns:
	void
***********************************************************/
void update(Snake& snake, Block& food) {

	if (snake.getNextDirection() != None) {
		snake.turn();
	}

	//see if snake has started moving yet
	if (snake.isMoving()) {
		snake.move();

		//see if snake ate food
		if (snake.getHead()->equals(food)) {
			//grow snake
			snake.grow();

			//get a new food block
			food = nextFood(snake);
			
		}
		else {
			//see if snake has any collision
			snake.collisions();
		}

	} //end isMoving

	return;
}

/**********************************************************
Draws a snake block on the console window
 Parameters:
	hdc     - handle to the console window for drawing
	snake   - snake structure
	food    - current food block
 Returns:
	void
***********************************************************/
void render(HDC hdc, Snake snake, Block food) {

	//draw food block
	draw(hdc, food.getColumn(), food.getRow(), DRAW_SCALE, FOOD_COLOR);

	//erase previous tail of snake
	if (!snake.getPrevTail()-> isEmpty()) {
		draw(hdc, snake.getPrevTail()->getColumn(), snake.getPrevTail()->getRow(), DRAW_SCALE, BACKGROUND_COLOR);
	}

	//draw head of snake
	draw(hdc, snake.getHead()->getColumn(), snake.getHead()->getRow(), DRAW_SCALE, SNAKE_COLOR);

	return;
}

/**********************************************************
Generate random location for new food
Parameters:
	snake   - snake class object
 Returns:
	Block   - location of next food
***********************************************************/
Block nextFood(Snake snake) {
	Block food = Block();

	while (food.isEmpty()) {

		food.setColumn(rand() % SCREEN_WIDTH);		//random column
		food.setRow(rand() % SCREEN_HEIGHT);		//random row

		//check if snake occupies this location
		if (snake.intersects(food, true)) {
			//set food to empty
			food = Block();
		}
	}
	
	return food;
}