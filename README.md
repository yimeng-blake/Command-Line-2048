# Project 02: Grid Game

## Overview
In this project, you'll implement a game that uses a two-dimensional grid. You can choose from any of the following games:
* [2048](https://en.wikipedia.org/wiki/2048_(video_game))
* [Battleship](https://en.wikipedia.org/wiki/Battleship_(game))
* [Boggle](https://en.wikipedia.org/wiki/Boggle)
* [Connect Four](https://en.wikipedia.org/wiki/Connect_Four)
* [Crossword](https://en.wikipedia.org/wiki/Crossword)
* [Minesweeper](https://en.wikipedia.org/wiki/Minesweeper_(video_game))
* [Nonogram](https://en.wikipedia.org/wiki/Nonogram)
* [Sudoku](https://en.wikipedia.org/wiki/Sudoku)
* Your own idea that has been approved by your course instructor

### Standards
This project assesses the following standards:
* 1.2: Use variables, operators, conditionals, loops, and functions appropriately in C code
* 3.1: Use arrays, pointers, dynamic memory allocation, and structs appropriately in C code
* 3.3: Diagnose and correct pointer and dynamic memory errors and leaks in C code

### Important tips
* Read the entirety of the project description before you write any code.
* Work on the project over multiple sessions. Do not try to complete the project in a single session on the day (before) the project is due. This will give you time to think about how to solve problems, allow you to ask questions, and result in better outcomes.
* Ensure you follow good program design, coding, testing, and debugging practices (details below).

## Requirements
Write your code in the `game.c` file included in your repository. Use the included `Makefile` to compile your code.

Your game must be meet **all** of the following requirements:
* Allocate a 2D array on the heap to store the main game state
* Define and use a struct to store some aspect of the game state
* When the game starts, print instructions on how to play the game
* Read (e.g., using `fgets`) and validate user input
* Display the player's current progress/score each time the player takes an action
* End the game based on appropriate criteria
* Free all heap-allocated memory
* Run without any memory errors or leaks (check using `valgrind`)

## Program design

You **must follow good program design and coding practices**, including:
* Making multiple commits to your git repository: Do not wait until your entire program is working before you commit it to your git repository. You should commit your code each time you write and debug a piece of functionality (e.g., after writing and testing code to check the binary formula, perform addition, etc.)
* Properly indenting your code: Recall that indentation is not semantically significant in C, but it makes your code much easier to read.
* Using multiple functions: Do not put all of your code in the `main` function.
* Include comments: Each function (except for `main`), must be preceded by a short comment that describes what the function does.

## Testing and debugging your code
The first step in debugging your code is to fix any errors or warnings output by the compiler. You may be tempted to ignore the warnings, but they almost always mean there is a bug in your code.

Play your game several times with different inputs. If your program doesn't work as expected, add `printf` statements in strategic locations to help you understand what your program is doing.

Run your game with `valgrind` to detect memory errors and leaks that occur during game play.

## Submission instructions
You should **commit and push** your updated `game.c` file to your git repository. However, as noted above, do not wait until your entire program is working before you commit it to your git repository; you should commit your code each time you write and debug a piece of functionality.