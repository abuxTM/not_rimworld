#include "core/renderer.h"
#include "core/global.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

void renderer_init(void) {
  global.renderer = SDL_CreateRenderer(global.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (global.renderer == NULL) {
    printf("[SDL2] There was an error while creating renderer. SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(global.window);
    SDL_Quit();
    exit(1);
  }
}
