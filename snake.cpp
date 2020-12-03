/**********************************************************
* file: snake.cpp
* by: Jessica Wazbinski
* org: COP 2001, Spring 2020
* for: Implementation file for a snake class object
**********************************************************/
#include "snake.h"

using namespace std;

//-------------------------------------------
// Constructors
//-------------------------------------------

/**********************************************************
Default constructor of an empty snake wehere all variables
czro or null 
Parameters:
Returns:
***********************************************************/
Snake::Snake() {
	head = Block();			// root of the snake body
	tail = &head;			//tail points to current head
	prevTail = Block();		//no previous tail (empty)

	speed = Block(0, 0);	//speed = 0
	size = 1;				//1 block (head)

	current = None;			//not moving
	next = None;			//no player input
}

/**********************************************************
Set a new snake with head at starting column & row
Parameters:
	startColumn		- x-coord for head
	startRow		- y-coord for head
Returns:
***********************************************************/
Snake::Snake(int startColumn, int startRow) {
	head = Block(startColumn, startRow);	// root of the snake body
	tail = &head;							//tail points to current head
	prevTail = Block();						//empty previous tail
	prevTail.copy(tail);					//set same as tail

	speed = Block(0, 0);					//speed = 0
	size = 1;								//1 block (head)

	current = None;							//not moving
	next = None;							//no player input
}

//-------------------------------------------
// Accessors
//-------------------------------------------

/**********************************************************
Returns the pointer to the head of the snake
Parameters:
Returns:
	Block*	- head block
***********************************************************/
Block* Snake::getHead() {
	return &head;		//returns address of head b/c we need pointer but head isnt a pointer
}

/**********************************************************
Returns the pointer to the tail of the snake
Parameters:
Returns:
	Block*	- tail block
***********************************************************/
Block* Snake::getTail() {
	return tail;
}

/**********************************************************
Returns the pointer to the previous tail of the snake to
erase it or append it if growing
Parameters:
Returns:
	Block*	-  previous tail block
***********************************************************/
Block* Snake::getPrevTail() {
	return &prevTail;
}

/**********************************************************
Returns the current size of snake in blocks
Parameters:
Returns:
	int		- size of snake
***********************************************************/
int Snake::getSize() {
	return size;
}

/**********************************************************
Returns current direction snake is moving in
Parameters:
Returns:
	Direction	- current direction enum, None, or Quit
***********************************************************/
Direction Snake::getCurrentDirection() {
	return current;
}

/**********************************************************
Returns next direction snake should move based on user 
input
Parameters:
Returns:
	Direction	- next direction enum, None, or Quit
***********************************************************/
Direction Snake::getNextDirection() {
	return next;
}

//-------------------------------------------
// Methods
//-------------------------------------------

/**********************************************************
Set next direction for the snake based on player input
Parameters:
	input	- console keyboard code
Returns:
	void
***********************************************************/
void Snake::setDirections(int input) {

	switch (input) {
	case VK_A:
	case VK_LEFT:
		if (current != Left && current != Right)
			next = Left;
		break;
	case VK_D:
	case VK_RIGHT:
		if (current != Left && current != Right)
			next = Right;
		break;
	case VK_W:
	case VK_UP:
		if (current != Up && current != Down)
			next = Up;
		break;
	case VK_S:
	case VK_DOWN:
		if (current != Up && current != Down)
			next = Down;
		break;
	case VK_X:
	case VK_ESCAPE:
		next = Quit;
		break;
	default:
		next = None;
	} //end of switch

	return;
}

/**********************************************************
Adjust speed of the snake based on directions to turn
the snake
Parameters:
Returns:
	void
***********************************************************/
void Snake::turn() {

	//update directions
	current = next;
	next = None;

	//update velocities for change in direction 
	switch (current) {
	case Left:
		speed.setColumn(-1);
		speed.setRow(0);
		break;
	case Right:
		speed.setColumn(1);
		speed.setRow(0);
		break;
	case Up:
		speed.setColumn(0);
		speed.setRow(-1);
		break;
	case Down:
		speed.setColumn(0);
		speed.setRow(1);
		break;
	}

	return;
}

/**********************************************************
Move snake based on its speed
Parameters:
Returns:
	void
***********************************************************/
void Snake::move() {

	//save previous tail x,y location
	prevTail.copy(tail);

	//move up snake from tail to head copying x,y coordinates
	//from block in front to current block
	Block* temp = tail;
	while (temp) {

		//if block in front of this one
		if (temp->getForward()) {
			//then copy its values to this one
			temp->copy(temp->getForward());
		}

		//move temp pointer forward in snake
		temp = temp->getForward();
	}

	//adjust snake head for speed
	head.setColumn(head.getColumn() + speed.getColumn());
	head.setRow(head.getRow() + speed.getRow());

	return;
}

/**********************************************************
Determine if the snake had a non-zero speed set
Parameters:
Returns:
	bool	- true if speed is not zero
***********************************************************/
bool Snake::isMoving() {\

	Block zero = Block(0, 0);

	return !speed.equals(zero);
}

/**********************************************************
Add a block on the tail of the snake when it eats food
Parameters:
Returns:
	void
***********************************************************/
void Snake::grow() {

	//increase snake size
	size++;

	//add old tail location back onto body at new index
	tail->append(&prevTail);

	//move tail pointer to new tail block
	tail = tail->getBackward();

	//make tail empty so we don't erase it
	prevTail = Block();

	return;
}

/**********************************************************
See if snake collides with walls or itself and set next 
direction to quite if it does
Parameters:
Returns:
	void
***********************************************************/
void Snake::collisions() {
	bool collision = false;

	//horiz collision checks
	if (head.getColumn() <= 0) {				 // if hit left
		collision = true;
		head.setColumn(0);
	}
	else if (head.getColumn() >= SCREEN_WIDTH) { //if hit right
		collision = true;
		head.setColumn(SCREEN_WIDTH - 1);
	}

	//vert collision checks
	if (head.getRow() <= 0) {					 //if hit top
		collision = true;
		head.setRow(0);
	}
	else if (head.getRow() >= SCREEN_HEIGHT) {	//if hit bottom
		collision = true;
		head.setRow(SCREEN_HEIGHT - 1);
	}

	//check if snake hit itself
	if (!collision) 
		collision = intersects(head, false);

	// if collision then update head
	if (collision) 
		next = Quit;
}

/**********************************************************
Determine if a block intersects with the snake body
Parameters:
	other		- the block to check
	withHead	- true if block is compared with head of
				  snake
Returns:
	bool		- true if block intersects body of snake
***********************************************************/
bool Snake::intersects(Block other, bool withHead) {
	bool collide = false;

	Block* temp = &head;

	//if not checking head then move pointer backwards
	if (!withHead) {
		temp = head.getBackward();
	}

	//move along body checking each block against other
	while (temp && !collide) {
		collide = temp->equals(other);

		//move pointer to next body block
		temp = temp->getBackward();
	}

	return collide;
}