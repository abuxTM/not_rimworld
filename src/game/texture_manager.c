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

FontTexture* get_font_texture(const char* text, int font_size) {
    FontTexture* font_texture = malloc(sizeof(FontTexture));
    if (!font_texture) return NULL;
    font_texture->texture = NULL;
    font_texture->width = 0;
    font_texture->height = 0;

    TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", font_size);
    if (font == NULL) {
        printf("Unable to load font: %s\n", TTF_GetError());
        return font_texture;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, (SDL_Color){255, 255, 255});
    if (surface == NULL) {
        printf("Unable to create text surface: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return font_texture;
    }

    font_texture->texture = SDL_CreateTextureFromSurface(global.renderer, surface);
    if (font_texture->texture == NULL) {
        printf("Unable to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return font_texture;
    }

    // Create a rectangle to hold the dimensions of the rendered text
    font_texture->width = surface->w;
    font_texture->height = surface->h;

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

    return font_texture;
}
