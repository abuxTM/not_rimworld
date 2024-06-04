#ifndef WORLD_H
#define WORLD_H

#include "utils/remath.h"
#include <SDL2/SDL_render.h>
#include <sys/types.h>

typedef enum {
  TILE,
  TREE,
  ROCK,
  WATER
} Type;

typedef struct {
  Vector2D pos, scale;
  SDL_Texture* texture;
  Type type;
} Block;

typedef struct {
  Block** blocks;
  size_t capacity, count;
} World;

Block* block_create(World* world, Vector2D pos, Vector2D scale, const char* texture_path, Type type);
void block_destroy(Block* block);

void block_render(Block* block);

World* world_create(size_t initial_capacity);
void world_destroy(World* world);

void world_render(World* world);

#endif
