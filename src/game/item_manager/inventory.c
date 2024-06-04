#include "game/item_manager/inventory.h"
#include "game/item_manager/item.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <sys/types.h>

Inventory* inventory_create(size_t initial_capacity) {
  Inventory* inventory = malloc(sizeof(Inventory));
  if (!inventory) return NULL;

  inventory->items = malloc(initial_capacity * sizeof(Item*));
  inventory->count = 0;
  inventory->capacity = initial_capacity;

  return inventory;
}

void inventory_destroy(Inventory* inventory) {
  if (inventory == NULL) return;

  for (size_t i = 0; i < inventory->count; ++i) {
    if (!inventory->items[i]) return;
    item_destroy(inventory->items[i]);
  }
  free(inventory->items);
  free(inventory);
}

void inventory_update(Inventory* inventory) {}

void inventory_render(Inventory* inventory) {
  for (size_t i = 0; i < inventory->count; ++i) {
    item_render(inventory->items[i]);
    if (inventory->items[i]->type == GUN) {
    }
  }
}

void shoot(Item* item) {
    Gun* gun = (Gun*)item;
    if (gun->current_ammo <= gun->max_ammo) {
        Bullet* new_bullet = bullet_create(); // Define bullet_create() function to allocate and initialize a new bullet
        if (new_bullet != NULL) {
            gun->bullets[gun->current_ammo++] = new_bullet;
        } else {
            // Handle error if bullet creation fails (e.g., out of memory)
        }
    } else {
        // Handle error if gun is out of ammo
    }
}

void inventory_use_item(Inventory* inventory, int id) {
  if (id < inventory->count) {
    Item* item = inventory->items[id];
    if (!item->is_active) return;
    switch (item->type) {
      case GUN:
        shoot(item);
        break;
      default:
        break;
    }
  } else {
    printf("Invalid item id.\n");
  }
}

void inventory_add_item(Inventory* inventory, Item* item) {
  if (inventory->count >= inventory->capacity) {
    inventory->capacity *= 2;
    inventory->items = realloc(inventory->items, inventory->capacity * sizeof(Item*));
  }
  printf("[Inventory] New item added: %s\n", item->name);
  inventory->items[inventory->count++] = item;
}

void inventory_remove_item(Inventory* inventory, int id) {
  if (id < inventory->count) {
    for (size_t i = id; i < inventory->count - 1; ++i) {
      inventory->items[i] = inventory->items[i + 1];
    }
    inventory->count--;
  }
}

void use_tool(Tool* tool) {
  if (tool->durability >= 0) {
    tool->durability--;
    printf("Used: %s. Remaining durability: %d\n", tool->item.name, tool->durability);
  } else {
    printf("%s is broken!\n", tool->item.name);
  }
}

void use_gun(Gun* gun) {
  printf("Shoot!\n");
}
