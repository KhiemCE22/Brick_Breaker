# Brick Breaker Game on STM32

This is a classic Brick Breaker game implemented on an BKIT-ARM4 board. The project utilizes various peripherals to create an interactive and engaging gaming experience.

## Hardware Requirements

-   **BKIT-ARM4 Board:** The main microcontroller board.
-   **LCD Display:** For rendering the game graphics.
-   **Potentiometer:** Used to control the paddle's movement.
-   **Buttons:** For game interactions like starting, pausing, and restarting the game.
-   **Buzzer:** To provide audio feedback during the game.

## Features

-   **Classic Brick Breaker Gameplay:** Destroy all the bricks on the screen with a bouncing ball.
-   **Paddle Control:** Use a potentiometer to move the paddle left and right.
-   **Multiple Game States:** The game includes a start screen, playing state, paused state, and a game-over screen.
-   **Score and Lives:** Keep track of your score and the number of lives remaining.
-   **Audio Feedback:** A buzzer provides sound effects for game events.
-   **Special Bricks:** Some bricks have special effects when destroyed.

## How to Play

1.  **Start Game:** Press `BUTTON 1` on the start screen to begin the game.
2.  **Control the Paddle:** Rotate the potentiometer to move the paddle horizontally at the bottom of the screen.
3.  **Gameplay:** Use the paddle to hit the ball and destroy the bricks.
4.  **Pause/Resume:** Press `BUTTON 4` to pause the game and press it again to resume.
5.  **Game Over:** The game ends when you lose all your lives. You can restart the game by pressing `BUTTON 5`.



## Project Structure

The project is organized into the following main directories:

-   `Core/`: Contains the main application source code.
    -   `Inc/`: Header files (`.h`).
        -   `main.h`: Main header file.
        -   `game_logic.h`: Contains the core game logic.
        -   `game_ui.h`: Handles the game's user interface and rendering.
        -   Other peripheral driver headers.
    -   `Src/`: Source files (`.c`).
        -   `main.c`: The main program entry point and game loop.
        -   `game_logic.c`: Implementation of the game logic.
        -   `game_ui.c`: Implementation of the UI rendering functions.
        -   Other peripheral driver source files.
-   `Drivers/`: Contains the STM32 HAL drivers and CMSIS files.
-   `Startup/`: The startup file for the STM32 microcontroller.

## Video Demo

[Watch the video demo here](https://youtu.be/TaV1FcbDuhg)

## Author: DreamTeam CE22
-   Phan Lê Hậu
-   Nguyễn Minh Hưng
-   Nguyễn Văn Huynh
-   Nguyễn Duy Khiêm
-   Lê Quang Trưng
