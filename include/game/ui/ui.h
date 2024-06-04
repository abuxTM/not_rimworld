#ifndef UI_H
#define UI_H

#include "game/pawn_manager.h"
#include "game/texture_manager.h"
#include "utils/remath.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

typedef struct {
  const char* text;

  Vector2D pos, scale;
  SDL_Texture* texture;
  FontTexture* text_texture;

  bool is_active;
  bool is_hovered;
  bool is_clicked;
  void (*on_click)(void);
} Button;

typedef struct {
  Vector2D pos, scale;
  SDL_Texture* texture;
  bool is_active;
} Panel;

typedef struct {
  Vector2D pos, scale;
  float min, max;
  bool is_active;
} Slider;

typedef struct {
  Vector2D pos, scale;
  Button** buttons;
  Panel** panels;
  size_t button_count;
  size_t panel_count;
  size_t capacity;
  float padding;
  bool is_active;
} Tab;

typedef struct {
  Tab** tabs;
  size_t count;
  size_t capacity;
} Tab_Manager;

typedef struct {
  Vector2D pos, scale;
  Button* pawn_health;
  Button* pawn_speed;
  Panel* panel;
  bool is_active;
} PawnUI;

// ██▄ █ █ ▀█▀ ▀█▀ ▄▀▄ █▄ █ ▀ ▄▀▀
// █▄█ ▀▄█  █   █  ▀▄▀ █ ▀█   ▄██

Button* button_create(const char* text, Vector2D pos, Vector2D scale, void (*on_click)(void));
void button_destroy(Button* button);
void button_handle_event(Button* button, SDL_Event* event);
void button_render(Button* button);
void button_set_text(Button* button, const char* text);

// █▀▄ ▄▀▄ █▄ █ ██▀ █   ▀ ▄▀▀
// █▀  █▀█ █ ▀█ █▄▄ █▄▄   ▄██

Panel* panel_create(Vector2D pos, Vector2D scale, SDL_Texture* texture);
void panel_destroy(Panel* panel);
void panel_render(Panel* panel);

// ▀█▀ ▄▀▄ ██▄ ▀ ▄▀▀
//  █  █▀█ █▄█   ▄██

Tab* tab_create(Tab_Manager* manager, Vector2D pos, Vector2D scale, size_t initial_capacity);
void tab_destroy(Tab* tab);
void tab_render(Tab* tab);
void tab_handle_events(Tab* tab, SDL_Event* event);
void tab_add_button(Tab* tab, const char* text, Vector2D pos, Vector2D scale, void (*on_click)(void), bool is_active);
void tab_add_button_lined(Tab* tab, const char* text, void (*on_click)(void), bool is_active);
void tab_add_panel(Tab* tab, SDL_Texture* texture, bool is_active);

Tab_Manager* tab_manager_create(size_t initial_capacity);
void tab_manager_destroy(Tab_Manager* manager);
void tab_manager_render(Tab_Manager* manager);
void tab_manager_handle_events(Tab_Manager* manager, SDL_Event* event);

PawnUI* pawn_ui_create(Vector2D pos, Vector2D scale);
void pawn_ui_destroy(PawnUI* ui);
void pawn_ui_render(PawnUI* ui);
void pawn_ui_toggle(PawnUI* ui, Pawn* pawn);

#endif
