#include "game/ui/ui.h"
#include "core/global.h"
#include "game/pawn_manager.h"
#include "game/texture_manager.h"
#include "utils/remath.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

Button* button_create(const char* text, Vector2D pos, Vector2D scale, void (*on_click)(void)) {
  Button* button = malloc(sizeof(Button));
  if (!button) return NULL;

  button->pos = pos;
  button->scale = scale;
  button->texture = get_texture("assets/world/grass_tile.png");
  button->text_texture = get_font_texture(text, 25);
  button->is_active =  true;
  button->is_hovered = false;
  button->is_clicked = false;
  button->on_click = on_click;

  return button;
}

void button_destroy(Button* button) {
  if (button->texture) SDL_DestroyTexture(button->texture);
  if (button->text_texture->texture) SDL_DestroyTexture(button->text_texture->texture);
  free(button->text_texture);
  free(button);
}

void button_handle_event(Button* button, SDL_Event* event) {
  if (!button->is_active) return;
  // Check if button is hovered or not
  button->is_hovered = (
    global.mouse->pos.x >= button->pos.x && global.mouse->pos.x <= button->pos.x + button->scale.x &&
    global.mouse->pos.y >= button->pos.y && global.mouse->pos.y <= button->pos.y + button->scale.y
  );
  if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
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
  float text_padding = 12.f;
  SDL_Rect rect = {
    button->pos.x,
    button->pos.y,
    button->scale.x,
    button->scale.y
  };
  SDL_Rect rect_shadow = rect;
  rect_shadow.w += 8;
  rect_shadow.h += 8;
  rect_shadow.x -= 4;
  rect_shadow.y -= 4;
  SDL_Rect text_rect = {
    (button->pos.x + (button->scale.x - button->text_texture->width)/2) + text_padding/2,
    (button->pos.y + (button->scale.y - button->text_texture->height)/2) + text_padding/4,
    button->text_texture->width, button->text_texture->height
  };

  if (button->is_active) {
    SDL_SetRenderDrawColor(global.renderer, 0, 0, 0, 100);  // Black border
    if (button->is_hovered) {
      SDL_SetRenderDrawColor(global.renderer, 40, 21, 11, 255);  // Light gray when hovered
    } else {
      SDL_SetRenderDrawColor(global.renderer, 133, 97, 67, 255);  // Dark gray otherwise
    }
    SDL_RenderFillRect(global.renderer, &rect);
    SDL_SetTextureColorMod(button->texture, 0, 0, 0);
    SDL_SetTextureAlphaMod(button->texture, 191);
    SDL_RenderCopy(global.renderer, button->texture, NULL, &rect_shadow);
    SDL_SetTextureColorMod(button->texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(button->texture, 255);
    SDL_SetRenderDrawColor(global.renderer, 80, 89, 62, 255);
    SDL_RenderFillRect(global.renderer, &rect);
    //SDL_RenderCopy(global.renderer, button->texture, NULL, &rect);

    SDL_RenderCopy(global.renderer, button->text_texture->texture, NULL, &text_rect);
  }
}

void button_set_text(Button* button, const char* text) {
  SDL_DestroyTexture(button->text_texture->texture);
  button->text_texture = get_font_texture(text, 30);
}

Panel* panel_create(Vector2D pos, Vector2D scale, SDL_Texture* texture) {
  Panel* panel = malloc(sizeof(Panel));
  if (!panel) return NULL;

  panel->pos = pos;
  panel->scale = scale;
  panel->texture = texture;
  panel->is_active = true;
  
  return panel;
}

void panel_destroy(Panel* panel) {
  if (panel->texture) SDL_DestroyTexture(panel->texture);
  free(panel);
}

void panel_render(Panel* panel) {
  if (!panel->is_active) return;
  SDL_Rect rect = { panel->pos.x, panel->pos.y, panel->scale.x, panel->scale.y };

  SDL_SetRenderDrawColor(global.renderer, 0, 0, 0, 255);
  (panel->texture)
    ? SDL_RenderCopy(global.renderer, panel->texture, NULL, &rect)
    : SDL_RenderFillRect(global.renderer, &rect);
}

Tab* tab_create(Tab_Manager* manager, Vector2D pos, Vector2D scale, size_t initial_capacity) {
  if (manager->count >= manager->capacity) {
    manager->capacity *= 2;
    manager->tabs = realloc(manager->tabs, manager->capacity * sizeof(Tab*));
    if (!manager->tabs) return NULL;
  }

  Tab* tab = malloc(sizeof(Tab));
  if (!tab) return NULL;

  tab->pos = pos;
  tab->scale = scale;
  tab->buttons = malloc(initial_capacity * sizeof(Button*));
  tab->panels = malloc(initial_capacity * sizeof(Panel*));
  tab->button_count = 0;
  tab->panel_count = 0;
  tab->capacity = initial_capacity;
  tab->is_active = true;

  manager->tabs[manager->count++] = tab;

  return tab;
}

void tab_destroy(Tab* tab) {
  for (size_t i = 0; i < tab->button_count; ++i) {
    if (tab->buttons[i]) button_destroy(tab->buttons[i]);
  }
  for (size_t i = 0; i < tab->panel_count; ++i) {
    if (tab->panels[i]) panel_destroy(tab->panels[i]);
  }
  free(tab->buttons);
  free(tab->panels);
}

void tab_render(Tab* tab) {
  if (!tab->is_active) return;
  for (size_t i = 0; i < tab->panel_count; ++i) {
    panel_render(tab->panels[i]);
  }
  for (size_t i = 0; i < tab->button_count; ++i) {
    button_render(tab->buttons[i]);
  }
}

void tab_handle_events(Tab* tab, SDL_Event* event) {
  if (!tab->is_active) return;
  for (size_t i = 0; i < tab->button_count; ++i) {
    button_handle_event(tab->buttons[i], event);
  }
}

void tab_add_button_lined(Tab* tab, const char* text, void (*on_click)(void), bool is_active) {
  if (tab->button_count >= tab->capacity) {
    tab->capacity *= 2;
    tab->buttons = realloc(tab->buttons, tab->capacity * sizeof(Button*));
  }

  // Calculate the position and scale for the new button
  float button_width = tab->scale.x / (tab->button_count + 1);
  float button_height = tab->scale.y;
  Vector2D pos = { tab->pos.x, tab->pos.y };
  Vector2D scale = { button_width, button_height };

  // Create the new button
  Button* button = button_create(text, pos, scale, on_click);
  button->is_active = is_active;

  // Adjust the position and scale of all existing buttons
  for (int i = 0; i < tab->button_count; i++) {
    tab->buttons[i]->scale.x = button_width;
    tab->buttons[i]->pos.x = tab->pos.x + i * button_width;
  }

  // Set the position of the new button
  button->pos.x = tab->pos.x + tab->button_count * button_width;

  // Add the new button to the tab
  tab->buttons[tab->button_count++] = button;
}

void tab_add_button(Tab* tab, const char* text, Vector2D pos, Vector2D scale, void (*on_click)(void), bool is_active) {
  if (tab->button_count >= tab->capacity) {
    tab->capacity *= 2;
    tab->buttons = realloc(tab->buttons, tab->capacity * sizeof(Button*));
  }
  Button* button = button_create(text, pos, scale, on_click);
  button->pos.x += tab->pos.x;
  button->pos.y += tab->pos.y;
  button->is_active = is_active;
  tab->buttons[tab->button_count++] = button;
}

void tab_add_panel(Tab* tab, SDL_Texture* texture, bool is_active) {
  if (tab->panel_count >= tab->capacity) {
    tab->capacity *= 2;
    tab->panels = realloc(tab->panels, tab->capacity * sizeof(Panel*));
  }
  Panel* panel = panel_create(tab->pos, tab->scale, texture);
  panel->is_active = is_active;
  tab->panels[tab->panel_count++] = panel;
}

Tab_Manager* tab_manager_create(size_t initial_capacity) {
  Tab_Manager* manager = malloc(sizeof(Tab_Manager));
  if (!manager) return NULL;

  manager->tabs = malloc(initial_capacity * sizeof(Tab*));
  manager->count = 0;
  manager->capacity = initial_capacity;

  return manager;
}

void tab_manager_destroy(Tab_Manager* manager) {
  if (!manager) return;
  for (size_t i = 0; i < manager->count; ++i) {
    tab_destroy(manager->tabs[i]);
  }
  free(manager->tabs);
}

void tab_manager_render(Tab_Manager* manager) {
  for (size_t i = 0; i < manager->count; ++i) {
    tab_render(manager->tabs[i]);
  }
}

void tab_manager_handle_events(Tab_Manager* manager, SDL_Event* event) {
  for (size_t i = 0; i < manager->count; ++i) {
    tab_handle_events(manager->tabs[i], event);
  }
}

PawnUI* pawn_ui_create(Vector2D pos, Vector2D scale) {
  PawnUI* pawn_ui = malloc(sizeof(PawnUI));
  
  pawn_ui->pos = pos;
  pawn_ui->scale = scale;
  pawn_ui->is_active = false;

  pawn_ui->pawn_health = button_create(" ", pos, scale, NULL);
  pawn_ui->pawn_health->is_active = pawn_ui->is_active;

  pawn_ui->pawn_speed = button_create(" ", (Vector2D){pos.x+106, pos.y}, scale, NULL);
  pawn_ui->pawn_health->is_active = pawn_ui->is_active;

  pawn_ui->panel = panel_create(pos, (Vector2D){scale.x*2, scale.y*2}, NULL);
  pawn_ui->panel->is_active = pawn_ui->is_active;

  return pawn_ui;
}

void pawn_ui_destroy(PawnUI* ui) {
  if (ui) {
    button_destroy(ui->pawn_health);
    button_destroy(ui->pawn_speed);
    panel_destroy(ui->panel);
    free(ui);
  }
}

void pawn_ui_render(PawnUI* ui) {
  if (ui->is_active) {
    //panel_render(ui->panel);
    button_render(ui->pawn_health);
    button_render(ui->pawn_speed);
  }
}

void pawn_ui_toggle(PawnUI* ui, Pawn* pawn) {
  ui->is_active = !ui->is_active;

  ui->pawn_health->is_active = ui->is_active;
  ui->pawn_health->is_active = ui->is_active;
  ui->panel->is_active = ui->is_active;

  if (ui->is_active) {
    char buffer[50];
    snprintf(buffer, 50, "Health: %d", pawn->health);
    button_set_text(ui->pawn_health, buffer);
    snprintf(buffer, 50, "Speed: %d", pawn->speed);
    button_set_text(ui->pawn_speed, buffer);
  }
}

