#include "game/texture_manager.h"
#include "core/global.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

SDL_Texture* get_texture(const char* path) {
  SDL_Surface* surface = IMG_Load(path);
  if (surface == NULL) { printf("Unable to load image! %s\n", path); return NULL; }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(global.renderer, surface);
  if (texture == NULL) { printf("Unable to create texture!\n"); return NULL; }

  SDL_FreeSurface(surface);

  return texture;
}

SDL_Texture* get_font_texture(const char* text, int font_size) {
  TTF_Font* font = TTF_OpenFont("assets/fonts/Norsebold.otf", font_size);
  SDL_Surface* surface = TTF_RenderText_Solid(font, text, (SDL_Color){200,200,200});
  if (surface == NULL) { printf("Unable to create text surface!\n"); return NULL; }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(global.renderer, surface);
  if (texture == NULL) { printf("Unable to create text texture!\n"); return NULL; }

  SDL_FreeSurface(surface);

  return texture;
}
