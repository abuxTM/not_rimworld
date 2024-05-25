#include "core/window.h"
#include "core/global.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

void window_init(void) {
  global.window = SDL_CreateWindow("Rimworld",
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   global.screen_width, global.screen_height, SDL_WINDOW_SHOWN);
  if (global.window == NULL) {
    printf("[SDL2] There was an error while creating window. SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
  }
}
