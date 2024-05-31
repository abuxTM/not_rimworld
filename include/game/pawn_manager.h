#ifndef PAWN_MANAGER_H
#define PAWN_MANAGER_H

#include "game/item_manager/inventory.h"
#include "utils/remath.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <sys/types.h>

typedef struct Pawn Pawn;

struct Pawn {
  const char* name;
  int id, health, speed;
  Vector2D pos, dir, scale;
  Inventory* inventory;
  SDL_Texture* texture;
  SDL_Texture* text_texture;
  bool is_dead;
  bool is_controled;
  SDL_RendererFlip flip;
};

typedef struct {
  Pawn** pawns;
  size_t count;
  size_t capacity;
} PawnManager;

/// Constructor
PawnManager* pawn_manager_create(size_t initial_capacity);
/// Deconstructor
void pawn_manager_destroy(PawnManager* manager);

/// Function for updating all pawns
void pawn_manager_update(PawnManager* manager);
/// Function for rendering all pawns sprites
void pawn_manager_render(PawnManager* manager);
void pawn_manager_inputs(SDL_Event* event);
/// Function for removing a pawn in run-time
void pawn_manager_remove(PawnManager* manager, int id);

void pawn_manager_controll_pawn(int id);


/// Constructor
Pawn* pawn_create(Vector2D pos, Vector2D scale);
/// Deconstrucor
void pawn_destroy(Pawn* pawn);

/// Function for updating pawn logic
void pawn_update(Pawn* pawn);

/// Function for rendering pawn sprites
void pawn_render(Pawn* pawn);

void pawn_on_click();

#endif
