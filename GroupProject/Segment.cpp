/* 
 * Segment.cpp
 * Implements the Segment Class.
 */

 // For NULL ptr
#include <cstdlib>

class Segment {
private:
	Segment* next = NULL; //Points to the next segment on the snake. 'Tail' segment points to NULL.
	int x;
	int y;
protected: // Only to be called by one segment on another segment
	void updateXY(int a, int b) {
		/* Function: updateXY
		 * Purpose: Move the next segment's location to this segment's location
		 * Inputs: New location (a,b) for the called segment = former location of the calling segment.
		 */
		next->updateXY(x, y); //Call on the next segment to move to this segment's coordinates

		x = a; //Move up to the given coordinates.
		y = b;
	}
public:
	Segment(int x, int y) {
		//Segment constructor: New segment created at (x,y) points to NULL (aka, becomes the tail segment)
		this->x = x;
		this->y = y;
		next = NULL;
	}
	void createNewSegment() {
		/* Function: CreateNewSegment
		 * Purpose: create a new Segment at the end of the snake.
		 */
		if (next != NULL) { //If this is not the tail of the snake
			next->createNewSegment(); // Call this function on the next segment
		} 
		else { //By the time we get here, we have reached the end of the snake
			next = new Segment(x - 1, y); //Create a new segment next to the tail segment. That segment becomes the new tail.
		}
		
	}

	void addXY(int x, int y) {
		/* Function: addXY
		 * Purpose: move the head segment in one direction at a time
		 * Inputs: (x,y) coordinates to add, x: [-1,1], y: [-1,1]
		 */
		int a = this->x; //Create temp variables to hold the old coordinates (a,b)
		int b = this->y;
		if (x != 0) //This if-else structure prevents the snake from moving diagonally (two directions simultaneously)
			this->x += x;
		else
			this->y += y;
		next->updateXY(a, b); //Move the next segment to (a,b).
	}
	int checkCoords(int a, int b) { 
		/* Function: checkCoords
		 * Purpose: Count the number of segments at a given location.
		 * Input: A coordinate point (a,b)
		 * Output: The # of segments at location (a,b)
		 */
		int count = 0;
		if (a == x && b == y)
			++count; //If the given coordinates match the segment coordinates, increment the counter.
		if (next == NULL)
			return count; // Stop when there are no more segments
		else
			return count + next->checkCoords(a, b); //check the next segment, return the total # of segments at this location.
	}
	
	~Segment() {
		//Should iterate down the linked list and free all allocated memory.
		if (next != NULL) {
			delete next;
			next = NULL;
		}
	}
	//Getter functions
	int getX() const { return x; };
	int getY() const { return y; };

};