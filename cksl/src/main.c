#include "ksl/token.h"
#include "ksl/utils.h"
#include "ksl/vector.h"

int main(void) {
  Vector *tokens = tokenizer("x := 12.3; y := Any[12, \"Hello World\"]");
  show_vector(tokens);
  drop_vector(tokens);
  return 0;
}