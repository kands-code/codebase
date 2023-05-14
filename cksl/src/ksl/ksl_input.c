/**
 * @file ksl/ksl_input.c
 * @brief content input of KSL
 */

// includes

// self include
#include "ksl/ksl.h"
#include "ksl/utils.h"
// standard include
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// readline include
#include <readline/history.h>
#include <readline/readline.h>

// constant

static size_t COMMAND_COMPARE_LENGTH = 2;

// function: input

void ksl_repl(const char *prompt) {
  // set default prompt
  if (prompt == NULL) {
    prompt = "> ";
  }
  char *get_input = readline(prompt);
  while (strncmp(get_input, ":q", COMMAND_COMPARE_LENGTH)) {
    // do command
    if (get_input[0] == ':') {
      switch (get_input[1]) {
      case 'h':
        ksl_repl_help();
        break;
      default:
        log_error("panic: wrong command [%s]", get_input + 1);
      }
      // get new input
      free(get_input);
      get_input = readline(prompt);
      continue;
    }
    // start multi-line
    if (!strncmp(get_input, "#{", COMMAND_COMPARE_LENGTH)) {
      size_t input_count = 0;
      char *inner_input = readline("~ ");
      while (strncmp(inner_input, "}#", COMMAND_COMPARE_LENGTH)) {
        // append inner input to input
        get_input = realloc(get_input, input_count + strlen(inner_input) + 1);
        strncpy(get_input + input_count, inner_input, strlen(inner_input));
        input_count = strlen(get_input);
        // get new line
        free(inner_input);
        inner_input = readline("~ ");
      }
      // end multi-line
      free(inner_input);
    }
    // add content into history
    add_history_time(get_input);

    // process input
    const char *result = ksl_interpreter(get_input);
    if (result != NULL) {
      printf("=> %s\n", result);
    }

    // get new input
    free(get_input);
    get_input = readline(prompt);
  }
  free(get_input);
}

void ksl_repl_help() {
  puts("");
  puts("COMMAND:");
  puts("  :q, :quit  => quit the REPL");
  puts("  :h, :help  => show REPL help infomation");
  puts("use `#{` and `}#` to enable multi-line input");
  puts("");
}
