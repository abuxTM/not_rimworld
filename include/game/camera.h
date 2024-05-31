#ifndef CAMERA_H
#define CAMERA_H

#include "utils/remath.h"
#include <SDL2/SDL_rect.h>
#include <stdbool.h>

typedef struct {
  Vector2D pos, dir, scale;
  float speed;
  float zoom;
  bool follow_target;
} Camera;

Camera* camera_create();
void camera_update(Camera* camera);

#endif
