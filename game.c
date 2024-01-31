/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 * Blake Wang
 * Lance Chen
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 4//dimention of the game.
#define WGAME 2048//win condition of the game

//Blocks is the abstraction of every tile in the 4x4 grid
//In other words, the main grid is made up of 4 Blocks arrays each with  4 Blocks in it
//The Blocks have two properties: value (the value each tile holds) and filled (indicator of whether they are filled or not)
//These properties are useful when updating the grid after each player move
typedef struct{
    int value;
    int filled;
}Blocks;

//Dimensions is a simplified struct that is used mainly for checking whether the block is filled, which helps random)generation
typedef struct{
    int rows;
    int cols;
}Dimensions;

//Initialize the 4x4 Blocks grid on the head, with the values and filled initialized to 0
Blocks** init_grid(int rows, int cols){
    Blocks **grid = malloc(sizeof(Blocks *) * rows);
    for (int i = 0; i < rows; i++){
        grid[i] = malloc(sizeof(Blocks *)*cols);
        for (int j = 0; j < cols; j++){
            grid[i][j].value = 0;
            grid[i][j].filled = 0;
        }
    }
    return grid;
}

//Function Prototypes
void display_grid(Blocks **grid);
void display_header(int rounds, int score, Blocks **grid);
int has_valid_moves(Blocks **grid);
int obtained_2048(Blocks **grid);
int upwards_column(int column[]);
int upwards_grid(Blocks **grid, int score);
int downwards_column(int column[]);
int downwards_grid(Blocks **grid, int score);
int rightward_row(int row[], int score);
int rightward_grid(Blocks **grid, int score);
int leftward_row(int row[]);
int leftwards_grid(Blocks **grid, int score);
void random_generation(Blocks **grid);
void free_grid(Blocks **grid, int rows);
int is_effective_move(Blocks **grid, char move);
int manual();
int move_grid(int score, char choice, Blocks **grid);
void summary(int win, Blocks **grid);
void play(int win, int flag, int rounds, int score, Blocks **grid);

int main() {
    int play_check = manual();
    if (play_check){
        Blocks **grid = init_grid(DIM,DIM);
        int flag = has_valid_moves(grid);
        int win = obtained_2048(grid);
        int rounds = 1;
        int score = 0;
        play(win, flag, rounds, score, grid);
        free_grid(grid, DIM);
    }
    else{
        printf("See you next time!\n");
    }
}


//Displays the current state of the grid for player observation
//for numbers with different digits, the display is updated to make sure the grid looks good
void display_grid(Blocks **grid) {
    printf("_____________________\n");
    for (int i = 0; i < DIM; i++) {
        printf("______ ______ ______ ______\n");

        printf("|    | |    | |    | |    |\n");
        for (int j = 0; j < DIM; j++) {
            if (grid[i][j].value < 10){
                printf("|  %d | ", grid[i][j].value);
            }
            else if (grid[i][j].value < 100){
                printf("| %d | ", grid[i][j].value);
            }
            else if (grid[i][j].value < 1000){
                printf("| %d| ", grid[i][j].value);
            }
            else if (grid[i][j].value < 10000){
                printf("|%d| ", grid[i][j].value);
            }
        }
        printf("\n");
        printf("|    | |    | |    | |    |\n");
        printf("‾‾‾‾‾‾ ‾‾‾‾‾‾ ‾‾‾‾‾‾ ‾‾‾‾‾‾\n");

    }
}

//displays the head properties at each round of the game
//displays the: Round number, current score, the current grid, and asks the user for an input move
void display_header(int rounds, int score, Blocks **grid){
    printf("Round #%d     Score: %d\n", rounds, score);
    display_grid(grid);
    printf("Enter a letter: w(up) / a(left) / s(down) / d(right)\n");
}

//checks whether there are valid moves for the player
//There ARE moves when:
//1. There's a blank tile (i.e. the Block is not filled)
//2. There are possible merge operations for the player
//For 2, we go into the second nested for loop and check the current value with its neighbors to see there are values equal to it
//Return 1 if there are, 0 if not. 0 usually means its game over
int has_valid_moves(Blocks **grid){
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (grid[i][j].filled == 0) {
                return 1;
            }
        }
    }
    for (int i = 0; i < DIM; i++){
        for (int j = 0; j < DIM; j++) {
            int val = grid[i][j].value;

            // Check above
            if (i > 0 && grid[i - 1][j].value == val) {
                return 1;
            }
            // Check below
            if (i < (DIM-1) && grid[i + 1][j].value == val) {
                return 1;
            }
            // Check left
            if (j > 0 && grid[i][j - 1].value == val) {
                return 1;
            }
            // Check right
            if (j < (DIM-1) && grid[i][j + 1].value == val) {
                return 1;
            }
        }
    }
    return 0;
}

//checks whether 2048 is contained in the current game grid using a nested for loop
//returns 1 (true) if it is, 0 otherwise
int obtained_2048(Blocks **grid){
    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (grid[i][j].value == WGAME) {
                return 1;
            }
        }
    }
    return 0;
}

//calculates and updates the grid for an upwards movement (on one column)
//merges adjacent tiles if they are the same and move all tiles within the column upwards
//The merge & move algorithm:
//1. Create an auxiliary array up_column
//2. Loop through each element in the input 'column'
//3. Merge equal adjacent tiles and move tiles upwards, score is handled in the merge process
//4. Map the new state back to the original 'column' - no malloc needed
//5. Return the scores gained from this column movement
int upwards_column(int column[]){
    int up_column[DIM];
    for (int i = 0; i<DIM; i++){
        up_column[i] = 0;
    }
    int idp_idx = 0;
    int updated_score = 0;
    for (int i = 0; i<DIM; i++){
        if (column[i] == 0) continue;  
        if (up_column[idp_idx] == 0){
            up_column[idp_idx] = column[i];
        }
        else if (up_column[idp_idx] == column[i]) {
            up_column[idp_idx] *= (DIM-2);
            updated_score = up_column[idp_idx];
            idp_idx++;
        } 
        else {
            idp_idx++;
            up_column[idp_idx] = column[i];
        }
    }
    for (int i = 0; i < DIM; i++) {
        column[i] = up_column[i];
    }
    return updated_score;
}

//Move the entire grid upwards by calling upwards_column on each column
//Modifies the grid status, whether each block is filled, and updates the current score
int upwards_grid(Blocks **grid, int score){
    int return_score = score;
    for (int col = 0; col < DIM; col++) {
        int column[DIM];
        for (int row = 0; row < DIM; row++) {
            column[row] = grid[row][col].value;
        }
        return_score += upwards_column(column);
        for (int row = 0; row < DIM; row++) {
            grid[row][col].value = column[row];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
    }
    return return_score;
}

//Basically the same as upwards_column() but the process is reversed
//Updates the state of one column, computes scores
int downwards_column(int column[]){
    int down_column[DIM]; 
    for (int i = 0; i<DIM; i++){
        down_column[i] = 0;
    }
    int idp_idx = (DIM-1); 
    int updated_score = 0;

    for (int i = (DIM-1); i >= 0; i--) {
        if (column[i] == 0) continue;  
        if (down_column[idp_idx] == 0) {
            down_column[idp_idx] = column[i];  
        } else if (down_column[idp_idx] == column[i]) {
            down_column[idp_idx] *= (DIM-2);  
            updated_score = down_column[idp_idx];
            idp_idx--; 
            down_column[idp_idx] = 0;  
        } else {
            idp_idx--;  
            down_column[idp_idx] = column[i];  
        }
    }
    for (int i = 0; i < DIM; i++) {
        column[i] = down_column[i];
    }
    return updated_score;
}

//Similar with upwards_grid, calls downwards_column() on each abstracted column of the game grid
int downwards_grid(Blocks **grid, int score){
    int return_score = score;

    for (int col = 0; col < DIM; col++) {
        int column[DIM];
        for (int row = 0; row < DIM; row++) {
            column[row] = grid[row][col].value;
        }
        return_score += downwards_column(column);
        for (int row = 0; row < DIM; row++) {
            grid[row][col].value = column[row];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
    }
    return return_score;
}

//Shifts a row rightwards, update score in the process
//This is easier to implement as it processes the shifts within the current sub array of the grid, which is easier to manage
int rightward_row(int row[], int score){
    int right_row[DIM]; 
    for (int i = 0; i<DIM; i++){
        right_row[i] = 0;
    }
    int idp_idx = (DIM-1); 
    int updated_score = 0;

    for (int i = (DIM-1); i >= 0; i--) {
        if (row[i] == 0) continue;  
        if (right_row[idp_idx] == 0) {
            right_row[idp_idx] = row[i];  
        } else if (right_row[idp_idx] == row[i]) {
            right_row[idp_idx] *= (DIM-2); 
            updated_score += right_row[idp_idx]; 
            idp_idx--; 
            right_row[idp_idx] = 0;  
        } else {
            idp_idx--;  
            right_row[idp_idx] = row[i];  
        }
    }

    for (int i = 0; i < DIM; i++) {
        row[i] = right_row[i];
    }
    return updated_score;
}

//shifts all 4 rows (i.e. all columns in the grid) rightwards, updates the scores in the process, reassess the filled status of each block
int rightward_grid(Blocks **grid, int score){
    int return_score = score;
    for (int row = 0; row < DIM; row++) {
        //int *rows = malloc(sizeof(int) * 4);
        int rows[DIM];

        for (int col = 0; col < DIM; col++) {
            rows[col] = grid[row][col].value;
        }
        
        score += rightward_row(rows, score);

        for (int col = 0; col < DIM; col++) {
            grid[row][col].value = rows[col];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
    }
    return return_score;
}

//Honestly these shifts are the same at its core... but it cannot be integrated into one function because their differences cannot be addressed that easily
//But yeah this function handles the left-shift move on one row, using an auxiliary array to store temporary calculation and in the end stored back to the given row[]
int leftward_row(int row[]){
    int left_row[DIM];
    for (int i = 0; i<DIM; i++){
        left_row[i] = 0;
    }
    int idp_idx = 0;
    int updated_score = 0;
    for (int i = 0; i<DIM; i++){
        if (row[i] == 0) continue;  

        if (left_row[idp_idx] == 0){
            left_row[idp_idx] = row[i];
        }
        else if (left_row[idp_idx] == row[i]) {
            left_row[idp_idx] *= (DIM-2);
            updated_score+=left_row[idp_idx];
            idp_idx++;
        } 
        else {
            idp_idx++;
            left_row[idp_idx] = row[i];
        }
    }
    for (int i = 0; i < DIM; i++) {
        row[i] = left_row[i];
    }
    return updated_score;
}

//Shifts all rows in the grid leftwards, update scores and filled, etc.
int leftwards_grid(Blocks **grid, int score){
    int return_score = score;
    for (int row = 0; row < DIM; row++) {
        int rows[DIM];
        for (int col = 0; col < DIM; col++) {
            rows[col] = grid[row][col].value;
        }
        
        return_score += leftward_row(rows);

        for (int col = 0; col < DIM; col++) {
            grid[row][col].value = rows[col];
            if (grid[row][col].value == 0){
                grid[row][col].filled = 0;
            }
            else{
                grid[row][col].filled = 1;
            }
        }
    }
    return return_score;
}

//randomly generates a tile, either 2 or 4, on to the grid
//mainly called after each round
//90% chance for a 2 to be generated
//10% chance for a 4 to be generated
//updates the filled status of the grid after generation
void random_generation(Blocks **grid){
    Dimensions collection[16];
    int counter = 0;
    for (int row = 0; row < DIM; row++){
        for (int col = 0; col < DIM; col++){
            if (grid[row][col].filled == 0){
                collection[counter].rows = row;
                collection[counter].cols = col;
                counter++;
            }
        }
    }
    int random_num = rand() % counter - 1;
    if (random_num < 0){
        random_num = 0;
    }
    float two_four_decider = (float)rand() / (float)RAND_MAX;
    if (two_four_decider >= 0.9){
        grid[collection[random_num].rows][collection[random_num].cols].value = 4;
    }
    else{
        grid[collection[random_num].rows][collection[random_num].cols].value = (DIM-2);
    }
    grid[collection[random_num].rows][collection[random_num].cols].filled = 1;
}

//frees the grid on the heap
//frees every row, and the entire thing in the end
void free_grid(Blocks **grid, int rows) {
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

//checks whether a given move is effective (i.e. it will change the current grid layout)
//needed because in original 2048 game new tiles will be generated only if the move is effective
//simulates the grid movement in a temporary alias grid with exact properties as the current grid
//performs operation on the alias, compare it to the current grid
//if its the exact same return 0, else 1
//alias space on the heap is cleared at the end of the function
int is_effective_move(Blocks **grid, char move){
    int flag = 0;
    Blocks **alias = init_grid(DIM,DIM);
    for (int i = 0; i < DIM; i++){
        for (int j = 0; j < DIM; j++){
            alias[i][j].value = grid[i][j].value;
        }
    }
    if (move == 'w'){
        upwards_grid(alias, 0);
    }
    else if (move == 'a'){
        leftwards_grid(alias,0);
    }
    else if (move == 's'){
        downwards_grid(alias,0);
    }
    else if (move == 'd'){
        rightward_grid(alias,0);
    }
    for (int i = 0; i<DIM; i++){
        for (int j = 0; j < DIM; j++){
            if (alias[i][j].value != grid[i][j].value){
                flag = 1;
            }
        }
    }
    free_grid(alias, DIM);
    return flag;
}

//Displays the game rules and operations to the player, ask the user whether they want to play the game
//Returns an int bool depending on user input
int manual(){
    printf("Welcome to Command Line 2048!!!\n");
    printf("===============================\n\n");
    printf("How to play: \n");
    printf("Slide tiles on a 4x4 game grid by using w/a/s/d. \n");
    printf("Slide two same tiles together to form a larger tile.\n");
    printf("You win when you get a 2048 tile!\n\n");
    
    printf("Movements:\n");
    printf("\t- Use the 'w' key to slide up\n");
    printf("\t- Use the 'a' key to slide left\n");
    printf("\t- Use the 's' key to slide down\n");
    printf("\t- Use the 'd' key to slide right\n\n");

    printf("Scoring:\n");
    printf("\t- Each move generates a new tile (either 2 or 4) in an empty slot on the board.\n");
    printf("\t- When two tiles with the same number collide, they combine to form a single tile with double the value.\n\n");

    printf("Do you want to begin? y (yes) / n (and all other characters) (no)\n");
    char choice[3];//10 character should be enough to store input
    fgets(choice, 3, stdin);
    choice[2] = '\0';
    if (strcmp(choice, "y\n") == 0){
        return 1;
    }
    else {
        return 0;
    }
}

//move the grid as a response to user input choice, in the process updates the score
int move_grid(int score, char choice, Blocks **grid){
    int new_score = 0;
    if (choice == 'w'){
        new_score = upwards_grid(grid, score);
    }
    else if (choice == 'a'){
        new_score = leftwards_grid(grid, score);
    }
    else if (choice == 'd'){
        new_score = rightward_grid(grid, score);
    }
    else if (choice == 's'){
        new_score = downwards_grid(grid, score);
    }
    random_generation(grid);
    return new_score;
}

//summary of the game, pops up when the game is over
//Outputs different results based on whether the user won or lose
void summary(int win, Blocks **grid){
    display_grid(grid);
    if (win == 1){
        printf("You win! \n");
    }
    else{
        printf("Oh no. Game Over.\nBetter luck next time\n");
    }
}

//plays the game in a while loop, with two random_generations() called beforehand to give the users 2 values for the starting grid
//takes user input movement with fgets(), moves the grid if it is an effective move, evaluate whether there are any valid moves left, 
//and check whether the player won already
//if the move is ineefective, pops notification and nothing happens except round number
//validates input so that the grid will only move if the player enter w/a/s/d
//summarizes the game when the main game loop is over
void play(int win, int flag, int rounds, int score, Blocks **grid){
    random_generation(grid);
    random_generation(grid);
    while (flag == 1 && win==0){
        display_header(rounds, score, grid);
        char move[3];
        fgets(move, 3, stdin);
        move[2] = '\0';
        printf("Your move: %c\n", move[0]);
        if (move[0] == 'w' || move[0] == 'a' || move[0] == 's' || move[0]=='d'){
            int moves = is_effective_move(grid, move[0]);
            if (moves){
                score = move_grid(score, move[0], grid);
                flag = has_valid_moves(grid);
                win = obtained_2048(grid);
            }
            else{
                printf("=====================\n");
                printf(" Tiles did not change\n");
                printf("=====================\n");
            }
        }
        else{
            printf("Invalid input, try again.\n");
        }
        
        rounds++;
    }
    summary(win, grid);
}

