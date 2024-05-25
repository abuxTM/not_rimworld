#ifndef UI_H
#define UI_H

#include "utils/remath.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>

typedef struct {
  Vector2D pos;
  Vector2D scale;
  SDL_Texture* text_texture;
  const char* text;

  bool is_hovered;
  bool is_clicked;
  void (*on_click)(void);
} Button;

typedef struct {
    Button** buttons;
    int count;
    int capacity;
    bool show_pawn_panel;
} UIManager;

Button* button_create(const char* text, Vector2D pos, Vector2D scale, void (*on_click)(void));
void button_destroy(Button* button);
void button_handle_event(Button* button, SDL_Event* event);
void button_render(Button* button);

UIManager* ui_manager_create(int initial_capacity);
void ui_manager_destroy(UIManager* manager);
void ui_manager_handle_event(UIManager* manager, SDL_Event* event);
void ui_manager_render(UIManager* manager);

void ui_show_pawn_panel(UIManager* manager);

#endif
