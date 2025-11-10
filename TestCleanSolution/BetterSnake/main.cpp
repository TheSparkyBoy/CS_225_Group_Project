/****************************************************************************
 * File:	main.cpp
 * Author:  Yunfeng Nie, Konnor Barnes
 * Purpose:	Implements the snake class and supporting functions to the segments.
 * Version:  1.0 Nov 8, 2025
 * Resources: GitHub Copilot for comment generation
 *******************************************************************************/

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#pragma	once
#include "Snake.hpp"
#include "Fruit.hpp"
#include "GameOver.hpp"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <vector>

using namespace std;

#define WIDTH 20
#define HEIGHT 20
#define GRID_SZ 40
#define MAX_FRUITS 5
#define GAME_SPEED 100 //ms

// Application state stored and passed to SDL callbacks.
// Holds window/rendering pointers, the current snake, active fruits and game over helper.
typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Snake* s;           // pointer to the snake head object
	vector<Fruit> f;    // active fruits on the field
	GameOver go;        // helper for recording game over/score
} Appstate;

// SDL_AppInit: called once by SDL at startup to initialize app-level resources.
// - allocates and initializes Appstate
// - creates SDL window and renderer
// - spawns initial snake segments
SDL_AppResult SDL_AppInit(void** apstate, int argc, char* argv[]) {
	srand(time(NULL));
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK))
		return SDL_APP_FAILURE;

	Appstate* as = new Appstate;
	if (!as) {
		return SDL_APP_FAILURE;
	}

	*apstate = as;

	// Create window + renderer sized to the logical grid
	if (!SDL_CreateWindowAndRenderer("Better Snake", WIDTH * GRID_SZ, HEIGHT * GRID_SZ, 0, &as->window, &as->renderer))
		return SDL_APP_FAILURE;

	// Configure the renderer's logical presentation to keep pixel-aligned grid
	SDL_SetRenderLogicalPresentation(as->renderer, WIDTH * GRID_SZ, HEIGHT * GRID_SZ, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	// Create the snake centered on the grid and give it two extra segments
	as->s = new Snake(WIDTH / 4, HEIGHT / 2);
	as->s->createNewSegment();
	as->s->createNewSegment();

	return SDL_APP_CONTINUE;
}

// key_press: handle keyboard input that affects the snake
// Note: receives the Snake pointer by reference so resets replace the pointer held in Appstate.
SDL_AppResult key_press(Snake*& s, SDL_Scancode key) {
	switch (key) {
	case SDL_SCANCODE_ESCAPE:
	case SDL_SCANCODE_Q:
		// Request application quit
		return SDL_APP_SUCCESS;
	case SDL_SCANCODE_R:
		// Reset the snake: free old and allocate a fresh one centered on the grid.
		delete s;
		s = new Snake(WIDTH / 4, HEIGHT / 2);
		s->createNewSegment();
		s->createNewSegment();
		break;
	case SDL_SCANCODE_RIGHT:
	case SDL_SCANCODE_D:
		s->setDirection(RIGHT);
		break;
	case SDL_SCANCODE_DOWN:
	case SDL_SCANCODE_S:
		s->setDirection(DOWN);
		break;
	case SDL_SCANCODE_LEFT:
	case SDL_SCANCODE_A:
		s->setDirection(LEFT);
		break;
	case SDL_SCANCODE_UP:
	case SDL_SCANCODE_W:
		s->setDirection(UP);
		break;
	}
	return SDL_APP_CONTINUE;
}

// SDL_AppEvent: SDL will call this when events are available.
// We forward key-down events to key_press and handle quit events.
SDL_AppResult SDL_AppEvent(void* apstate, SDL_Event* event) {

	Appstate* as = static_cast<Appstate*>(apstate);
	switch (event->type) {
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
		break;
	case SDL_EVENT_KEY_DOWN:
		// Forward keyboard scancode to input handler (updates as->s when resetting)
		return key_press(as->s, event->key.scancode);
		break;
	}
	return SDL_APP_CONTINUE;
}

// SDL_AppIterate: main per-frame (or per-iteration) update + draw.
// - may spawn fruits randomly
// - moves the snake
// - renders the grid: segments, fruits and head
// - handles fruit consumption and removal
SDL_AppResult SDL_AppIterate(void* appstate) {

	Appstate* as = static_cast<Appstate*>(appstate);

	// Rectangle used for drawing each grid cell
	SDL_FRect r;
	r.w = r.h = GRID_SZ;
	int sfill;
	int numFruits = rand() % MAX_FRUITS;

	// Randomly add fruits (simple heuristic: push a fruit if we currently have fewer fruits than a newly chosen target)
	// This keeps fruit count dynamic but bounded by MAX_FRUITS.
	if (as->f.size() < numFruits) {
		as->f.push_back(Fruit(WIDTH, HEIGHT, *as->s, 1, 255 - rand()%50, rand()%50, rand()%50, SDL_ALPHA_OPAQUE));
	}

	// Move the snake according to its current direction; if collision occurs the snake will be set to STOP downstream
	if (as->s->moveSnake(WIDTH, HEIGHT)) {
		// On collision we stop movement (game over handling elsewhere)
		as->s->setDirection(STOP);
	}

	// Clear screen to black
	SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(as->renderer);

	// Draw all segments by scanning the grid and asking the snake how many segments are at each coordinate.
	// Using checkCoords makes overlapping / self-collision detection easy.
	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j < HEIGHT; ++j) {
			sfill = as->s->checkCoords(i, j);

			// Compute draw position for this grid cell
			r.x = float(i * GRID_SZ);
			r.y = float(j * GRID_SZ);

			// If the snake has stopped (collision/game-over), highlight the cell red and record score
			if (sfill == 0 && as->s->getDirection() == STOP) {
				SDL_SetRenderDrawColor(as->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
				// Record game-over score to file (GameOver helper)
				as->go.recordToFile(as->s->getLength());
			}
			else {
				// Default background color for empty cells
				SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			}

			// Draw body segments with a distinct color
			if (sfill == 1) {
				SDL_SetRenderDrawColor(as->renderer, 0, 128, 128, SDL_ALPHA_OPAQUE);
			}

			SDL_RenderFillRect(as->renderer, &r);
		}
	}

	// Draw fruits and handle consumption.
	// We iterate forward and erase elements when eaten, adjusting index after erase.
	for (int i = 0; i < (int)as->f.size(); ++i) {
		Fruit& fruit = as->f[i];
		r.x = float(fruit.getX() * GRID_SZ);
		r.y = float(fruit.getY() * GRID_SZ);

		// Use fruit's stored RGBA bytes for color
		SDL_SetRenderDrawColor(as->renderer, fruit.getRGBA(0), fruit.getRGBA(1), fruit.getRGBA(2), fruit.getRGBA(3));
		SDL_RenderFillRect(as->renderer, &r);

		// If snake head is on the fruit, grow snake and remove the fruit from the vector
		if (fruit.getX() == as->s->getX() && fruit.getY() == as->s->getY()) {
			// Grow by adding a new tail segment
			as->s->createNewSegment();
			// Remove fruit safely and step index back to account for vector shift
			as->f.erase(as->f.begin() + i);
			--i; // Decrement i to keep iterating the remaining fruits correctly
		}
	}

	// Draw the head segment last in a bright color so it stands out
	r.x = float(as->s->getX() * GRID_SZ);
	r.y = float(as->s->getY() * GRID_SZ);
	SDL_SetRenderDrawColor(as->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(as->renderer, &r);

	// Present the composed frame and sleep to control game speed
	SDL_RenderPresent(as->renderer);
	SDL_Delay(GAME_SPEED); //Control speed of the game
	return SDL_APP_CONTINUE;
}

// SDL_AppQuit: free resources allocated in SDL_AppInit
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	if(appstate != NULL){
		Appstate* as = static_cast<Appstate*>(appstate);
		SDL_DestroyRenderer(as->renderer);
		SDL_DestroyWindow(as->window);
		// Note: GameOver object is a value member and will be destroyed automatically.
		// Delete snake pointer and free Appstate memory.
		delete as->s;
		delete(as);
	}
}