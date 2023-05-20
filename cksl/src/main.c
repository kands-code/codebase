#include "ksl/token.h"
#include "ksl/utils.h"
#include "ksl/vector.h"

int main(void) {
  Vector *tokens =
      tokenizer("x := 12.3; y := Any[12, \"Hello World\"]; z := #Pi;");
  if (tokens == NULL) {
    log_warn("tokenizer failed");
  } else {
    show_vector(tokens);
    drop_vector(tokens);
  }
  return 0;
}