#ifndef ITEM_H
#define ITEM_H

#include "utils/remath.h"
#include <SDL2/SDL_render.h>
#include <stdbool.h>

typedef enum {
  TOOL,
  GUN,
  MELEE,
  CONSUMABLE
} ItemType;

typedef struct {
  const char* name;
  const char* description;
  Vector2D pos, scale;
  SDL_Texture* texture;
  SDL_RendererFlip flip;
  ItemType type;
  bool is_active;
} Item;

typedef struct {
  Item item;
  int durability;
  int max_durability;
} Tool;

typedef struct {
  Vector2D pos, scale;
} Bullet;

typedef struct {
  Item* item;
  Bullet** bullets;
  int current_ammo, max_ammo;
} Gun;

Item* item_create(Vector2D pos, Vector2D scale, const char* name, const char* description, ItemType type, SDL_Texture* texture);
void item_destroy(Item* item);
void item_render(Item* item);

Bullet* bullet_create();
void bullet_render(Bullet* bullet);
void bullet_destroy(Bullet* bullet);
void bullets_render(Gun* gun);
void bullets_destroy(Gun* gun);

#endif
