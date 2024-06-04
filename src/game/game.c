#include "game/game.h"
#include "core/global.h"
#include "core/init.h"
#include "core/renderer.h"
#include "core/window.h"
#include "game/camera.h"
#include "game/mouse.h"
#include "game/particle_manager.h"
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
Tab* footer_building = NULL;
Tab_Manager* tab_manager = NULL;
PawnUI* pawn_ui = NULL;

void toggle_building() {footer_building->is_active = !footer_building->is_active;}

void game_init(void) {
  // Default init
  sdl_init();
  window_init();
  renderer_init();
  srand(time(NULL));

  //SDL_RenderSetLogicalSize(global.renderer, global.screen_width, global.screen_height);
  //SDL_RenderSetIntegerScale(global.renderer, SDL_TRUE);
  SDL_GetWindowSize(global.window, &global.screen_width, &global.screen_height);

  // Global init
  global.mouse = mouse_init();
  global.camera = camera_create();
  global.camera->follow_target = false;
  world = world_create(100);
  tab_manager = tab_manager_create(10);
  global.pawn_manager = pawn_manager_create(10);
  global.particle_manager = particle_manager_create(40);
  memset(global.key_state, 0, sizeof(global.key_state));

  // Tests init
  pawn_create((Vector2D){64*20, 64*20}, (Vector2D){64, 64});

  footer = tab_create(
    tab_manager,
    (Vector2D){0, global.screen_height-30},
    (Vector2D){global.screen_width, 30},
    10
  );
  tab_add_button_lined(footer, "BUILDING", toggle_building, true);
  tab_add_button_lined(footer, "WORLD", NULL, true);
  tab_add_button_lined(footer, "YES", NULL, false);
  tab_add_button_lined(footer, "WORLD", NULL, true);
  tab_add_button_lined(footer, "PAWNS", NULL, true);
  tab_add_panel(footer, NULL, true);

  footer_building = tab_create(
    tab_manager,
    (Vector2D){0, global.screen_height-142},
    (Vector2D){160, 100},
    10
  );
  footer_building->is_active = false;
  tab_add_button(footer_building, "FURNITURE", (Vector2D){0, -20}, (Vector2D){200, 40}, NULL, true);
  tab_add_button(footer_building, "PRODUCTION", (Vector2D){0, 24}, (Vector2D){200, 40}, NULL, true);
  tab_add_button(footer_building, "POWER", (Vector2D){0, 68}, (Vector2D){200, 40}, NULL, true);
  pawn_ui = pawn_ui_create((Vector2D){6, 6}, (Vector2D){100, 40});

  game_loop();
}

void game_cleanup(void) {
  free(global.mouse);
  free(global.camera);
  tab_manager_destroy(tab_manager);
  pawn_ui_destroy(pawn_ui);
  particle_manager_destroy();
  world_destroy(world);
  pawn_manager_destroy(global.pawn_manager);
  SDL_DestroyRenderer(global.renderer);
  SDL_DestroyWindow(global.window);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void game_update(void) {
  SDL_GetWindowSize(global.window, &global.screen_width, &global.screen_height);
  camera_update(global.camera);
  pawn_manager_update(global.pawn_manager);
  particle_manager_update();
}

void game_render(void) {
  SDL_SetRenderDrawColor(global.renderer, 20, 20, 20, 255);
  SDL_RenderClear(global.renderer);

  world_render(world);
  particle_manager_render();
  pawn_manager_render(global.pawn_manager);
  pawn_ui_render(pawn_ui);
  mouse_render();
  tab_manager_render(tab_manager);

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
      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_TAB:
            toggle_building();
            break;
          default:
            break;
        }
      }
      pawn_manager_inputs(&event);
      mouse_inputs(&event, world);
      tab_manager_handle_events(tab_manager, &event);
    }

    if (global.key_state[SDLK_LCTRL % MAX_KEYS] && global.key_state[SDLK_c % MAX_KEYS]) global.should_quit = true;

    game_update();
    game_render();
  }

  game_cleanup();
}

