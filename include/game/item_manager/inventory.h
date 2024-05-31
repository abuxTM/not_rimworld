#ifndef INVENTORY_H
#define INVENTORY_H

#include "game/item_manager/item.h"
#include <SDL2/SDL_render.h>
#include <sys/types.h>

typedef struct {
  Item** items;
  size_t capacity, count;
} Inventory;

Inventory* inventory_create(size_t initial_capacity);
void inventory_destroy(Inventory* inventory);

void use_tool(Tool* tool);
void use_gun(Gun* gun);

void inventory_use_item(Inventory* inventory, int id);
void inventory_add_item(Inventory* inventory, Item* item);
void inventory_remove_item(Inventory* inventory, int id);
void inventory_update(Inventory* inventory);
void inventory_render(Inventory* inventory);

#endif
