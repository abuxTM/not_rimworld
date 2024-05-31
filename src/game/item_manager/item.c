#include "game/item_manager/item.h"

Item* item_create(Vector2D pos, Vector2D scale, const char* name, const char* description, ItemType type, SDL_Texture* texture) {
  Item* item = malloc(sizeof(Item));
  if (!item) return NULL;

  item->pos = pos;
  item->scale = scale;
  item->name = name;
  item->description = description;
  item->type = type;
  item->texture = texture;
  item->is_active = false;

  return item;
}
