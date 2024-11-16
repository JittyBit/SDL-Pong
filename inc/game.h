//
// Created by jitty on 11/16/2024.
//

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

bool init_game(void);
void close_game(void);
bool is_running(void);

void event_handler(const SDL_Event* event);

#endif //GAME_H
