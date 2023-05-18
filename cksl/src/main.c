#include "ksl/token.h"
#include "ksl/utils.h"
#include "ksl/vector.h"

int main(void) {
  Vector *tokens =
      tokenizer("Add[1, 2, 3]; x := Tim[3, 4]; y := \"Hello World\"");
  show_vector(tokens);
  drop_vector(tokens);
  return 0;
}