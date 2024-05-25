#ifndef PAWN_MANAGER_H
#define PAWN_MANAGER_H

#include "utils/remath.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <sys/types.h>

typedef struct Pawn Pawn;

struct Pawn {
  int id;
  int health;
  int speed;
  Vector2D pos, dir;
  Vector2D scale;
  SDL_Texture* texture;
  bool is_dead;
  bool is_controled;
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


/// Constructor
Pawn* pawn_create(PawnManager* manager, Vector2D pos, Vector2D scale);
/// Deconstrucor
void pawn_destroy(Pawn* pawn);

/// Function for updating pawn logic
void pawn_update(Pawn* pawn);

/// Function for rendering pawn sprites
void pawn_render(Pawn* pawn);

void pawn_on_click();

#endif
