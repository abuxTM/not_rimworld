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

Pawn* pawn_create(PawnManager* manager, Vector2D pos, Vector2D scale) {
  if (manager->count >= manager->capacity) {
    manager->capacity *= 2;
    manager->pawns = (Pawn**)realloc(manager->pawns, manager->capacity * sizeof(Pawn*));
  }

  Pawn* pawn = (Pawn*)malloc(sizeof(Pawn));
  if (!pawn) return NULL;

  pawn->id = manager->count;
  pawn->health = 100;
  pawn->speed = 2;
  pawn->pos = pos;
  pawn->scale = scale;
  pawn->texture = get_texture("assets/SkinSheerWhite.png");
  pawn->is_dead = false;
  pawn->is_controled = false;

  manager->pawns[manager->count++] = pawn;

  global.camera->scale = pawn->scale;

  return pawn;
}

void pawn_destroy(Pawn* pawn) {
  if (pawn) { SDL_DestroyTexture(pawn->texture); free(pawn); }
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
  float anim = 3 * sinf(6 * time);

  SDL_Rect camera_rect = {
    pawn->pos.x - global.camera->pos.x,
    pawn->pos.y - global.camera->pos.y + (int)anim,
    pawn->scale.x,
    pawn->scale.y
  };

  //SDL_RenderDrawRect(global.renderer, &camera_rect);
  if (pawn->texture) SDL_RenderCopy(global.renderer, pawn->texture, NULL, &camera_rect);
}

void pawn_on_click() {
  printf("Clicked!\n");
}

PawnManager* pawn_manager_create(size_t initial_capacity) {
  PawnManager* manager = (PawnManager*)malloc(sizeof(PawnManager));

  manager->pawns = (Pawn**)malloc(initial_capacity * sizeof(Pawn*));
  manager->count = 0;
  manager->capacity = initial_capacity;

  return manager;
}

void pawn_manager_destroy(PawnManager* manager) {
  for (size_t i = 0; i < manager->count; ++i) {
    free(manager->pawns[i]);
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
