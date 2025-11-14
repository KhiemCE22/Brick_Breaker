# Game UI Module (`game_ui`)

## Overview

This module is responsible for all rendering aspects of the Brick Breaker game. It handles drawing the initial game scene, updating object positions on the screen, and displaying various game state screens like "Pause" and "Game Over".

The logic part of the application should call functions from this module to reflect changes in the game state on the display.

## Core Data Structures

The entire state of the game that is relevant for the UI is encapsulated in the `GameState` struct.

```c
// From: Core/Inc/game_ui.h

typedef struct {
    Brick bricks[BRICK_ROWS][BRICK_COLS];
    Ball ball;
    Paddle paddle;
    uint32_t score;
    uint8_t lives;
    GameStatus status;
    uint8_t show_potentiometer_prompt;
} GameState;
```

-   `bricks`: A 2D array representing the grid of bricks.
-   `ball`: The game ball object.
-   `paddle`: The player's paddle object.
-   `score`: The current score.
-   `lives`: The number of lives remaining.
-   `status`: The current status of the game (see Game States below).
-   `show_potentiometer_prompt`: A flag to display a prompt for the player to use the potentiometer.

## Game States (`GameStatus`)

The `GameStatus` enum defines the different states the game can be in, each with a corresponding UI representation.

-   `GAME_START_SCREEN`: The initial screen when the game loads.
-   `GAME_PLAYING`: The main game screen where gameplay occurs. The UI shows the paddle, ball, bricks, score, and lives.
-   `GAME_PAUSED`: A pause menu overlay is displayed, showing the "PAUSED" message and current score/lives.
-   `GAME_OVER`: A "Game Over" screen is displayed with the final score.

## UI Rendering Functions

The game logic should primarily interact with the following functions to render the UI.

-   `void game_init_state(GameState *state)`: Initializes a `GameState` struct to the default starting state for a new game.
-   `void game_draw_initial_scene(const GameState *state)`: Performs a full redraw of the game screen. This should be called when the game starts or when resuming from a state that requires clearing the screen (e.g., after the pause menu).
-   `void game_update_screen(GameState *state)`: Performs an efficient, partial update of the screen by erasing the ball and paddle at their previous positions and redrawing them at their new positions. This should be called in the main game loop for smooth animation.
-   `void game_draw_pause_screen(const GameState *state)`: Draws the pause menu.
-   `void game_clear_pause_screen(const GameState *state)`: Clears the pause menu and redraws the main game scene.
-   `void game_draw_game_over_screen(const GameState *state)`: Draws the game over screen.
-   `void game_erase_brick(const Brick* brick)`: Erases a single brick from the screen. This should be called when a brick is destroyed.

## Special UI Elements

### Special Bricks (`BrickSpecial`)

Some bricks have special properties, visually indicated as follows:

-   `BRICK_SPECIAL_BALL`: The brick is drawn with a white circle inside it.
-   `BRICK_SPECIAL_PLUS`: The brick is drawn with a black, balanced plus sign inside it.

The game logic is responsible for implementing the effects of these special bricks.

### Potentiometer Prompt

When the `show_potentiometer_prompt` flag in `GameState` is set to `1`, a prompt is displayed to the user. It consists of a dashed-line box containing the text:
```
ROTATE POTENTIOMETER
      TO PLAY
```
This is intended to be shown at the start of the game to instruct the player on the controls.

## How to Use

1.  Create a `GameState` variable.
2.  Call `game_init_state()` to set up the initial state.
3.  Modify the `GameState` struct as the game logic progresses (e.g., updating ball/paddle positions, changing score).
4.  In the main game loop:
    -   Update ball and paddle coordinates in the `GameState` struct.
    -   Call `game_update_screen()` to render the changes.
5.  When a brick is hit, update its state in the `GameState` struct and call `game_erase_brick()`.
6.  When the game state changes (e.g., to `GAME_PAUSED`), call the appropriate drawing function (`game_draw_pause_screen`, etc.).
