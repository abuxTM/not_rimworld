#include "game/pawn_manager.h"
#include "core/global.h"
#include "game/item_manager/inventory.h"
#include "game/item_manager/item.h"
#include "game/particle_manager.h"
#include "game/texture_manager.h"
#include "utils/remath.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Pawn* pawn_create(Vector2D pos, Vector2D scale) {
  PawnManager* manager = global.pawn_manager;

  // Add extra capacity to the manager on overflow
  if (manager->count >= manager->capacity) {
    manager->capacity *= 2;
    manager->pawns = (Pawn**)realloc(manager->pawns, manager->capacity * sizeof(Pawn*));
    if (!manager->pawns) return NULL;
  }

  // Allocate new memory for the pawn
  Pawn* pawn = malloc(sizeof(Pawn));
  if (!pawn) return NULL;

  // Set default parameters
  pawn->name = "PAWN";
  pawn->id = manager->count;
  pawn->health = 100;
  pawn->speed = 2;
  pawn->pos = pos;
  pawn->dir = (Vector2D){0, 0};
  pawn->scale = scale;

  pawn->inventory = inventory_create(10);
  pawn->particle_system = particle_system_create(pos, 4);
  Item* axe = item_create(pos, scale, "Axe", "Chop tree's", TOOL, get_texture("assets/items/tools/axe.png"));
  inventory_add_item(pawn->inventory, axe);
  //Item* gun = item_create(pos, scale, "Gun", "Shoot people", GUN, get_texture("assets/items/weapons/guns/rifle.png"));
  //gun->is_active = true;
  //inventory_add_item(pawn->inventory, gun);

  pawn->texture = get_texture("assets/pawns/pawn_blue.png");
  pawn->text_texture = NULL;
  pawn->is_dead = false;
  pawn->is_controled = false;
  pawn->flip = false;

  // Add the pawn to the pawns
  manager->pawns[manager->count++] = pawn;

  return pawn;
}

void pawn_destroy(Pawn* pawn) {
  if (pawn) {
    inventory_destroy(pawn->inventory);
    SDL_DestroyTexture(pawn->texture);
    SDL_DestroyTexture(pawn->text_texture);
    free(pawn);
  }
}

void pawn_update(Pawn* pawn) {
  pawn->particle_system->pos.x = (pawn->pos.x + pawn->scale.x/2)-4;
  pawn->particle_system->pos.y = (pawn->pos.y + pawn->scale.y)-4;
  pawn->pos.x += pawn->dir.x;
  pawn->pos.y += pawn->dir.y;

  for (size_t i = 0; i < pawn->inventory->count; ++i) {
    pawn->inventory->items[i]->pos = pawn->pos;
    pawn->inventory->items[i]->flip = pawn->flip;
  }

  if (pawn->is_controled) {
    global.camera->scale = pawn->scale;
  }

  pawn->dir = (Vector2D){0, 0};
  if (pawn->is_controled) {
    global.camera->dir = pawn->pos;
    if (global.key_state[SDLK_LSHIFT % MAX_KEYS]) pawn->speed *= 2;
    if (global.key_state[SDLK_w] % MAX_KEYS) {
      particle_system_add_particle(pawn->particle_system, (Vector2D){8, 8}, (Vector2D){random_value(-1, 1), random_value(-1, 1)}, get_texture("assets/world/dirt_particle.png"));
      pawn->dir.y -= pawn->speed;
    }
    if (global.key_state[SDLK_s] % MAX_KEYS) {
      pawn->dir.y += pawn->speed;
      particle_system_add_particle(pawn->particle_system, (Vector2D){8, 8}, (Vector2D){random_value(-1, 1), random_value(-1, 1)}, get_texture("assets/world/dirt_particle.png"));
    }
    if (global.key_state[SDLK_a] % MAX_KEYS) {
      particle_system_add_particle(pawn->particle_system, (Vector2D){8, 8}, (Vector2D){random_value(-1, 1), random_value(-1, 1)}, get_texture("assets/world/dirt_particle.png"));
      pawn->dir.x -= pawn->speed;
      pawn->flip = SDL_FLIP_HORIZONTAL;
    }
    if (global.key_state[SDLK_d] % MAX_KEYS) {
      particle_system_add_particle(pawn->particle_system, (Vector2D){8, 8}, (Vector2D){random_value(-1, 1), random_value(-1, 1)}, get_texture("assets/world/dirt_particle.png"));
      pawn->dir.x += pawn->speed;
      pawn->flip = SDL_FLIP_NONE;
    }
    if (global.key_state[SDLK_LSHIFT % MAX_KEYS]) pawn->speed /= 2;
    if (global.key_state[SDLK_c] % MAX_KEYS) inventory_use_item(pawn->inventory, 1);
    //if (global.key_state[SDLK_1] % MAX_KEYS) pawn->inventory->items[0]->is_active = ! pawn->inventory->items[0]->is_active;
    //if (global.key_state[SDLK_2] % MAX_KEYS) pawn->inventory->items[1]->is_active = ! pawn->inventory->items[1]->is_active;
  }
}

void pawn_render(Pawn* pawn) {
  SDL_SetRenderDrawColor(global.renderer, 200, 200, 200, 255);

  float time = SDL_GetTicks() / 1000.0f;
  float anim = 3 * sinf(6 * (pawn->is_controled ? pawn->speed : 1) * time);

  SDL_Rect camera_rect = {
    pawn->pos.x * global.camera->zoom - global.camera->pos.x,
    pawn->pos.y * global.camera->zoom - global.camera->pos.y + (int)anim,
    pawn->scale.x * global.camera->zoom,
    pawn->scale.y * global.camera->zoom
  };
  SDL_Rect camera_shadow_rect = camera_rect;
  camera_shadow_rect.y += 8;

  SDL_Rect camera_text_rect = {
    pawn->pos.x * global.camera->zoom - global.camera->pos.x + (pawn->scale.x/4.f * global.camera->zoom),
    pawn->pos.y * global.camera->zoom - global.camera->pos.y + (64.f * global.camera->zoom),
    pawn->scale.x/2 * global.camera->zoom,
    pawn->scale.y/3.5 * global.camera->zoom
  };

  if (global.show_coll) SDL_RenderDrawRect(global.renderer, &camera_rect);
  if (pawn->texture) {
    SDL_SetTextureColorMod(pawn->texture, 0, 0, 0);
    SDL_SetTextureAlphaMod(pawn->texture, 191);
    SDL_RenderCopyEx(global.renderer, pawn->texture, NULL, &camera_shadow_rect, 0, NULL, pawn->flip);
    SDL_SetTextureColorMod(pawn->texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(pawn->texture, 255);
    SDL_RenderCopyEx(global.renderer, pawn->texture, NULL, &camera_rect, 0, NULL, pawn->flip);
  }
  if (pawn->text_texture) {
    SDL_SetRenderDrawColor(global.renderer, 20, 20, 20, 255);
    SDL_RenderFillRect(global.renderer, &camera_text_rect);
    SDL_RenderCopy(global.renderer, pawn->text_texture, NULL, &camera_text_rect);
  }
  inventory_render(pawn->inventory);
}

void pawn_on_click() {
  printf("Clicked!\n");
}

PawnManager* pawn_manager_create(size_t initial_capacity) {
  // Allocate some memory for the manager
  PawnManager* manager = malloc(sizeof(PawnManager));
  if (!manager) return NULL;

  manager->pawns = malloc(initial_capacity * sizeof(Pawn*));
  manager->count = 0;
  manager->capacity = initial_capacity;

  return manager;
}

void pawn_manager_destroy(PawnManager* manager) {
  for (size_t i = 0; i < manager->count; ++i) {
    pawn_destroy(manager->pawns[i]);
  }

  free(manager->pawns);
  free(manager);
}

void pawn_manager_update(PawnManager* manager) {
  for (size_t i = 0; i < manager->count; ++i) {
    Pawn* pawn = manager->pawns[i];
    pawn_update(pawn);
  }
}

void pawn_manager_render(PawnManager* manager) {
  for (size_t i = 0; i < manager->count; ++i) {
    Pawn* pawn = manager->pawns[i];
    pawn_render(pawn);
    inventory_render(pawn->inventory);
    SDL_RenderCopy(global.renderer, pawn->texture, NULL, &(SDL_Rect){global.screen_width/2.f-(pawn->scale.x/2)*i, 0, 50, 50});
  }
}

void pawn_manager_inputs(SDL_Event* event) {
  if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_1) pawn_manager_controll_pawn(0);
  if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_2) pawn_manager_controll_pawn(1);
}

void pawn_manager_remove(PawnManager* manager, int id) {
  if (!manager || id >= manager->count) return;

  // Free pawn memory
  pawn_destroy(manager->pawns[id]);

  // Shift remaining pawns left to fill the gap
  for (size_t i = id; i < manager->count - 1; ++i) {
    manager->pawns[i] = manager->pawns[i + 1];
  }

  manager->count--;

  manager->pawns = realloc(manager->pawns, sizeof(Pawn*) * manager->capacity);
}

void pawn_manager_controll_pawn(int id) {
  for (size_t i = 0; i < global.pawn_manager->count; ++i) {
    if (id == i) {
      global.pawn_manager->pawns[id]->is_controled = !global.pawn_manager->pawns[id]->is_controled;
      global.camera->follow_target = global.pawn_manager->pawns[id]->is_controled;
    }
    else global.pawn_manager->pawns[i]->is_controled = false;
  }
}
