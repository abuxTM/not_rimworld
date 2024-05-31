#ifndef ITEM_H
#define ITEM_H

#include "utils/remath.h"
#include <SDL2/SDL_render.h>
#include <stdbool.h>

typedef enum {
  CONSUMABLE,
  GUN,
  MEELE,
  TOOL
} ItemType;

typedef struct {
  Vector2D pos, scale;
  const char* name;
  const char* description;
  ItemType type;
  SDL_Texture* texture;
  bool is_active;
  SDL_RendererFlip flip;
} Item;

typedef struct {
  Item item;
  int durability;
  int max_durability;
} Tool;

typedef struct {
  Item* item;
  int current_ammo, max_ammo;
} Gun;

Item* item_create(Vector2D pos, Vector2D scale, const char* name, const char* description, ItemType type, SDL_Texture* texture);

#endif
