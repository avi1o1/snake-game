#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"

int main(int argc, char* argv[]) {
  bool io_stdin = false;
  char* in_filename = NULL;
  char* out_filename = NULL;
  game_state_t* state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      if (io_stdin) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      in_filename = argv[i + 1];
      i++;
      continue;
    } else if (strcmp(argv[i], "--stdin") == 0) {
      if (in_filename != NULL) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      io_stdin = true;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
    return 1;
  }

  /* Task 7 */
  game_state_t* game;
  if (in_filename != NULL) {
    // Load the board from in_filename. Return -1, if the file doesn't exist.
    FILE* fPtr = fopen(in_filename, "r");
    if (fPtr == NULL) {
      return -1;
    }

    game = load_board(fPtr);
    game = initialize_snakes(game);
    fclose(fPtr);
    
  } else if (io_stdin) {
    // Load the board from stdin
    game = load_board(stdin);
  
  } else {
    // Create default state
    game = create_default_state();
  }

  // Update state.
  update_state(game, deterministic_food);

  // Write updated board to file or stdout
  if (out_filename != NULL) {
    // Save the board to out_filename
    FILE* Ptr = fopen(out_filename, "w");
    print_board(game, Ptr);
    fclose(Ptr);
  
  } else {
    // Print the board to stdout
    print_board(state, stdout);
  }

  // Free the state
  free_state(state);

  return 0;
}
