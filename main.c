//
// Created by Sujit on 11/15/2024.
//

#include <stdio.h>

#include "inc/game.h"

int main(int argc, char** argv) {

    init_game();

    SDL_Event event;
    printf("%llu\n",sizeof(event));
    while (is_running()) {
        while(SDL_PollEvent(&event)) {
            event_handler(&event);
        }
    }

    close_game();
    return 0;
}