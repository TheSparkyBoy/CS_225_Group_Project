/*
 * Logic implementation of the Snake game. It is designed to efficiently
 * represent the state of the game in memory.
 *
 * This code is public domain. Feel free to use it for any purpose!
 *
 * Added explanatory comments throughout to describe data layout, bit packing,
 * game rules, and how SDL main callbacks are used.
 */

#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

 /* Game timing and rendering constants */
#define STEP_RATE_IN_MILLISECONDS  125
#define SNAKE_BLOCK_SIZE_IN_PIXELS 24
#define SDL_WINDOW_WIDTH           (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_WIDTH)
#define SDL_WINDOW_HEIGHT          (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_HEIGHT)

/* Logical game grid size (width x height) and derived matrix size */
#define SNAKE_GAME_WIDTH  24U
#define SNAKE_GAME_HEIGHT 18U
#define SNAKE_MATRIX_SIZE (SNAKE_GAME_WIDTH * SNAKE_GAME_HEIGHT)

/* Bit/packing helpers:
 * - Snake cells are stored in a compact bit-packed array to reduce memory.
 * - Each cell consumes SNAKE_CELL_MAX_BITS bits (3 bits here) and can hold
 *   up to 8 different values (0..7). We only need 6 values (nothing, 4
 *   directions, and food).
 */
#define THREE_BITS  0x7U /* mask for 3 bits */
#define SHIFT(x, y) (((x) + ((y) * SNAKE_GAME_WIDTH)) * SNAKE_CELL_MAX_BITS)
 /* SHIFT(x,y) computes the bit-index for the cell at grid coordinates (x,y).
  * To access bytes we divide by 8; to access a specific bit offset inside a
  * byte we use (shift % 8).
  */

typedef enum
{
    SNAKE_CELL_NOTHING = 0U,
    SNAKE_CELL_SRIGHT = 1U,
    SNAKE_CELL_SUP = 2U,
    SNAKE_CELL_SLEFT = 3U,
    SNAKE_CELL_SDOWN = 4U,
    SNAKE_CELL_FOOD = 5U
} SnakeCell;

/* Number of bits used per cell. Must be >= ceil(log2(max cell value + 1)). */
#define SNAKE_CELL_MAX_BITS 3U /* floor(log2(SNAKE_CELL_FOOD)) + 1 */

/* High-level snake direction enumeration used by input and movement logic */
typedef enum
{
    SNAKE_DIR_RIGHT,
    SNAKE_DIR_UP,
    SNAKE_DIR_LEFT,
    SNAKE_DIR_DOWN
} SnakeDirection;

/* Game state - very compact representation:
 * - `cells` is a packed bit array storing one SnakeCell value per grid cell.
 * - head/tail positions are stored as chars (sufficient for these grid sizes).
 * - `next_dir` stores the requested direction for the next step.
 * - `inhibit_tail_step` is used to grow the snake (when >1, tail does not
 *   advance that step).
 * - `occupied_cells` counts how many logical 'occupied' cells the snake/food
 *   have (used for detecting a full grid).
 */
typedef struct
{
    unsigned char cells[(SNAKE_MATRIX_SIZE * SNAKE_CELL_MAX_BITS) / 8U];
    char head_xpos;
    char head_ypos;
    char tail_xpos;
    char tail_ypos;
    char next_dir;
    char inhibit_tail_step;
    unsigned occupied_cells;
} SnakeContext;

/* Application state passed to SDL callbacks: keeps the renderer/window and
 * a SnakeContext plus timing information for stepping the game logic.
 */
typedef struct
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SnakeContext snake_ctx;
    Uint64 last_step;
} AppState;

/* Read a SnakeCell value at grid coordinate (x,y).
 * The value is extracted from the bit-packed `cells` array; to read across
 * a byte boundary we copy two bytes into a 16-bit `range` and shift/mask.
 */
SnakeCell snake_cell_at(const SnakeContext* ctx, char x, char y)
{
    const int shift = SHIFT(x, y);
    unsigned short range;
    SDL_memcpy(&range, ctx->cells + (shift / 8), sizeof(range));
    return (SnakeCell)((range >> (shift % 8)) & THREE_BITS);
}

/* Helper: set an SDL_FRect's upper-left coordinates for a given cell (x,y).
 * This computes pixel positions from logical grid coordinates.
 */
static void set_rect_xy_(SDL_FRect* r, short x, short y)
{
    r->x = (float)(x * SNAKE_BLOCK_SIZE_IN_PIXELS);
    r->y = (float)(y * SNAKE_BLOCK_SIZE_IN_PIXELS);
}

/* Write a SnakeCell value into the packed cells array at (x,y).
 * - Load the 16-bit range that contains the target bits.
 * - Clear the target 3-bit field.
 * - OR in the new value and write the two bytes back.
 *
 * Using a 16-bit buffer means we can safely modify values that straddle a
 * byte boundary without performing per-bit loops.
 */
static void put_cell_at_(SnakeContext* ctx, char x, char y, SnakeCell ct)
{
    const int shift = SHIFT(x, y);
    const int adjust = shift % 8;
    unsigned char* const pos = ctx->cells + (shift / 8);
    unsigned short range;
    SDL_memcpy(&range, pos, sizeof(range));
    range &= ~(THREE_BITS << adjust); /* clear bits */
    range |= (ct & THREE_BITS) << adjust;
    SDL_memcpy(pos, &range, sizeof(range));
}

/* Returns non-zero when the grid is completely occupied (no empty cells) */
static int are_cells_full_(SnakeContext* ctx)
{
    return ctx->occupied_cells == SNAKE_GAME_WIDTH * SNAKE_GAME_HEIGHT;
}

/* Place a food cell at a random empty location:
 * - Picks random x/y until an empty cell is found.
 * - Writes SNAKE_CELL_FOOD into that cell.
 */
static void new_food_pos_(SnakeContext* ctx)
{
    while (true) {
        const char x = (char)SDL_rand(SNAKE_GAME_WIDTH);
        const char y = (char)SDL_rand(SNAKE_GAME_HEIGHT);
        if (snake_cell_at(ctx, x, y) == SNAKE_CELL_NOTHING) {
            put_cell_at_(ctx, x, y, SNAKE_CELL_FOOD);
            break;
        }
    }
}

/* Initialize snake state:
 * - Zero the packed cell array.
 * - Place the head and tail in the center of the board and set the default
 *   moving direction to RIGHT.
 * - Seed the board with some food and set occupied_cells to reflect how many
 *   items are present (used for full-check).
 *
 * Note: `inhibit_tail_step` is used so the tail doesn't immediately move for
 * a number of steps, allowing an initial snake length greater than 1.
 */
void snake_initialize(SnakeContext* ctx)
{
    int i;
    SDL_zeroa(ctx->cells);
    ctx->head_xpos = ctx->tail_xpos = SNAKE_GAME_WIDTH / 2;
    ctx->head_ypos = ctx->tail_ypos = SNAKE_GAME_HEIGHT / 2;
    ctx->next_dir = SNAKE_DIR_RIGHT;
    ctx->inhibit_tail_step = ctx->occupied_cells = 4;
    --ctx->occupied_cells;
    put_cell_at_(ctx, ctx->tail_xpos, ctx->tail_ypos, SNAKE_CELL_SRIGHT);
    for (i = 0; i < 4; i++) {
        new_food_pos_(ctx);
        ++ctx->occupied_cells;
    }
}

/* Request a change of direction for the snake head:
 * - It rejects commands that would immediately reverse the snake into itself.
 *   The check compares the requested direction to the stored current head cell
 *   direction (which is encoded as a SnakeCell).
 */
void snake_redir(SnakeContext* ctx, SnakeDirection dir)
{
    SnakeCell ct = snake_cell_at(ctx, ctx->head_xpos, ctx->head_ypos);
    if ((dir == SNAKE_DIR_RIGHT && ct != SNAKE_CELL_SLEFT) ||
        (dir == SNAKE_DIR_UP && ct != SNAKE_CELL_SDOWN) ||
        (dir == SNAKE_DIR_LEFT && ct != SNAKE_CELL_SRIGHT) ||
        (dir == SNAKE_DIR_DOWN && ct != SNAKE_CELL_SUP)) {
        ctx->next_dir = dir;
    }
}

/* Wrap-around helper used for toroidal board behavior:
 * If an index goes off the left/top it wraps to the opposite side, and same
 * for right/bottom. Modifies the value in-place.
 */
static void wrap_around_(char* val, char max)
{
    if (*val < 0) {
        *val = max - 1;
    }
    else if (*val > max - 1) {
        *val = 0;
    }
}

/* Advance the game state by one step:
 * - Moves the tail forward (unless inhibited by growth).
 * - Moves the head forward according to ctx->next_dir, with wrap-around.
 * - Detects self-collision: if head moves onto a body cell (not NOTHING or
 *   FOOD) the game resets.
 * - Places the direction marker in the previous head cell to record snake
 *   body orientation (used to advance the tail later).
 * - If food was consumed, spawn new food, increase growth inhibition so the
 *   tail doesn't move on the next step, and update occupied count.
 *
 * Important details:
 * - Cells contain directional markers in body segments (SRIGHT, SUP, ...)
 *   so the tail knows which way to move forward when it advances.
 * - `inhibit_tail_step` temporarily prevents the tail from moving, which
 *   implements growth when food is eaten.
 */
void snake_step(SnakeContext* ctx)
{
    const SnakeCell dir_as_cell = (SnakeCell)(ctx->next_dir + 1);
    SnakeCell ct;
    char prev_xpos;
    char prev_ypos;
    /* Move tail forward */
    if (--ctx->inhibit_tail_step == 0) {
        ++ctx->inhibit_tail_step;
        ct = snake_cell_at(ctx, ctx->tail_xpos, ctx->tail_ypos);
        put_cell_at_(ctx, ctx->tail_xpos, ctx->tail_ypos, SNAKE_CELL_NOTHING);
        switch (ct) {
        case SNAKE_CELL_SRIGHT:
            ctx->tail_xpos++;
            break;
        case SNAKE_CELL_SUP:
            ctx->tail_ypos--;
            break;
        case SNAKE_CELL_SLEFT:
            ctx->tail_xpos--;
            break;
        case SNAKE_CELL_SDOWN:
            ctx->tail_ypos++;
            break;
        default:
            break;
        }
        wrap_around_(&ctx->tail_xpos, SNAKE_GAME_WIDTH);
        wrap_around_(&ctx->tail_ypos, SNAKE_GAME_HEIGHT);
    }
    /* Move head forward */
    prev_xpos = ctx->head_xpos;
    prev_ypos = ctx->head_ypos;
    switch (ctx->next_dir) {
    case SNAKE_DIR_RIGHT:
        ++ctx->head_xpos;
        break;
    case SNAKE_DIR_UP:
        --ctx->head_ypos;
        break;
    case SNAKE_DIR_LEFT:
        --ctx->head_xpos;
        break;
    case SNAKE_DIR_DOWN:
        ++ctx->head_ypos;
        break;
    }
    wrap_around_(&ctx->head_xpos, SNAKE_GAME_WIDTH);
    wrap_around_(&ctx->head_ypos, SNAKE_GAME_HEIGHT);
    /* Collisions */
    ct = snake_cell_at(ctx, ctx->head_xpos, ctx->head_ypos);
    if (ct != SNAKE_CELL_NOTHING && ct != SNAKE_CELL_FOOD) {
        /* collision with self -> reset game */
        snake_initialize(ctx);
        return;
    }
    /* Mark previous head cell with the direction we moved, this becomes body. */
    put_cell_at_(ctx, prev_xpos, prev_ypos, dir_as_cell);
    /* Mark new head cell as part of head (we use same direction marker for head
     * drawing; head is drawn specially in the renderer later).
     */
    put_cell_at_(ctx, ctx->head_xpos, ctx->head_ypos, dir_as_cell);
    if (ct == SNAKE_CELL_FOOD) {
        if (are_cells_full_(ctx)) {
            /* Board full -> reset game */
            snake_initialize(ctx);
            return;
        }
        new_food_pos_(ctx);
        ++ctx->inhibit_tail_step; /* grow: skip advancing tail next time */
        ++ctx->occupied_cells;
    }
}

/* Handle keyboard input mapped to game behavior:
 * - Q or ESC quits the app.
 * - R restarts the game.
 * - Arrow keys request a direction change for the snake (validated by snake_redir).
 *
 * This returns SDL_AppResult so SDL's main-callback system knows whether to
 * continue or terminate.
 */
static SDL_AppResult handle_key_event_(SnakeContext* ctx, SDL_Scancode key_code)
{
    switch (key_code) {
        /* Quit. */
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        return SDL_APP_SUCCESS;
        /* Restart the game as if the program was launched. */
    case SDL_SCANCODE_R:
        snake_initialize(ctx);
        break;
        /* Decide new direction of the snake. */
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_D:
        snake_redir(ctx, SNAKE_DIR_RIGHT);
        break;
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_W:
        snake_redir(ctx, SNAKE_DIR_UP);
        break;
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_A:
        snake_redir(ctx, SNAKE_DIR_LEFT);
        break;
    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_S:
        snake_redir(ctx, SNAKE_DIR_DOWN);
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

/* SDL main-loop iterate callback:
 * - This is called repeatedly by SDL's callback-driven "main" implementation.
 * - It advances game logic as many times as necessary to "catch up" with the
 *   wall-clock time (so slow frames don't drop game steps).
 * - It clears the renderer, draws all non-empty cells (food blue, body green),
 *   then draws the head in yellow, and presents the frame.
 */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    AppState* as = (AppState*)appstate;
    SnakeContext* ctx = &as->snake_ctx;
    const Uint64 now = SDL_GetTicks();
    SDL_FRect r;
    unsigned i;
    unsigned j;
    int ct;

    // run game logic if we're at or past the time to run it.
    // if we're _really_ behind the time to run it, run it
    // several times.
    while ((now - as->last_step) >= STEP_RATE_IN_MILLISECONDS) {
        snake_step(ctx);
        as->last_step += STEP_RATE_IN_MILLISECONDS;
    }

    r.w = r.h = SNAKE_BLOCK_SIZE_IN_PIXELS;
    SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(as->renderer);

    /* The coordinate loops iterate i across width and j across height.
     * Only non-empty cells are drawn; we use the per-cell value to decide the
     * draw color (food vs body). The head is drawn afterwards to ensure it
     * appears over the body.
     */
    for (i = 0; i < SNAKE_GAME_WIDTH; i++) {
        for (j = 0; j < SNAKE_GAME_HEIGHT; j++) {
            ct = snake_cell_at(ctx, i, j);
            if (ct == SNAKE_CELL_NOTHING)
                continue;
            set_rect_xy_(&r, i, j);
            if (ct == SNAKE_CELL_FOOD)
                SDL_SetRenderDrawColor(as->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            else /* body */
                SDL_SetRenderDrawColor(as->renderer, 0, 128, 128, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(as->renderer, &r);
        }
    }
    /* Draw head after body so it is visually distinct. */
    SDL_SetRenderDrawColor(as->renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); /*head*/
    set_rect_xy_(&r, ctx->head_xpos, ctx->head_ypos);
    SDL_RenderFillRect(as->renderer, &r);
    SDL_RenderPresent(as->renderer);
    return SDL_APP_CONTINUE;
}

/* Extended metadata for the SDL application (optional) */
static const struct
{
    const char* key;
    const char* value;
} extended_metadata[] =
{
    { SDL_PROP_APP_METADATA_URL_STRING, "https://examples.libsdl.org/SDL3/demo/01-snake/" },
    { SDL_PROP_APP_METADATA_CREATOR_STRING, "SDL team" },
    { SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Placed in the public domain" },
    { SDL_PROP_APP_METADATA_TYPE_STRING, "game" }
};

/* SDL_AppInit: called once at program start (SDL main-callback).
 * - Sets metadata, initializes the video subsystem.
 * - Allocates an AppState and creates the window/renderer.
 * - Initializes the snake and sets the last_step timestamp.
 *
 * Returns SDL_APP_CONTINUE to run the app, SDL_APP_FAILURE on fatal error,
 * SDL_APP_SUCCESS to quit immediately.
 */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    size_t i;

    if (!SDL_SetAppMetadata("Example Snake game", "1.0", "com.example.Snake")) {
        return SDL_APP_FAILURE;
    }

    for (i = 0; i < SDL_arraysize(extended_metadata); i++) {
        if (!SDL_SetAppMetadataProperty(extended_metadata[i].key, extended_metadata[i].value)) {
            return SDL_APP_FAILURE;
        }
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return SDL_APP_FAILURE;
    }

    AppState* as = (AppState*)SDL_calloc(1, sizeof(AppState));
    if (!as) {
        return SDL_APP_FAILURE;
    }

    *appstate = as;

    if (!SDL_CreateWindowAndRenderer("examples/demo/snake", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, 0, &as->window, &as->renderer)) {
        return SDL_APP_FAILURE;
    }

    snake_initialize(&as->snake_ctx);

    as->last_step = SDL_GetTicks();

    return SDL_APP_CONTINUE;
}

/* SDL_AppEvent: called by SDL when events happen.
 * - Translates SDL events into game actions (quit, key presses).
 * - The keyboard handling delegates to handle_key_event_ which may request
 *   program termination (SDL_APP_SUCCESS) or continue.
 */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    SnakeContext* ctx = &((AppState*)appstate)->snake_ctx;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        return handle_key_event_(ctx, event->key.scancode);
    }
    return SDL_APP_CONTINUE;
}

/* SDL_AppQuit: cleanup when the application exits.
 * - Destroys renderer/window and frees the AppState memory.
 */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    if (appstate != NULL) {
        AppState* as = (AppState*)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
}