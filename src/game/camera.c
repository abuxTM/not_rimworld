#include "game/camera.h"
#include "core/global.h"
#include <SDL2/SDL_keycode.h>

Camera* camera_create() {
  Camera* camera = (Camera*)malloc(sizeof(Camera));

  camera->pos.x = 0;
  camera->pos.y = 0;
  camera->speed = 4;

  return camera;
}

void camera_update(Camera* camera) {
  // Calculate the target position for the camera to center the character
  float target_x = camera->dir.x - (global.screen_width / 2.0f) + (camera->scale.x / 2.0f);
  float target_y = camera->dir.y - (global.screen_height / 2.0f) + (camera->scale.y / 2.0f);
  
  // Update camera position smoothly towards the target position
  camera->pos.x += (target_x - camera->pos.x) / (camera->follow_target ? 15.0f : 1.0f);
  camera->pos.y += (target_y - camera->pos.y) / (camera->follow_target ? 15.0f : 1.0f);
  
  // Manual camera movement when not following a target
  if (!camera->follow_target) {
    if (global.key_state[SDLK_LSHIFT % MAX_KEYS]) camera->speed *= 2;
    if (global.key_state[SDLK_w % MAX_KEYS]) camera->dir.y -= camera->speed;
    if (global.key_state[SDLK_s % MAX_KEYS]) camera->dir.y += camera->speed;
    if (global.key_state[SDLK_a % MAX_KEYS]) camera->dir.x -= camera->speed;
    if (global.key_state[SDLK_d % MAX_KEYS]) camera->dir.x += camera->speed;
    if (global.key_state[SDLK_LSHIFT % MAX_KEYS]) camera->speed /= 2;
  }
}

