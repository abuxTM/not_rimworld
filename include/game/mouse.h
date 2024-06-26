#ifndef MOUSE_H
#define MOUSE_H

#include "game/world.h"
#include "utils/remath.h"
#include <SDL2/SDL_events.h>
#include <stdbool.h>

typedef enum {
  NORMAL,
  BUILDING
} State;

typedef struct {
  VectorI2D pos;
  SDL_Rect rect;
  State state;
} Mouse;

Mouse* mouse_init();

void mouse_render();
void mouse_inputs(SDL_Event* event, World* world);

#endif
