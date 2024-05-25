#include "game/world.h"
#include "core/global.h"
#include "game/texture_manager.h"
#include "utils/remath.h"
#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <sys/types.h>

Block* block_create(World* world, Vector2D pos, Vector2D scale) {
  if (world->count >= world->capacity) {
    world->capacity *= 2;
    world->blocks = (Block**)realloc(world->blocks, world->capacity * sizeof(Block*));
  }

  Block* block = (Block*)malloc(sizeof(Block));
  if (!block) return NULL;

  block->pos = pos;
  block->scale = scale;
  block->texture = get_texture("assets/grass.png");

  world->blocks[world->count++] = block;

  return block;
}

void block_destroy(Block* block) {
  SDL_DestroyTexture(block->texture);
}

void block_render(Block* block) {
  SDL_SetRenderDrawColor(global.renderer, 200, 20, 20, 255);

  SDL_Rect camera_rect = {
    block->pos.x - global.camera->pos.x,
    block->pos.y - global.camera->pos.y,
    block->scale.x,
    block->scale.y
  };

  if (global.show_coll) SDL_RenderDrawRect(global.renderer, &camera_rect);
  if (block->texture) SDL_RenderCopy(global.renderer, block->texture, NULL, &camera_rect);
}

World* world_create(size_t initial_capacity) {
  World* world = (World*)malloc(sizeof(World));

  world->blocks = (Block**)malloc(initial_capacity * sizeof(Block*));
  world->capacity = initial_capacity;
  world->count = 0;

  return world;
}

void world_destroy(World* world) {
  for (size_t i = 0; i < world->count; ++i) {
    block_destroy(world->blocks[i]);
    free(world->blocks[i]);
  }

  free(world->blocks);
  free(world);
}

void world_render(World* world) {
  for (size_t i = 0; i < world->count; ++i) {
    block_render(world->blocks[i]);
  }
}
