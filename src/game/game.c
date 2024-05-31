#include "game/game.h"
#include "core/global.h"
#include "core/init.h"
#include "core/renderer.h"
#include "core/window.h"
#include "game/camera.h"
#include "game/mouse.h"
#include "game/pawn_manager.h"
#include "game/ui/ui.h"
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
#include <time.h>

Camera* camera = NULL;
World* world = NULL;

Tab* footer = NULL;
PawnUI* pawn_ui = NULL;

void game_init(void) {
  // Default init
  sdl_init();
  window_init();
  renderer_init();
  srand(time(NULL));

  SDL_RenderSetLogicalSize(global.renderer, global.screen_width, global.screen_height);
  SDL_RenderSetIntegerScale(global.renderer, SDL_TRUE);

  // Global init
  global.mouse = mouse_init();
  global.camera = camera_create();
  global.camera->follow_target = false;
  world = world_create(100);
  global.pawn_manager = pawn_manager_create(10);
  memset(global.key_state, 0, sizeof(global.key_state));

  // Tests init
  pawn_create((Vector2D){64, 0}, (Vector2D){64, 64});
  pawn_create((Vector2D){0, 0}, (Vector2D){64, 64});

  footer = tab_create(
    (Vector2D){0, global.screen_height-30},
    (Vector2D){global.screen_width, 30},
    10
  );
  tab_add_button_lined(footer, "BUILDING", NULL, true);
  tab_add_button_lined(footer, "WORLD", NULL, true);
  tab_add_button_lined(footer, "YES", NULL, false);
  tab_add_button_lined(footer, "WORLD", NULL, true);
  tab_add_button_lined(footer, "PAWNS", NULL, true);
  tab_add_panel(footer, NULL, true);
  pawn_ui = pawn_ui_create((Vector2D){6, 6}, (Vector2D){100, 40});

  for (float y = 0; y < 20; ++y) {
    for (float x = 0; x < 20; ++x) {
      block_create(world, (Vector2D){x*62, y*62}, (Vector2D){TILE_SIZE, TILE_SIZE});
    }
  }

  game_loop();
}

void game_cleanup(void) {
  free(global.mouse);
  free(global.camera);
  tab_destroy(footer);
  pawn_ui_destroy(pawn_ui);
  world_destroy(world);
  pawn_manager_destroy(global.pawn_manager);
  SDL_DestroyRenderer(global.renderer);
  SDL_DestroyWindow(global.window);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void game_update(void) {
  camera_update(global.camera);
  pawn_manager_update(global.pawn_manager);
}

void game_render(void) {
  SDL_SetRenderDrawColor(global.renderer, 20, 20, 20, 255);
  SDL_RenderClear(global.renderer);

  world_render(world);
  pawn_manager_render(global.pawn_manager);
  pawn_ui_render(pawn_ui);
  mouse_render(world);
  tab_render(footer);

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
      pawn_manager_inputs(&event);
      mouse_inputs(&event);
    }

    if (global.key_state[SDLK_LCTRL % MAX_KEYS] && global.key_state[SDLK_c % MAX_KEYS]) global.should_quit = true;
    if (global.key_state[SDLK_r % MAX_KEYS]) global.camera->zoom += .02;
    if (global.key_state[SDLK_f % MAX_KEYS]) global.camera->zoom -= .02;

    game_update();
    game_render();
  }

  game_cleanup();
}

