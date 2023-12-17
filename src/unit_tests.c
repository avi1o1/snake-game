#include "unit_tests.h"

int main(int argc, char* argv[]) {
  bool MEMCHECK_MODE = false;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-m") == 0) {
      MEMCHECK_MODE = true;
      continue;
    }
    fprintf(stderr, "Usage: %s [-m]\n", argv[0]);
    return 1;
  }

  init_colors();

  printf("%s\n", "Reminder: These tests are not comprehensive, and passing them does not guarantee that your implementation is working.");

  if (MEMCHECK_MODE) {
    printf("\nTesting free_state...\n");
    if (!test_free_state()) {
      exit(1);
    }
  } else {
    if (!test_and_print("create_default_state (Task 1)", test_create_default_state)) {
      exit(1);
    }
    if (!test_and_print("print_board (Task 3)", test_print_board)) {
      exit(1);
    }
    if (!test_and_print("next_square (Task 4)", test_next_square)) {
      exit(1);
    }
    if (!test_and_print("update_head (Task 4)", test_update_head)) {
      exit(1);
    }
    if (!test_and_print("update_tail (Task 4)", test_update_tail)) {
      exit(1);
    }
    if (!test_and_print("update_state (Task 4)", test_update_state)) {
      exit(1);
    }
    if (!test_and_print("load_board (Task 5)", test_load_board)) {
      exit(1);
    }
    if (!test_and_print("find_head (Task 6)", test_find_head)) {
      exit(1);
    }
    if (!test_and_print("initialize_snakes (Task 6)", test_initialize_snakes)) {
      exit(1);
    }
  }
}
