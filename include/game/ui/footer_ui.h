#ifndef FOOTER_UI_H
#define FOOTER_UI_H

#include "game/ui/ui.h"
#include <SDL2/SDL_events.h>
#include <stdbool.h>

typedef struct {
  Button* building;
  Button* pawns;
  Panel* panel;
  bool is_active;
} FooterUI;

FooterUI* footer_ui_create();
void footer_ui_destroy(FooterUI* footer);
void footer_ui_render(FooterUI* footer);
void footer_ui_inputs(FooterUI* footer, SDL_Event* event);

void footer_ui_building_tab();
void footer_ui_pawns_tab();

#endif
