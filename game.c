//
// Created by jitty on 11/16/2024.
//

#include <stdio.h>

#include "inc/game.h"

SDL_Window *window;
SDL_Surface *screen_surface;

static bool running;

// GAME STARTUP LOGIC HERE
bool init_game() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "ERROR: COULDN'T INITIALIZE VIDEO: %s\n", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN );
    if (!window) {
        fprintf(stderr, "ERROR: COULDN'T CREATE WINDOW: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    screen_surface = SDL_GetWindowSurface(window);

    // LOAD START SCREEN HERE
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0xFF, 0, 0));
    SDL_UpdateWindowSurface(window);

    running = true;

    return true;
}

void event_handler(const SDL_Event* event) {
    switch (event->type) {
        case SDL_QUIT:
            running = false;
            break;
        default: {
            printf("UNIMPLEMENTED EVENT: %d\n",event->type);
        } break;
    }
}

bool is_running() {
    return running;
}

void close_game() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}