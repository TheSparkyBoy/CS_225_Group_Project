#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#pragma	once
#include "Snake.hpp"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#define WIDTH 20
#define HEIGHT 20
#define GRID_SZ 24

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Snake* s;
} Appstate;

SDL_AppResult SDL_AppInit(void** apstate, int argc, char* argv[]) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK))
		return SDL_APP_FAILURE;

	Appstate* as = (Appstate*)(SDL_calloc(1, sizeof(Appstate)));
	if (!as) {
		return SDL_APP_FAILURE;
	}

	*apstate = as;

	if (!SDL_CreateWindowAndRenderer("Better Snake", WIDTH * GRID_SZ, HEIGHT * GRID_SZ, 0, &as->window, &as->renderer))
		return SDL_APP_FAILURE;

	//SDL_SetRenderLogicalPresentation(as->renderer, WIDTH * GRID_SZ, HEIGHT * GRID_SZ, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	as->s = new Snake(WIDTH / 2, HEIGHT / 2);
	return SDL_APP_CONTINUE;
}

SDL_AppResult key_press(Snake* s, SDL_Scancode key) {
	switch (key) {
	case SDL_SCANCODE_ESCAPE:
	case SDL_SCANCODE_Q:
		return SDL_APP_SUCCESS;
	case SDL_SCANCODE_R:
		break;
	case SDL_SCANCODE_RIGHT:
	case SDL_SCANCODE_D:
		s->addXY(1, 0);
		break;
	case SDL_SCANCODE_DOWN:
	case SDL_SCANCODE_S:
		s->addXY(0, -1);
		break;
	case SDL_SCANCODE_LEFT:
	case SDL_SCANCODE_A:
		s->addXY(-1, 0);
		break;
	case SDL_SCANCODE_UP:
	case SDL_SCANCODE_W:
		s->addXY(0, 1);
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
	SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(as->renderer);

	//Draw all segments
	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j < HEIGHT; ++j) {
			sfill = as->s->checkCoords(i, j);

			//If at this coordinate the are more than one segment, then game ends
			if (sfill > 1) {
				return SDL_APP_SUCCESS;
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
	SDL_RenderPresent(as->renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	if(appstate != NULL){
		Appstate* as = static_cast<Appstate*>(appstate);
		SDL_DestroyRenderer(as->renderer);
		SDL_DestroyWindow(as->window);
		delete(as->s);
		SDL_free(as);
	}
}