#include "game/item_manager/item.h"
#include "core/global.h"
#include <SDL2/SDL_render.h>

Item* item_create(Vector2D pos, Vector2D scale, const char* name, const char* description, ItemType type, SDL_Texture* texture) {
  Item* item = malloc(sizeof(Item));
  if (!item) return NULL;

  item->pos = pos;
  item->scale = scale;
  item->name = name;
  item->description = description;
  item->texture = texture;
  item->type = type;
  item->is_active = false;

  return item;
}

void item_destroy(Item* item) {
  if (!item) return;
  if (item->texture) SDL_DestroyTexture(item->texture);
  free(item);
}

void item_render(Item* item) {
  SDL_Rect rect = {
    item->pos.x * global.camera->zoom - global.camera->pos.x,
    item->pos.y * global.camera->zoom - global.camera->pos.y,
    item->scale.x * global.camera->zoom,
    item->scale.y * global.camera->zoom
  };
  SDL_Rect rect_shadow = rect;
  rect_shadow.x -= 4;
  rect_shadow.y += 4;
  if (item->is_active) {
    SDL_SetTextureColorMod(item->texture, 0, 0, 0);
    SDL_SetTextureAlphaMod(item->texture, 191);
    SDL_RenderCopyEx(global.renderer, item->texture, NULL, &rect_shadow, 0, NULL, item->flip);
    SDL_SetTextureColorMod(item->texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(item->texture, 255);
    SDL_RenderCopyEx(global.renderer, item->texture, NULL, &rect, 0, NULL, item->flip);
  }
}

Bullet* bullet_create() {
  Bullet* bullet = malloc(sizeof(Bullet));
  if (!bullet) return NULL;

  return bullet;
}

void bullet_render(Bullet* bullet) {
  SDL_Rect rect = {
    bullet->pos.x - global.camera->pos.x,
    bullet->pos.y - global.camera->pos.y,
    bullet->scale.x, bullet->scale.y
  };

  SDL_RenderFillRect(global.renderer, &rect);
}

void bullet_destroy(Bullet* bullet) {
  free(bullet);
}

void bullets_render(Gun* gun) {
  for (size_t i = 0; i < gun->current_ammo; ++i) {
    Bullet* bullet = gun->bullets[i];
    bullet_render(bullet);
  }
}

void bullets_destroy(Gun* gun) {
  for (size_t i = 0; i < gun->current_ammo; ++i) {
    Bullet* bullet = gun->bullets[i];
    if (bullet) bullet_destroy(bullet);
  }
  free(gun->bullets);
}
