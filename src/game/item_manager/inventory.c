#include "game/item_manager/inventory.h"
#include "core/global.h"
#include "utils/remath.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <sys/types.h>

Inventory* inventory_create(size_t initial_capacity) {
  Inventory* inventory = malloc(sizeof(Inventory));
  if (!inventory) return NULL;

  inventory->items = (Item**)malloc(initial_capacity * sizeof(Item*));
  inventory->count = 0;
  inventory->capacity = initial_capacity;

  return inventory;
}

void inventory_destroy(Inventory* inventory) {
  if (inventory == NULL) return;

  for (size_t i = 0; i < inventory->count; ++i) {
    if (inventory->items[i] != NULL) {
      if (inventory->items[i]->texture != NULL) {
        SDL_DestroyTexture(inventory->items[i]->texture);
      }
      free(inventory->items[i]);
    }
  }
  free(inventory->items);
  free(inventory);
}

void inventory_update(Inventory* inventory) {}

void inventory_render(Inventory* inventory) {
  for (size_t i = 0; i < inventory->count; ++i) {
    SDL_Rect rect = {
      inventory->items[i]->pos.x * global.camera->zoom - global.camera->pos.x,
      inventory->items[i]->pos.y * global.camera->zoom - global.camera->pos.y,
      inventory->items[i]->scale.x * global.camera->zoom,
      inventory->items[i]->scale.y * global.camera->zoom
    };
    if (inventory->items[i]->is_active) SDL_RenderCopyEx(global.renderer, inventory->items[i]->texture, NULL, &rect, 0, NULL, inventory->items[i]->flip);
  }
}

void inventory_use_item(Inventory* inventory, int id) {
  if (id < inventory->count) {
    Item* item = inventory->items[id];
    if (!item->is_active) return;
    switch (item->type) {
      case TOOL:
        use_tool((Tool*)item);
        break;
      case GUN:
        use_gun((Gun*)item);
        break;
      default:
        printf("Item is of generic type or unknown type.\n");
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
