#ifndef GLOBAL_H
#define GLOBAL_H

#include "game/mouse.h"
#include "game/particle_manager.h"
#include "game/pawn_manager.h"
#define TILE_SIZE 64
#define MAX_KEYS 256

#include "game/camera.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
  SDL_Window* window;
  SDL_Renderer* renderer;
  TTF_Font* font;

  int screen_width;
  int screen_height;

  Mouse* mouse;
  Camera* camera;
  PawnManager* pawn_manager;
  ParticleManager* particle_manager;

  bool key_state[MAX_KEYS];
  bool should_quit;
  bool show_coll;

  double deltaTime;
} Global;

extern Global global;

#endif
