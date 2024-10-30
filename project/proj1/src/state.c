#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
    // 1. initiate game state
    game_state_t *state = malloc(sizeof(game_state_t));
    if (!state) { // check for malloc failure
        return NULL;
    }

    // 2. initiate num_rows
    state->num_rows = 18;

    // 3. initiate board
    state->board = malloc(state->num_rows * sizeof(char *));
    if (!state->board) { // check for malloc failure
        free(state);
        return NULL;
    }

    for (int i = 0; i < state->num_rows; i++) {
        state->board[i] = malloc(22 * sizeof(char)); 
        if (!state->board[i]) {
            // free previously allocated rows and state
            for (int k = 0; k < i; k++){
                free(state->board[k]);
            }
            free(state->board);
            free(state);
            return NULL;
        }
        for (int j = 0; j < 20; j++) {
            if (i == 0 || i == 17 || j == 0 || j == 19) {
                state->board[i][j] = '#';
            } else {
                state->board[i][j] = ' ';
            }
        }
        state->board[i][20] = '\n'; 
        state->board[i][21] = '\0';
    }

    
    state->board[2][9] = '*';
    state->board[2][2] = 'd';
    state->board[2][3] = '>';
    state->board[2][4] = 'D';

    // 4. initiate num_snakes
    state->num_snakes = 1;

    // 5. initiate snake
    state->snakes = malloc(sizeof(snake_t));
    if (!state->snakes) { // check for malloc failure
        for (int i = 0; i < state->num_rows; i++) {
            free(state->board[i]);
        }
        free(state->board);
        free(state);
        return NULL;
    }

    state->snakes->tail_row = 2;
    state->snakes->tail_col = 2;
    state->snakes->head_row = 2;
    state->snakes->head_col = 4;
    state->snakes->live = true;
    
    return state; 
}

/* Task 2 */
void free_state(game_state_t *state) {
    // Handle the case where state is NULL
    if (state == NULL) { 
        return;
    }

    // Check if the board has been allocated
    if (state->board != NULL) { 
        for (unsigned int i = 0; i < state->num_rows; i++) {
            free(state->board[i]);
        }
        free(state->board);
    }

    // Check if snakes has been allocated
    if (state->snakes != NULL) { 
        free(state->snakes);
    }

    free(state);
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
    if (!state || !fp) { 
        return;
    }

    for (unsigned int i = 0; i < state->num_rows; i++) {
        if (state->board[i] != NULL) {  
            fprintf(fp, "%s", state->board[i]); 
        }
    }
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
    FILE *f = fopen(filename, "w");
    print_board(state, f);
    fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
    state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
    return (c == 'w' || c == 'a' || c == 's' || c == 'd');
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
    return (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x');
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
    return (is_tail(c) || is_head(c) || c == '^' || c == '<' || c == 'v' || c == '>');
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
    switch (c) {
        case '^': return 'w';
        case '<': return 'a';
        case 'v': return 's';
        case '>': return 'd';
        default: return '?'; 
    }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
    switch (c) {
        case 'W': return '^';
        case 'A': return '<';
        case 'S': return 'v';
        case 'D': return '>';
        default: return '?'; 
    }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
    if (c == 'v' || c == 's' || c == 'S') {
        return cur_row + 1;
    } else if (c == '^' || c == 'w' || c == 'W') {
        return cur_row - 1;
    } else {
        return cur_row;
    }
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
    if (c == '>' || c == 'd' || c == 'D') {
        return cur_col + 1;
    } else if (c == '<' || c == 'a' || c == 'A') {
        return cur_col - 1;
    } else {
        return cur_col;
    }
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
    if (!state || snum >= state->num_snakes) {
        return '?';
    }

    snake_t *snake = &state->snakes[snum];
    unsigned int next_row = get_next_row(snake->head_row, get_board_at(state, snake->head_row, snake->head_col));
    unsigned int next_col = get_next_col(snake->head_col, get_board_at(state, snake->head_row, snake->head_col));

    if (next_row >= state->num_rows || next_col >= strlen(state->board[0]) -1) {
        return '?';
    }
    return get_board_at(state, next_row, next_col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
    // Check for invalid input
    if (!state || snum >= state->num_snakes) { 
        return; 
    }

    // Get current head position and character
    unsigned int head_row = state->snakes[snum].head_row;
    unsigned int head_col = state->snakes[snum].head_col;
    char head_char = get_board_at(state, head_row, head_col);

    // Calculate new head position
    unsigned int new_row = get_next_row(head_row, head_char);
    unsigned int new_col = get_next_col(head_col, head_char);

    // Update the board: change current head to body
    set_board_at(state, head_row, head_col, head_to_body(head_char));

    // Place head character at new position
    set_board_at(state, new_row, new_col, head_char);

    // Update snake's head position in the struct
    state->snakes[snum].head_row = new_row;
    state->snakes[snum].head_col = new_col;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
    // Check for invalid input
    if (!state || snum >= state->num_snakes) { 
        return; 
    }
    
    // Get the snake's tail position and character
    snake_t *snake = &(state->snakes[snum]);
    unsigned int tail_row = snake->tail_row;
    unsigned int tail_col = snake->tail_col;
    char tail_char = get_board_at(state, tail_row, tail_col);

    // Remove the tail from the board
    set_board_at(state, tail_row, tail_col, ' ');

    // Calculate the new tail position
    unsigned int new_tail_row = get_next_row(tail_row, tail_char);
    unsigned int new_tail_col = get_next_col(tail_col, tail_char);

    // Get the character at the new tail position
    char new_tail_char = get_board_at(state, new_tail_row, new_tail_col);

    // Update the new tail character on the board
    set_board_at(state, new_tail_row, new_tail_col, body_to_tail(new_tail_char));

    // Update the tail position in the snake struct
    snake->tail_row = new_tail_row;
    snake->tail_col = new_tail_col;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
    for (unsigned int snum = 0; snum < state->num_snakes; snum++) {
        snake_t *snake = &(state->snakes[snum]);

        // Skip if the snake is not alive
        if (!snake->live) {
            continue;
        }

        // Determine the next square the snake will move into
        char next = next_square(state, snum);

        if (next == ' ') {
            // Move forward by updating head and tail
            update_head(state, snum);
            update_tail(state, snum);
        } else if (next == '*') {
            // Eat the fruit: update head and add new food
            update_head(state, snum);
            add_food(state);
        } else {
            // Snake dies: change head to 'x' and set live to false
            set_board_at(state, snake->head_row, snake->head_col, 'x');
            snake->live = false;
        }
    }
}

/* Task 5.1 */
char *read_line(FILE *fp) {
    // TODO: Implement this function.
    return NULL;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
    // TODO: Implement this function.
    return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
    // TODO: Implement this function.
    return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
    // TODO: Implement this function.
    return NULL;
}
