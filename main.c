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
#define PADDLE_SPEED 10

#define BALL_RADIUS 5
#define BALL_DIAMETER BALL_RADIUS*2
#define BALL_SPEED 5
#define BALL_INIT_X SCREEN_WIDTH/2 - BALL_RADIUS
#define BALL_INIT_Y SCREEN_HEIGHT/2 - BALL_RADIUS

#define REFLECTION_CONST 1.5

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

int signum(const int x) {
  return (!!x) * (1 - (((x >> 31) & 1) << 1));
}

int abs(const int _X) {
  return (_X >> 31) ? (~_X)+1 : _X;
}

typedef struct {
  int xpos;
  int ypos;
} Paddle;

// not sure how to draw circles yet
// so will have square for now
typedef struct {
  int x;
  int y;
  int dx;
  int dy;
} Ball;

// handles collision
void move_paddle(Paddle* paddle, const int dy) {
  paddle->ypos = MIN(MAX(0, paddle->ypos+dy), SCREEN_HEIGHT-PADDLE_HEIGHT);
}

void ai_move(Paddle* paddle, Ball* ball) {
  int dy = 0;
  if (ball->y < paddle->ypos) {
    dy = -PADDLE_SPEED;
  } else if (ball->y > paddle->ypos + PADDLE_HEIGHT) {
    dy = PADDLE_SPEED;
  }
  move_paddle(paddle, dy);
}


// returns true if ball hit left or right boundary
bool move_ball(Ball* ball, const Paddle* const player_paddle, const Paddle* const ai_paddle) {
  ball->y += ball->dy;
  ball->x += ball->dx;

  // collision with top wall
  if (ball->y <= 0) {
    ball->y = 0;
    ball->dy = -ball->dy;
  } else if (ball->y >= SCREEN_HEIGHT - (BALL_RADIUS)) {
    ball->y = SCREEN_HEIGHT - (BALL_RADIUS);
    ball->dy = -ball->dy;
  }

  // collision with either goal
  if (ball->x <= 0 || ball->x >= SCREEN_WIDTH - (BALL_RADIUS)) return true;


  // collision with right (player) paddle
  if (ball->x + BALL_RADIUS >= player_paddle->xpos &&
        ball->y + BALL_RADIUS >= player_paddle->ypos &&
        ball->y - BALL_RADIUS <= player_paddle->ypos + PADDLE_HEIGHT) {
    ball->x = player_paddle->xpos - BALL_RADIUS;
    ball->dx *= -1;
    int dist = ball->y - (player_paddle->ypos + (PADDLE_HEIGHT/2));
    if (abs(dist) > PADDLE_HEIGHT/4) {
      ball->dy = ball->dy*REFLECTION_CONST + 1;
    } else {
      ball->dy = ball->dy/REFLECTION_CONST - 1;
    }
  }
  //paddle_collision(colliding wall, direction?)

  // collision with left (ai) paddle
  if (ball->x - BALL_RADIUS <= ai_paddle->xpos + PADDLE_WIDTH &&
      ball->y + BALL_RADIUS >= ai_paddle->ypos &&
      ball->y - BALL_RADIUS <= ai_paddle->ypos + PADDLE_HEIGHT) {
    ball->x = ai_paddle->xpos + PADDLE_WIDTH + BALL_RADIUS;
    ball->dx *= -1;
    int dist = ball->y - (player_paddle->ypos + (PADDLE_HEIGHT/2));
    if (abs(dist) > PADDLE_HEIGHT/4) {
      ball->dy = ball->dy*REFLECTION_CONST + 1;
    } else {
      ball->dy = ball->dy/REFLECTION_CONST - 1;
    }
  }

  return false;
}

void render_ball(const Ball* ball, SDL_Rect* draw_rect) {
  draw_rect->w = BALL_DIAMETER;
  draw_rect->h = BALL_DIAMETER;
  draw_rect->x = ball->x - (BALL_RADIUS);
  draw_rect->y = ball->y - (BALL_RADIUS);
}

void render_paddle(const Paddle* paddle, SDL_Rect* draw_rect) {
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
  bool paused = true;
  SDL_Event event;

  Paddle player_paddle = { .xpos = SCREEN_WIDTH - PADDLE_WIDTH - 15, .ypos = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2 };
  Paddle ai_paddle = { .xpos = 0 + 15, .ypos = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2 };

  Ball ball = { .x = BALL_INIT_X, .y = BALL_INIT_Y, .dx = BALL_SPEED, .dy = -BALL_SPEED };

  SDL_Rect paddle_rect = { 0 };
  SDL_Rect ai_paddle_rect = { 0 };
  SDL_Rect ball_rect = { 0 };

  while (running) {
    int dy = 0;
    while(SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
        printf("Quitting\n");
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_k: {
            dy = -PADDLE_SPEED;
          } break;
          case SDLK_j: {
            dy = PADDLE_SPEED;
          } break;
          case SDLK_ESCAPE: {
            paused = !paused;
          } break;
          default: {};
        }
      }
    }
   
    if (paused) continue;

    // update
    player_paddle.ypos += dy;
    move_paddle(&player_paddle, dy);
    ai_move(&ai_paddle, &ball);
    if (move_ball(&ball, &player_paddle, &ai_paddle)) break;

    // clear screen
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0, 0, 0));

    // render game objects
    render_paddle(&player_paddle, &paddle_rect);
    render_paddle(&ai_paddle, &ai_paddle_rect);
    render_ball(&ball, &ball_rect);

    SDL_FillRect(screen_surface, &paddle_rect, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));
    SDL_FillRect(screen_surface, &ball_rect, SDL_MapRGB(screen_surface->format, 0, 0xFF, 0));
    SDL_FillRect(screen_surface, &ai_paddle_rect, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

    SDL_UpdateWindowSurface(window);
    SDL_Delay(20);
  }
  
  SDL_DestroyWindow(window);
close:
  SDL_Quit();
  return 0;
}
