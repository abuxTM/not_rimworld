#include "game/ui.h"
#include "core/global.h"
#include "game/pawn_manager.h"
#include "game/texture_manager.h"
#include "utils/remath.h"
#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

Button* button_create(const char* text, Vector2D pos, Vector2D scale, void (*on_click)(void)) {
  Button* button = malloc(sizeof(Button));
  if (!button) return NULL;

  button->pos = pos;
  button->scale = scale;
  button->text_texture = get_font_texture(text, 60);
  button->is_active =  true;
  button->is_hovered = false;
  button->is_clicked = false;
  button->on_click = on_click;

  return button;
}

void button_destroy(Button* button) {
  SDL_DestroyTexture(button->text_texture);
  free(button);
}

void button_handle_event(Button* button, SDL_Event* event) {
  if (button->is_active) {
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
}

void button_render(Button* button) {
  float border = 12.f;
  float text_padding = 12.f;
  SDL_Rect rect = { button->pos.x, button->pos.y, button->scale.x, button->scale.y };
  SDL_Rect border_rect = { button->pos.x - border/2.f, button->pos.y - border/2.f, button->scale.x + border, button->scale.y + border };
  SDL_Rect text_rect = { button->pos.x + text_padding/2.f, button->pos.y + text_padding/2.f, button->scale.x - text_padding, button->scale.y - text_padding };

  if (button->is_active) {
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
}

void button_set_text(Button* button, const char* text) {
  button->text = text;
  button->text_texture = get_font_texture(text, 80);
}

Panel* panel_create(Vector2D pos, Vector2D scale) {
  Panel* panel = malloc(sizeof(Panel));

  panel->pos = pos;
  panel->scale = scale;
  panel->texture = NULL;
  panel->is_active = false;
  
  return panel;
}

void panel_destroy(Panel* panel) {
  if (panel->texture) SDL_DestroyTexture(panel->texture);
  free(panel);
}

void panel_render(Panel* panel) {
  SDL_Rect rect = { panel->pos.x, panel->pos.y, panel->scale.x, panel->scale.y };

  if (panel->is_active) SDL_RenderFillRect(global.renderer, &rect);
}

PawnUI* pawn_ui_create(Vector2D pos, Vector2D scale) {
  PawnUI* pawn_ui = malloc(sizeof(PawnUI));
  
  pawn_ui->pos = pos;
  pawn_ui->scale = scale;
  pawn_ui->is_active = false;

  pawn_ui->pawn_health = button_create("", pos, scale, NULL);
  pawn_ui->pawn_health->is_active = pawn_ui->is_active;

  pawn_ui->pawn_speed = button_create("", (Vector2D){pos.x+106, pos.y}, scale, NULL);
  pawn_ui->pawn_health->is_active = pawn_ui->is_active;

  pawn_ui->panel = panel_create(pos, (Vector2D){scale.x*2, scale.y*2});
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

