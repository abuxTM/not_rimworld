#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SDL2/SDL_render.h>

SDL_Texture* get_texture(const char* path);
SDL_Texture* get_font_texture(const char* text, int font_size);

#endif
