#include "core/init.h"
#include "core/global.h"
#include "game/game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

Global global;

void sdl_init(void) {
  // Initialize SDL2
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("[SDL2] There was an error while initializing. SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
    printf("Warning: Linear texture filtering not enabled!");
  }

  if (IMG_Init(IMG_INIT_PNG) < 0) {
    printf("[SDL2] There was an error while initializing. SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  if (TTF_Init() < 0) {
    printf("[SDL2] There was an error while initializing. SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  // Define global variables
  global.screen_width = 800;
  global.screen_height = 600;
  global.should_quit = false;
  global.show_coll = false;

  // Initialize game and game loop
  game_init();

  return 0;
}
