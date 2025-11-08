#include "Snake.hpp"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

#define WIDTH 20
#define HEIGHT 20
#define GRID_SZ 24

struct Appstate {
	SDL_Window* window;
	SDL_Renderer* renderer;
	Snake* s;
};

SDL_AppResult SDL_AppInit(Appstate& as) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK))
		return SDL_APP_FAILURE;

	if (!SDL_CreateWindowAndRenderer("Better Snake", WIDTH * GRID_SZ, HEIGHT * GRID_SZ, 0, &as.window, &as.renderer))
		return SDL_APP_FAILURE;
	SDL_SetRenderLogicalPresentation(as.renderer, WIDTH * GRID_SZ, HEIGHT * GRID_SZ, SDL_LOGICAL_PRESENTATION_LETTERBOX);
	as.s = new Snake(WIDTH / 2, HEIGHT / 2);
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

SDL_AppResult SDL_AppEvent(Appstate& as, SDL_Event* event) {
	switch (event->type) {
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
		break;
	case SDL_EVENT_KEY_DOWN:
		return key_press(as.s, event->key.scancode);
		break;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_App_Iterate(Appstate& as) {
	SDL_FRect r;
	r.w = r.h = GRID_SZ;
	int sfill;
	SDL_SetRenderDrawColor(as.renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(as.renderer);

	for (int i = 1; i <= WIDTH; ++i) {
		for (int j = 1; j <= HEIGHT; ++j) {
			sfill = as.s->checkCoords(i, j);

			//If at this chordinate the are more than one segment, then game ends
			if (sfill > 1) {
				return SDL_APP_SUCCESS;
			}
			if (as.s)
		}
	}
}

void SDL_AppQuit(Appstate& as) {
	SDL_DestroyRenderer(as.renderer);
	SDL_DestroyWindow(as.window);
	delete(as.s);
}

int main() {
	Snake s(WIDTH / 2, HEIGHT / 2);

	return 0;
}