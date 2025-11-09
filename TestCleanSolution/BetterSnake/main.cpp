/****************************************************************************
 * File:	Snake.cpp
 * Author:  Yunfeng Nie, Konnor Barnes
 * Purpose:	Implements the snake class and supporting functions to the segments.
 * Version:  1.0 Nov 8, 2025
 * Resources: NA
 *******************************************************************************/

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#pragma	once
#include "Snake.hpp"
#include "Fruit.hpp"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <vector>

using namespace std;

#define WIDTH 20
#define HEIGHT 20
#define GRID_SZ 24
#define MAX_FRUITS 10

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Snake* s;
	vector<Fruit> f;
} Appstate;

SDL_AppResult SDL_AppInit(void** apstate, int argc, char* argv[]) {
	srand(time(NULL));
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK))
		return SDL_APP_FAILURE;

	Appstate* as = new Appstate;
	if (!as) {
		return SDL_APP_FAILURE;
	}

	*apstate = as;

	if (!SDL_CreateWindowAndRenderer("Better Snake", WIDTH * GRID_SZ, HEIGHT * GRID_SZ, 0, &as->window, &as->renderer))
		return SDL_APP_FAILURE;

	SDL_SetRenderLogicalPresentation(as->renderer, WIDTH * GRID_SZ, HEIGHT * GRID_SZ, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	as->s = new Snake(WIDTH / 2, HEIGHT / 2);
	as->s->createNewSegment();
	as->s->createNewSegment();

	return SDL_APP_CONTINUE;
}

SDL_AppResult key_press(Snake* s, SDL_Scancode key) {
	switch (key) {
	case SDL_SCANCODE_ESCAPE:
	case SDL_SCANCODE_Q:
		return SDL_APP_SUCCESS;
	case SDL_SCANCODE_R:
		delete s;
		s = new Snake(WIDTH / 2, HEIGHT / 2);
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

SDL_AppResult SDL_AppEvent(void* apstate, SDL_Event* event) {

	Appstate* as = static_cast<Appstate*>(apstate);
	switch (event->type) {
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
		break;
	case SDL_EVENT_KEY_DOWN:
		return key_press(as->s, event->key.scancode);
		break;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {

	Appstate* as = static_cast<Appstate*>(appstate);

	SDL_FRect r;
	r.w = r.h = GRID_SZ;
	int sfill;
	int numFruits = rand() % MAX_FRUITS;
	bool collision = false;

	//Randomly add fruits
	if (as->f.size() < numFruits) {
		as->f.push_back(Fruit(WIDTH, HEIGHT, *as->s, 1, rand()%255 + 1, rand()%255 + 1, rand()%255 + 1, SDL_ALPHA_OPAQUE));
	}

	//Move snake
	collision = as->s->moveSnake(as->s->getDirection(), WIDTH, HEIGHT);

	SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(as->renderer);

	//Draw all segments
	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j < HEIGHT; ++j) {
			sfill = as->s->checkCoords(i, j);

			//If at this coordinate the are more than one segment, then game ends
			if (sfill > 1 || collision) {
				SDL_SetRenderDrawColor(as->renderer, 255, 0, 0, 5);
				SDL_RenderClear(as->renderer);
				SDL_RenderPresent(as->renderer);
			}
			if (sfill == 0) {
				//Nothing to draw
				continue;
			}
			r.x = float(i * GRID_SZ);
			r.y = float(j * GRID_SZ);

			if (sfill == 1) {
				SDL_SetRenderDrawColor(as->renderer, 0, 128, 128, SDL_ALPHA_OPAQUE);
			}

			SDL_RenderFillRect(as->renderer, &r);
		}
	}

	//Draw fruits
	for (int i = 0; i < as->f.size(); ++i) {
		Fruit& fruit = as->f[i];
		r.x = float(fruit.getX() * GRID_SZ);
		r.y = float(fruit.getY() * GRID_SZ);
		SDL_SetRenderDrawColor(as->renderer, fruit.getRGBA(0), fruit.getRGBA(1), fruit.getRGBA(2), fruit.getRGBA(3));
		SDL_RenderFillRect(as->renderer, &r);
		if (fruit.getX() == as->s->getX() && fruit.getY() == as->s->getY()) {
			//Snake eats the fruit
			as->s->createNewSegment();
			//Remove the fruit from the vector
			as->f.erase(as->f.begin() + i);
			--i; // Decrement i to account for the removed fruit
		}
	}

	//Draw head segment
	r.x = float(as->s->getX() * GRID_SZ);
	r.y = float(as->s->getY() * GRID_SZ);
	SDL_SetRenderDrawColor(as->renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(as->renderer, &r);

	SDL_RenderPresent(as->renderer);
	SDL_Delay(200); //Control speed of the game
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	if(appstate != NULL){
		Appstate* as = static_cast<Appstate*>(appstate);
		SDL_DestroyRenderer(as->renderer);
		SDL_DestroyWindow(as->window);
		delete(as);
	}
}