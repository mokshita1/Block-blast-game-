
# Block Blast Game

### Overview

The **Block Blast Game** is an interactive puzzle game developed using C++ and the Qt framework. It leverages the power of Data Structures, including **vectors (QVector)** and **matrices**, to create an engaging game experience.

The game features a ""10x10 grid"" where the player can place and blast blocks. The game includes a **start menu**, **game panel**, **timer**, **score tracking**, and **game over screen**. The objective is to place blocks within the grid and complete rows and columns to score points. The game ends either when no valid moves are left or when the timer runs out.

### Features

* **Start Menu**:

  * **Instructions**: Brief game rules and objectives.
  * **Start Button**: Begins the game and transitions to the game panel.
    
* **Game Panel**:

  * A **10x10 grid** where blocks are placed.
  * Three **auto-generated blocks** appear below the grid at the beginning and after each completed placement.
  * **Timer**: Countdown timer that adds a sense of urgency to the game, timer adds up for every block placed.
  * **Score**: Points are earned by successfully placing blocks (10 points) and clearing rows/columns (50 points).
    
* **Game Over Panel**:

  * Displays when the game ends due to running out of valid moves or time.
  * Two options are provided:

    * **Play Again**: Restarts the game and brings you back to the game panel.
    * **Exit**: Exits the game and returns you to the start menu.

### Data Structures Used

* **QVector**: Used to create the grid and to store and manage the blocks and grid positions. It ensures efficient handling of the grid and block data.
* **Matrix**: Represents the shape of the blocks.

### Game Flow

1. **Start Menu**:

   * Displays an introductory screen with instructions and a "Start" button.

2. **Game Panel**:

   * Upon clicking the **Start** button, the game moves to the grid display with a **10x10 grid**.
   * Three blocks are randomly generated at the start and placed at the bottom of the grid.
   * The player places these blocks within the grid to form valid patterns. Completing rows or columns increases the score.
   * As the blocks are placed, new blocks are generated automatically.
  
     
3. **Game Over**:

   * The game ends when either of the following conditions are met:

     * The ""timer runs out"".
     * There are ""no valid moves"" left to place the blocks.
   * The game transitions to the ""Game Over panel"", where the player has the option to ""Play Again"" or ""Exit"".

### Getting Started

To run the Block Blast Game on your machine:

2. Open the project in ""Qt Creator"".

3. Build and run the project. The game will start with the ""Start Menu"".

### Technologies Used

* C++
* Qt Framework
* QVector
* Matrix Data Structure


