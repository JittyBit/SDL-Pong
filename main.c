//
// Created by Sujit on 11/15/2024.
//

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 100

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

typedef struct {
  int xpos;
  int ypos;
} Paddle;

// handles collision
void move_paddle(Paddle* paddle, int dy) {
  paddle->ypos = min(max(0, paddle->ypos+dy), SCREEN_HEIGHT-PADDLE_HEIGHT);
}

void render_paddle(Paddle* paddle, SDL_Rect* draw_rect) {
  draw_rect->x = paddle->xpos;
  draw_rect->y = paddle->ypos;
  draw_rect->w = PADDLE_WIDTH;
  draw_rect->h = PADDLE_HEIGHT;
}

int main(int argc, char** argv) {
  SDL_Window *window;
  SDL_Surface *screen_surface;
   
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "ERROR: COULDN'T INITIALIZE VIDEO: %s\n", SDL_GetError());
    return 1;
  }
  
  window = SDL_CreateWindow(
    "Pong",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH, SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN );
  
  if (!window) {
    fprintf(stderr, "ERROR: COULDN'T CREATE WINDOW: %s\n", SDL_GetError());
    goto close;
  }

  screen_surface = SDL_GetWindowSurface(window);


  bool running = true;
  SDL_Event event;

  // TODO:
  // 1. Paddle Struct
  // 2. Ball
  // 3. Paddle Collision
  // 4. Score
  // 5. AI

  Paddle player_paddle = { .xpos = SCREEN_WIDTH - PADDLE_WIDTH - 5, .ypos = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2 };
  // Paddle ai_paddle = { 0 };
  SDL_Rect paddle_rect = { 0 };

  while (running) {
    int dy = 0;
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
        printf("Quitting\n");
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_k: {
            dy = -10;
          } break;
          case SDLK_j: {
            dy = 10;
          } break;
          default: {};
        }
      }
    }

    // update paddles
    player_paddle.ypos += dy;
    move_paddle(&player_paddle, dy);

    // clear screen
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0, 0, 0));

    // render paddles
    render_paddle(&player_paddle, &paddle_rect);
    SDL_FillRect(screen_surface, &paddle_rect, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));
    // SDL_FillRect(screen_surface, &rect, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

    SDL_UpdateWindowSurface(window);
    SDL_Delay(20);
  }
  
  SDL_DestroyWindow(window);
close:
  SDL_Quit();
  return 0;
}
