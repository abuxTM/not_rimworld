#ifndef GAME_H
#define GAME_H

/// Function for initializng main game components
void game_init(void);
/// Function for cleaning everything up
void game_cleanup(void);

/// Function for updating game logic
void game_update(void);
/// Function for rendering game
void game_render(void);
/// Function for initializng main game loop
void game_loop(void);

#endif
