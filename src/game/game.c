#include "game/game.h"
#include "core/global.h"
#include "core/init.h"
#include "core/renderer.h"
#include "core/window.h"
#include "game/camera.h"
#include "game/pawn_manager.h"
#include "game/ui.h"
#include "game/world.h"
#include "utils/remath.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

Camera* camera = NULL;
World* world = NULL;
PawnManager* pawn_manager = NULL;

Button* building_btn = NULL;
Button* work_btn = NULL;
Button* world_btn = NULL;

PawnUI* pawn_ui = NULL;

void button_one() {
  global.camera->follow_target = !global.camera->follow_target;
  pawn_manager->pawns[0]->is_controled = !pawn_manager->pawns[0]->is_controled;
  pawn_ui_toggle(pawn_ui, pawn_manager->pawns[0]);
}

void button_two() {
  global.camera->follow_target = !global.camera->follow_target;
  pawn_manager->pawns[1]->is_controled = !pawn_manager->pawns[1]->is_controled;
}

void game_init(void) {
  // Default init
  sdl_init();
  window_init();
  renderer_init();

  SDL_RenderSetLogicalSize(global.renderer, global.screen_width, global.screen_height);
  SDL_RenderSetIntegerScale(global.renderer, SDL_TRUE);

  // Global init
  global.font = TTF_OpenFont("assets/fonts/Norse.otf", 24);
  global.camera = camera_create();
  global.camera->follow_target = false;
  world = world_create(100);
  pawn_manager = pawn_manager_create(10);
  memset(global.key_state, 0, sizeof(global.key_state));

  // Tests init
  building_btn = button_create("Building", (Vector2D){6, global.screen_height-46}, (Vector2D){100, 40}, button_one);
  work_btn = button_create("Work", (Vector2D){112, global.screen_height-46}, (Vector2D){100, 40}, button_two);
  world_btn = button_create("World", (Vector2D){218, global.screen_height-46}, (Vector2D){100, 40}, button_two);
  pawn_create(pawn_manager, (Vector2D){0, 0}, (Vector2D){64, 64});
  pawn_create(pawn_manager, (Vector2D){35, 0}, (Vector2D){64, 64});

  pawn_ui = pawn_ui_create((Vector2D){6, 6}, (Vector2D){100, 40});

  for (float y = 0; y < 20; ++y) {
    for (float x = 0; x < 20; ++x) {
      block_create(world, (Vector2D){x*66, y*68}, (Vector2D){62, 64});
    }
  }

  game_loop();
}

void game_cleanup(void) {
  free(global.camera);
  button_destroy(building_btn);
  button_destroy(work_btn);
  button_destroy(world_btn);
  pawn_ui_destroy(pawn_ui);
  world_destroy(world);
  pawn_manager_destroy(pawn_manager);
  SDL_DestroyRenderer(global.renderer);
  SDL_DestroyWindow(global.window);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void game_update(void) {
  camera_update(global.camera);
  pawn_manager_update(pawn_manager);
}

void game_render(void) {
  SDL_SetRenderDrawColor(global.renderer, 20, 20, 20, 255);
  SDL_RenderClear(global.renderer);

  world_render(world);
  pawn_manager_render(pawn_manager);
  button_render(building_btn);
  button_render(work_btn);
  button_render(world_btn);
  pawn_ui_render(pawn_ui);

  SDL_RenderPresent(global.renderer);
}

void game_loop(void) {
  SDL_Event event;

  Uint32 last_time = SDL_GetTicks();

  while (!global.should_quit) {
    Uint32 current_time = SDL_GetTicks();
    global.deltaTime = (current_time - last_time) / 1000.0;
    last_time = current_time;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) global.should_quit = true;
      if (event.type == SDL_KEYDOWN) global.key_state[event.key.keysym.sym % MAX_KEYS] = true;
      if (event.type == SDL_KEYUP) global.key_state[event.key.keysym.sym % MAX_KEYS] = false;
    button_handle_event(building_btn, &event);
    }

    if (global.key_state[SDLK_LCTRL % MAX_KEYS] && global.key_state[SDLK_c % MAX_KEYS]) global.should_quit = true;

    game_update();
    game_render();
  }

  game_cleanup();
}

