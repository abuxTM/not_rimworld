#include "game/world.h"
#include "core/global.h"
#include "game/texture_manager.h"
#include "utils/remath.h"
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>

Block* block_create(World* world, Vector2D pos, Vector2D scale, const char* texture_path, Type type) {
  if (world->count >= world->capacity) {
    world->capacity *= 2;
    world->blocks = realloc(world->blocks, world->capacity * sizeof(Block*));
  }

  Block* block = malloc(sizeof(Block));
  if (!block) return NULL;

  block->pos = pos;
  block->scale = scale;
  block->texture = get_texture(texture_path);
  block->type = type;

  world->blocks[world->count++] = block;

  return block;
}

void block_destroy(Block* block) {
  SDL_DestroyTexture(block->texture);
}

void block_render(Block* block) {
  SDL_SetRenderDrawColor(global.renderer, 200, 20, 20, 255);

  SDL_Rect camera_rect = {
    (int)floor(block->pos.x * global.camera->zoom - global.camera->pos.x),
    (int)floor(block->pos.y * global.camera->zoom - global.camera->pos.y),
    (int)ceil(block->scale.x * global.camera->zoom),
    (int)ceil(block->scale.y * global.camera->zoom)
  };

  if (block->texture) SDL_RenderCopy(global.renderer, block->texture, NULL, &camera_rect);
  if (global.show_coll) SDL_RenderDrawRect(global.renderer, &camera_rect);
}

World* world_create(size_t initial_capacity) {
  World* world = malloc(sizeof(World));

  world->blocks = malloc(initial_capacity * sizeof(Block*));
  world->capacity = initial_capacity;
  world->count = 0;

  float height_map[40 * 40];

  for (int y = 0; y < 40; ++y) {
    for (int x = 0; x < 40; ++x) {
      height_map[y * 40 + x] = value_noise((Vector2D){x, y}, (Vector2D){0.1, 0.2}, rand());
      float height_value = height_map[y * 40 + x];
      if (height_value >= 1.0f) {

      } else {
        block_create(world, (Vector2D){x*64, y*64}, (Vector2D){TILE_SIZE, TILE_SIZE}, "assets/world/grass_tile.png", TILE);
        if (rand() % 2 == 0) block_create(world, (Vector2D){x*64, y*64}, (Vector2D){random_value(40, TILE_SIZE), random_value(40, TILE_SIZE)}, "assets/world/grass.png", TILE);
        if (rand() % 1000 == 0) block_create(world, (Vector2D){x*64, y*64}, (Vector2D){TILE_SIZE, TILE_SIZE}, "assets/world/rose.png", TILE);
        if (rand() % 100 == 0) block_create(world, (Vector2D){x*64, y*64}, (Vector2D){random_value(35, TILE_SIZE), random_value(35, TILE_SIZE)}, "assets/world/bush.png", TILE);
        if (height_value > 0.6 && rand() % 10 < 2) {
          block_create(world, (Vector2D){x*64, y*64}, (Vector2D){random_value(48, TILE_SIZE), random_value(48, TILE_SIZE)}, "assets/world/tree.png", TREE);
        }
      }
    }
  }

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
