#include "state.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  /* Input  : None
     Output : Create a default snake game in memory and return a
              pointer to the newly created game_state_t struct.
  */

  game_state_t* default_game_state = (game_state_t*) malloc(sizeof(game_state_t));
  snake_t* default_snek_struct = (snake_t*) malloc(sizeof(snake_t));

  default_game_state->num_rows = 18;
  default_game_state->num_snakes = 1;
  
  default_snek_struct->tail_row = 2;
  default_snek_struct->tail_col = 2;
  default_snek_struct->head_row = 2;
  default_snek_struct->head_col = 4;
  default_snek_struct->live = true;

  snake_t* snakes = (snake_t*) malloc(1 * sizeof(snake_t));
  snakes[0] = *default_snek_struct;
  default_game_state->snakes = snakes;

  char** board = (char**) malloc(default_game_state->num_rows * sizeof(char*));

  for (int i = 0; i < default_game_state->num_rows; i++) { 
    board[i] = (char*) malloc(21 * sizeof(char)); 
  }

  strcpy(board[0], "####################");
  strcpy(board[1], "#                  #");
  strcpy(board[2], "# d>D    *         #");
  strcpy(board[3], "#                  #");
  strcpy(board[4], "#                  #");
  strcpy(board[5], "#                  #");
  strcpy(board[6], "#                  #");
  strcpy(board[7], "#                  #");
  strcpy(board[8], "#                  #");
  strcpy(board[9], "#                  #");
  strcpy(board[10], "#                  #");
  strcpy(board[11], "#                  #");
  strcpy(board[12], "#                  #");
  strcpy(board[13], "#                  #");
  strcpy(board[14], "#                  #");
  strcpy(board[15], "#                  #");
  strcpy(board[16], "#                  #");
  strcpy(board[17], "####################");

  default_game_state->board = board;
  return default_game_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  /* Input  : Game state to be deleted.
     Output : Deletes and frees all memory occupied by the game state.
              Returns nothing.
  */
  
  for (int i = 0; i < state->num_snakes; i++) {
    free(&state->snakes[i]);
  }
  while(state->num_rows--){
    free(state->board[state->num_rows]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  /* Input  : Game state & a file pointer.
     Output : Saves the current state of the game in the provided file.
              While not modifying the state object.
  */
  
  for (int row = 0; row < state->num_rows; row++) {
    fprintf(fp, "%s\n", state->board[row]);
  }
  return;
}

void save_board(game_state_t* state, char* filename) {
  /* Input  : Game state & a file name.
     Output : Calls the print_board function to print the current board
              state in the provided file..
  */
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  /* Input  : Game state, row & column of required cell.
     Output : Returns the value at the specific row & column in the
              current game state.
  */

  return state->board[row][col];
}

static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  /* Input  : Game state, row & column of the cell and the new character value.
     Output : Updates the board with the provided value at the given row and
              column position.
  */

  state->board[row][col] = ch;
}

static bool is_tail(char c) {
  /* Input  : A singular character value.
     Output : Returns true if c is part of the snake's tail.
  */

  if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
    return true;
  }
  return false;
}

static bool is_head(char c) {
  /* Input  : A singular character value.
     Output : Returns true if c is part of the snake's head.
  */

  if (c == 'W' || c == 'A' || c == 'S' || c == 'D') {
    return true;
  }
  return false;
}

static bool is_snake(char c) {
  /* Input  : A singular character value.
     Output : Returns true if c is part of the snake.
  */

  if (c=='w' || c=='a' || c=='s' || c=='d' || c=='W' || c=='A' || c=='S' || c=='D' || c=='^' || c=='>' || c=='<' || c=='v' || c=='x') {
    return true;
  }
  return false;
}

static char body_to_tail(char c) {
  /* Input  : The next-to-tail character of the snake.
     Output : The character at tail position on the next cloxk.
  */
 
  if (c == '^') {
    return 'w';
  }
  else if (c == '<') {
    return 'a';
  }
  else if (c == '>') {
    return 's';
  }
  else if (c == 'v') {
    return 'd';
  }
  else {
    return c;
  }
}

static char head_to_body(char c) {
  /* Input  : The head character of the snake.
     Output : The character at head position on the next cloxk.
  */
 
  if (c == 'W') {
    return '^';
  }
  else if (c == 'A') {
    return '<';
  }
  else if (c == 'S') {
    return '>';
  }
  else if (c == 'D') {
    return 'v';
  }
  else {
    return c;
  }
}

static unsigned int get_next_row(unsigned int cur_row, char c) {
  /* Input  : Current row value and the containing character value.
     Output : Returns the next row based on the character's value.
              Returns cur_row+1 if c is 'v' or 's' or 'S'.
              Returns cur_row-1 if c is '^' or 'w' or 'W'.
              Returns cur_row otherwise.
  */

  if (c == 'v' || c == 's' || c == 'S') {
    return cur_row+1;
  }
  else if (c == '^' || c == 'w' || c == 'W') {
    return cur_row-1;
  }
  return cur_row;
}

static unsigned int get_next_col(unsigned int cur_col, char c) {
  /* Input  : Current column value and the containing character value.
     Output : Returns the next column based on the character's value.
              Returns cur_col+1 if c is '>' or 'd' or 'D'.
              Returns cur_col-1 if c is '<' or 'a' or 'A'.
              Returns cur_col otherwise.
  */

  if (c == '>' || c == 'd' || c == 'D') {
    return cur_col+1;
  }
  else if (c == '<' || c == 'a' || c == 'A') {
    return cur_col-1;
  }
  return cur_col;
}

/* Task 4.2 */
static char next_square(game_state_t* state, unsigned int snum) {
  /* Input  : Current game state & index of snake to be analyzed.
     Output : Character in cell, the given snake is moving into.
  */

  snake_t snake = state->snakes[snum];
  unsigned int head_col = snake.head_col;
  unsigned int head_row = snake.head_row;
  char ch = get_board_at(state, head_row, head_col);

  if (ch == 'W') {
    return get_board_at(state, head_row-1, head_col);
  }
  else if (ch == 'A') {
    return get_board_at(state, head_row, head_col-1);
  }
  else if (ch == 'S') {
    return get_board_at(state, head_row+1, head_col);
  }
  else if (ch == 'D') {
    return get_board_at(state, head_row, head_col+1);
  }
  else {
    return ch;
  }
}

/* Task 4.3 */
static void update_head(game_state_t* state, unsigned int snum) {
  /* Input  : Current game state & index of snake to be moved.
     Output : Updates the head to the next cell, based on current position.
  */

  snake_t snake = state->snakes[snum];
  unsigned int head_col = snake.head_col;
  unsigned int head_row = snake.head_row;
  char ch = get_board_at(state, head_row, head_col);

  if (ch == 'W') {
    set_board_at(state, head_row-1, head_col, ch);
    set_board_at(state, head_row, head_col, '^');
    state->snakes[snum].head_row--;
  }
  else if (ch == 'A') {
    set_board_at(state, head_row, head_col-1, ch);
    set_board_at(state, head_row, head_col, '<');
    state->snakes[snum].head_col--;
  }
  else if (ch == 'S') {
    set_board_at(state, head_row+1, head_col, ch);
    set_board_at(state, head_row, head_col, 'v');
    state->snakes[snum].head_row++;
  }
  else if (ch == 'D') {
    set_board_at(state, head_row, head_col+1, ch);
    set_board_at(state, head_row, head_col, '>');
    state->snakes[snum].head_col++;
  }
  return;
}

/* Task 4.5 */
static void update_tail(game_state_t* state, unsigned int snum) {
  /* Input  : Current game state & index of snake to be moved.
     Output : Updates the tail for the next clock cycle.
  */

  snake_t snake = state->snakes[snum];
  unsigned int tail_col = snake.tail_col;
  unsigned int tail_row = snake.tail_row;
  char ch = get_board_at(state, tail_row, tail_col);
  set_board_at(state, tail_row, tail_col, ' ');
  char nxt;

  if (ch == 'w') {
    nxt = get_board_at(state, tail_row-1, tail_col);   

    if (nxt == '^') {
      set_board_at(state, tail_row-1, tail_col, 'w');
    }
    else if (nxt == '<') {
      set_board_at(state, tail_row-1, tail_col, 'a');
    }
    else if (nxt == '>') {
      set_board_at(state, tail_row-1, tail_col, 'd');
    }
    state->snakes[snum].tail_row--; 
  }

  else if (ch == 'a') {
    nxt = get_board_at(state, tail_row, tail_col-1);   

    if (nxt == '^') {
      set_board_at(state, tail_row, tail_col-1, 'w');
    }
    else if (nxt == '<') {
      set_board_at(state, tail_row, tail_col-1, 'a');
    }
    else if (nxt == 'v') {
      set_board_at(state, tail_row, tail_col-1, 's');
    }
    state->snakes[snum].tail_col--; 
  }

  else if (ch == 's') {
    nxt = get_board_at(state, tail_row+1, tail_col);   

    if (nxt == 'v') {
      set_board_at(state, tail_row+1, tail_col, 's');
    }
    else if (nxt == '<') {
      set_board_at(state, tail_row+1, tail_col, 'a');
    }
    else if (nxt == '>') {
      set_board_at(state, tail_row+1, tail_col, 'd');
    }
    state->snakes[snum].tail_row++; 
  }

  else if (ch == 'd') {
    nxt = get_board_at(state, tail_row, tail_col+1); 

    if (nxt == '^') {
      set_board_at(state, tail_row, tail_col+1, 'w');
    }
    else if (nxt == 'v') {
      set_board_at(state, tail_row, tail_col+1, 's');
    }
    else if (nxt == '>') {
      set_board_at(state, tail_row, tail_col+1, 'd');
    }
    state->snakes[snum].tail_col++;   
  }

  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  /* Input  : Current game state & a pointer to a function to add fruit.
     Output : Updates the board for the next clock cycle.
  */

  for (unsigned int i = 0; i < state->num_snakes; i++) {
    snake_t snake = state->snakes[i];
    if (snake.live) {
      char next = next_square(state, i);

      if (next == ' ') {
        update_head(state, i);
        update_tail(state, i);
      }
      else if (next == '*') {
        update_head(state, i);
        add_food(state);
      }
      else {
        set_board_at(state, snake.head_row, snake.head_col, 'x');
        state->snakes[i].live = false;
      }
    }
  }
  
  return;
}

/* Task 5 */
game_state_t* load_board(FILE* fp) {
  /* Input  : File with the game board stored in it.
     Output : Return the game state variables and structures.
  */

  if (fp == NULL) {
    return NULL;
  }

  fpos_t start;
  fgetpos(fp, &start);

  game_state_t* game_state = (game_state_t*) malloc(sizeof(game_state_t));
  game_state->num_snakes = 0;
  game_state->snakes = NULL;

  unsigned int row = 0;
  while(69) {
    char c = (char) fgetc(fp);

    if (feof(fp)) {
      break;
    }
    else if (c == '\n') {
      row++;
    }
  }
  game_state->num_rows = row;

  fsetpos(fp, &start);
  char** board = (char**) malloc(row * sizeof(char*));
  for (int i = 0; i < row; i++) { 
    board[i] = (char*) malloc(100000 * sizeof(char)); 
  }

  char arr[100000];
  int line = 0;
  int index = 0;

  while(69) {
    char c = (char) fgetc(fp);

    if (feof(fp)) {
      break;
    }
    else if (c == '\n') {
      // strcpy(board[line], arr);
      for (int k = 0; k < index; k++) {
        board[line][k] = arr[k];
      }
      line++;
      char arr[100000];
      index = 0;
    }
    else {
      arr[index] = c;
      index++;
    }
  }
  game_state->board = board;
  return game_state;
}

/* Task 6.1 */
static void find_head(game_state_t* state, unsigned int snum) {
  /* Input  : Current incomplete game state & Index of snake to be traced.
     Output : Complete the snake_t structure for the provided snake.
  */
  
  snake_t snk = state->snakes[snum];
  unsigned int r = snk.tail_row;
  unsigned int c = snk.tail_col;
  // printf("tail col :  %d, tail row :  %d\n", col, row);

  while (69) {
    char ch = get_board_at(state, r, c);
    // printf("%c\n", ch);

    if (ch == 'W' || ch == 'A' || ch == 'S' || ch == 'D') {
      // printf("%d %d\n", col, row);
      snk.head_col = c;
      snk.head_row = r;
      snk.live = true;
      break;
    }

    else if (ch == 'x') {
      snk.head_col = c;
      snk.head_row = r;
      snk.live = false;
      break;
    }

    else if (ch == 'w' || ch == '^') {
      r--;
    }

    else if (ch == 'a' || ch == '<') {
      c--;
    }

    else if (ch == 's' || ch == 'v') {
      r++;
    }

    else if (ch == 'd' || ch == '>') {
      c++;
    }
  }
  state->snakes[snum] = snk;
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  /* Input  : Current incomplete game state.
     Output : Initialise snake related fields to the game state.
  */

  char** arr = state->board;
  unsigned int snk_cnt = 0;

  for (unsigned int i = 0; i < state->num_rows; i++) {
    for (unsigned int j = 0; j < strlen(arr[i]); j++) {
      char ch = get_board_at(state, i, j);
      if (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd') {
        snk_cnt++;
      }
    }
  }

  state->num_snakes = snk_cnt;
  snake_t* snakes = (snake_t*) malloc(snk_cnt * sizeof(snake_t));

  unsigned int index = 0;
  for (unsigned int i = 0; i < state->num_rows; i++) {
    for (unsigned int j = 0; j < strlen(arr[i]); j++) {
      char ch = get_board_at(state, i, j);
      if (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd') {
        snake_t* snek_struct = (snake_t*) malloc(sizeof(snake_t));
        snek_struct->tail_col = j;
        snek_struct->tail_row = i;
        // snek_struct->head_col = 0;
        // snek_struct->head_row = 0;
        // snek_struct->live = true;
        snakes[index] = *snek_struct;
        index++;
      }
    }
  }
  state->snakes = snakes;
  
  for (unsigned int i = 0; i < index; i++) {
    find_head(state, i);
    // printf("%d\n", state->snakes[i].head_col);
  }
  
  return state;
}
