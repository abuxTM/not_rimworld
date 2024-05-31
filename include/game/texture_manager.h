#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL_render.h>

typedef struct {
  SDL_Texture* texture;
  float width, height;
} FontTexture;

SDL_Texture* get_texture(const char* path);
FontTexture* get_font_texture(const char* text, int font_size);

#endif
