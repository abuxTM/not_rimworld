#ifndef UI_H
#define UI_H

#include "game/pawn_manager.h"
#include "utils/remath.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

typedef struct {
  const char* text;

  Vector2D pos, scale;
  SDL_Texture* text_texture;

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

Button* button_create(const char* text, Vector2D pos, Vector2D scale, void (*on_click)(void));
void button_destroy(Button* button);
void button_handle_event(Button* button, SDL_Event* event);
void button_render(Button* button);
void button_set_text(Button* button, const char* text);

Panel* panel_create(Vector2D pos, Vector2D scale);
void panel_destroy(Panel* panel);
void panel_render(Panel* panel);

typedef struct {
  Vector2D pos, scale;
  Button* pawn_health;
  Button* pawn_speed;
  Panel* panel;
  bool is_active;
} PawnUI;

PawnUI* pawn_ui_create(Vector2D pos, Vector2D scale);
void pawn_ui_destroy(PawnUI* ui);
void pawn_ui_render(PawnUI* ui);
void pawn_ui_toggle(PawnUI* ui, Pawn* pawn);

#endif
