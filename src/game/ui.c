#include "game/ui.h"
#include "core/global.h"
#include "game/texture_manager.h"
#include <SDL2/SDL_render.h>

Button* button_create(const char* text, Vector2D pos, Vector2D scale, void (*on_click)(void)) {
  UIManager* manager = global.ui_manager;

  if (manager->count >= manager->capacity) {
    manager->capacity *= 2;
    manager->buttons = (Button**)realloc(manager->buttons, manager->capacity * sizeof(Button*));
  }

  Button* button = (Button*)malloc(sizeof(Button));
  if (!button) return NULL;

  button->pos = pos;
  button->scale = scale;
  button->text_texture = get_font_texture(text, 60);
  button->is_hovered = false;
  button->is_clicked = false;
  button->on_click = on_click;

  manager->buttons[manager->count++] = button;

  return button;
}

void button_destroy(Button* button) {
  SDL_DestroyTexture(button->text_texture);
}

void button_handle_event(Button* button, SDL_Event* event) {
    if (event->type == SDL_MOUSEMOTION) {
        int mx = event->motion.x;
        int my = event->motion.y;
        button->is_hovered = (mx >= button->pos.x && mx <= button->pos.x + button->scale.x &&
                              my >= button->pos.y && my <= button->pos.y + button->scale.y);
    } else if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        if (button->is_hovered) {
            button->is_clicked = true;
            if (button->on_click) {
                button->on_click();
            }
        }
    } else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
        button->is_clicked = false;
    }
}

void button_render(Button* button) {
  float border = 12.f;
  float text_padding = 12.f;
  SDL_Rect rect = { button->pos.x, button->pos.y, button->scale.x, button->scale.y };
  SDL_Rect border_rect = { button->pos.x - border/2.f, button->pos.y - border/2.f, button->scale.x + border, button->scale.y + border };
  SDL_Rect text_rect = { button->pos.x + text_padding/2.f, button->pos.y + text_padding/2.f, button->scale.x - text_padding, button->scale.y - text_padding };

  SDL_SetRenderDrawColor(global.renderer, 0, 0, 0, 255);  // Black border
  SDL_RenderFillRect(global.renderer, &border_rect);
  if (button->is_hovered) {
      SDL_SetRenderDrawColor(global.renderer, 200, 200, 200, 255);  // Light gray when hovered
  } else {
      SDL_SetRenderDrawColor(global.renderer, 100, 100, 100, 255);  // Dark gray otherwise
  }
  SDL_RenderFillRect(global.renderer, &rect);

  SDL_RenderCopy(global.renderer, button->text_texture, NULL, &text_rect);
}

UIManager* ui_manager_create(int initial_capacity) {
    UIManager* manager = (UIManager*)malloc(sizeof(UIManager));

    manager->buttons = (Button**)malloc(sizeof(Button*) * initial_capacity);
    manager->count = 0;
    manager->capacity = initial_capacity;

    return manager;
}

void ui_manager_destroy(UIManager* manager) {
  for (size_t i = 0; i < manager->count; ++i) {
    button_destroy(manager->buttons[i]);
    free(manager->buttons[i]);
  }

  free(manager->buttons);
  free(manager);
}

void ui_manager_handle_event(UIManager* manager, SDL_Event* event) {
    for (int i = 0; i < manager->count; ++i) {
        button_handle_event(manager->buttons[i], event);
    }
}

void ui_manager_render(UIManager* manager) {
    for (int i = 0; i < manager->count; ++i) {
        button_render(manager->buttons[i]);
    }
}

void ui_show_pawn_panel(UIManager* manager) {

}
