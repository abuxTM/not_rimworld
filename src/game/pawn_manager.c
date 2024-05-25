#include "game/pawn_manager.h"
#include "core/global.h"
#include "game/texture_manager.h"
#include "utils/remath.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* names[] = {"Alice", "Bob", "Kamil", "Reduan"};
const size_t num_names = sizeof(names) / sizeof(names[0]);

Pawn* pawn_create(PawnManager* manager, Vector2D pos, Vector2D scale) {
  // Add extra capacity to the manager on overflow
  if (manager->count >= manager->capacity) {
    manager->capacity *= 2;
    manager->pawns = (Pawn**)realloc(manager->pawns, manager->capacity * sizeof(Pawn*));
    if (!manager->pawns) return NULL;
  }

  // Allocate new memory for the pawn
  Pawn* pawn = malloc(sizeof(Pawn));
  if (!pawn) return NULL;

  size_t random_id = rand() % num_names;
  const char* random_name = names[random_id];

  // Set default parameters
  pawn->name = strdup(random_name);
  pawn->id = manager->count;
  pawn->health = 100;
  pawn->speed = 2;
  pawn->pos = pos;
  pawn->dir = (Vector2D){0, 0};
  pawn->scale = scale;
  pawn->texture = get_texture("assets/SkinSheerWhite.png");
  pawn->text_texture = get_font_texture(pawn->name, 100);
  pawn->is_dead = false;
  pawn->is_controled = false;

  // Add the pawn to the pawns
  manager->pawns[manager->count++] = pawn;

  global.camera->scale = pawn->scale;

  return pawn;
}

void pawn_destroy(Pawn* pawn) {
  if (pawn) {
    SDL_DestroyTexture(pawn->texture);
    SDL_DestroyTexture(pawn->text_texture);
    free(pawn);
  }
}

void pawn_update(Pawn* pawn) {
  pawn->pos.x += pawn->dir.x;
  pawn->pos.y += pawn->dir.y;

  pawn->dir = (Vector2D){0, 0};
  if (pawn->is_controled) {
    global.camera->dir = pawn->pos;
    if (global.key_state[SDLK_LSHIFT % MAX_KEYS]) pawn->speed *= 2;
    if (global.key_state[SDLK_w] % MAX_KEYS) pawn->dir.y = -pawn->speed;
    if (global.key_state[SDLK_s] % MAX_KEYS) pawn->dir.y = pawn->speed;
    if (global.key_state[SDLK_a] % MAX_KEYS) pawn->dir.x = -pawn->speed;
    if (global.key_state[SDLK_d] % MAX_KEYS) pawn->dir.x = pawn->speed;
    if (global.key_state[SDLK_LSHIFT % MAX_KEYS]) pawn->speed /= 2;
  }
}

void pawn_render(Pawn* pawn) {
  SDL_SetRenderDrawColor(global.renderer, 200, 200, 200, 255);

  float time = SDL_GetTicks() / 1000.0f;
  float anim = 3 * sinf(6 * (pawn->is_controled ? pawn->speed : 1) * time);

  SDL_Rect camera_rect = {
    pawn->pos.x - global.camera->pos.x,
    pawn->pos.y - global.camera->pos.y + (int)anim,
    pawn->scale.x,
    pawn->scale.y
  };

  SDL_Rect camera_text_rect = {
    pawn->pos.x - global.camera->pos.x + pawn->scale.x/4.f,
    pawn->pos.y - global.camera->pos.y + 64.f,
    pawn->scale.x/2,
    pawn->scale.y/3
  };

  if (global.show_coll) SDL_RenderDrawRect(global.renderer, &camera_rect);
  if (pawn->texture) SDL_RenderCopy(global.renderer, pawn->texture, NULL, &camera_rect);
  if (pawn->text_texture) {
  SDL_SetRenderDrawColor(global.renderer, 20, 20, 20, 255);
    SDL_RenderFillRect(global.renderer, &camera_text_rect);
    SDL_RenderCopy(global.renderer, pawn->text_texture, NULL, &camera_text_rect);
  }
}

void pawn_on_click() {
  printf("Clicked!\n");
}

PawnManager* pawn_manager_create(size_t initial_capacity) {
  // Allocate some memory for the manager
  PawnManager* manager = malloc(sizeof(PawnManager));

  manager->pawns = (Pawn**)malloc(initial_capacity * sizeof(Pawn*));
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
  }
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

  manager->pawns = realloc(manager->pawns, sizeof(Pawn*) * manager->count);
}
