#include "game/camera.h"
#include "core/global.h"
#include <SDL2/SDL_keycode.h>

Camera* camera_create() {
  Camera* camera = malloc(sizeof(Camera));

  camera->pos.x = 0;
  camera->pos.y = 0;
  camera->speed = 4;
  camera->zoom = 1;
  camera->follow_target = false;

  return camera;
}

void camera_update(Camera* camera) {
  if (camera->zoom <= 0.4) camera->zoom = 0.4;
  if (camera->zoom >= 2.0) camera->zoom = 2.0;

  // Calculate the target position for the camera to center the character
  int target_x = camera->dir.x * camera->zoom - (global.screen_width / 2.f) + (int)(camera->scale.x / 2) * camera->zoom;
  int target_y = camera->dir.y * camera->zoom - (global.screen_height / 2.f) + (int)(camera->scale.y / 2) * camera->zoom;
  
  // Update camera position smoothly towards the target position
  camera->pos.x += (target_x - camera->pos.x) / (camera->follow_target ? 14 : 1);
  camera->pos.y += (target_y - camera->pos.y) / (camera->follow_target ? 14 : 1);
  
  // Manual camera movement when not following a target
  if (!camera->follow_target) {
    if (global.key_state[SDLK_LSHIFT % MAX_KEYS]) camera->speed *= 2;
    if (global.key_state[SDLK_w % MAX_KEYS]) camera->dir.y -= camera->speed;
    if (global.key_state[SDLK_s % MAX_KEYS]) camera->dir.y += camera->speed;
    if (global.key_state[SDLK_a % MAX_KEYS]) camera->dir.x -= camera->speed;
    if (global.key_state[SDLK_d % MAX_KEYS]) camera->dir.x += camera->speed;
    if (global.key_state[SDLK_LSHIFT % MAX_KEYS]) camera->speed /= 2;
  }
  if (global.key_state[SDLK_r % MAX_KEYS]) global.camera->zoom += 0.02;
  if (global.key_state[SDLK_f % MAX_KEYS]) global.camera->zoom -= 0.02;
}

