#include "game/mouse.h"
#include "core/global.h"
#include "game/texture_manager.h"
#include "game/world.h"
#include "utils/remath.h"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>

SDL_Texture* texture = NULL;
SDL_Rect nrect = {0, 0, TILE_SIZE, TILE_SIZE};

Mouse* mouse_init() {
  Mouse* mouse = malloc(sizeof(Mouse));
  if (!mouse) return NULL;

  texture = get_texture("assets/world/ground.png");
  mouse->rect = (SDL_Rect){0, 0, TILE_SIZE, TILE_SIZE};
  mouse->state = BUILDING;
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(texture, 200);

  return mouse;
}

void mouse_render(World* world) {
  if (global.mouse->state == BUILDING) {
    global.mouse->rect = (SDL_Rect) {
      nrect.x * global.camera->zoom - global.camera->pos.x,
      nrect.y * global.camera->zoom - global.camera->pos.y,
      nrect.w * global.camera->zoom,
      nrect.h * global.camera->zoom
    };
    SDL_RenderCopy(global.renderer, texture, NULL, &global.mouse->rect);

    block_create(
      world,
      (Vector2D){global.mouse->rect.x, global.mouse->rect.y},
      (Vector2D){64, 64}
    );
  }
}

void mouse_inputs(SDL_Event* event) {
  if (event->type == SDL_MOUSEBUTTONDOWN) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    mouseX += global.camera->pos.x;
    mouseY += global.camera->pos.y;
    
    // Calculate position considering zoom level
    int zoomedMouseX = mouseX / global.camera->zoom;
    int zoomedMouseY = mouseY / global.camera->zoom;
    
    // Calculate new position of the rectangle
    nrect.x = (zoomedMouseX / TILE_SIZE) * TILE_SIZE;
    nrect.y = (zoomedMouseY / TILE_SIZE) * TILE_SIZE;
  }
}

