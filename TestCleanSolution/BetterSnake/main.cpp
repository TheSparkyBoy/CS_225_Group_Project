#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Snake.cpp"


typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
} AppState;





static SDL_AppResult handle_key_event(SDL_Scancode key_code)
{
    switch (key_code) {
        /* Quit. */
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        return SDL_APP_SUCCESS;
        /* Restart the game as if the program was launched. */
    case SDL_SCANCODE_R:
        // Restart
        break;
        /* Decide new direction of the snake. */
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_D:
        //go right
        break;
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_W:
        // go up
        break;
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_A:
        // go left
        break;
    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_S:
        // go down
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(SDL_Event* event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        return handle_key_event(event->key.scancode);
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (appstate != NULL) {
        AppState* as = (AppState*)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
}