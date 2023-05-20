#include "ksl/token.h"
#include "ksl/utils.h"
#include "ksl/vector.h"

int main(void) {
  Vector *tokens = tokenizer("f[x: u8] := Add[x, 10]");
  if (tokens == NULL) {
    log_warn("tokenizer failed");
  } else {
    show_vector(tokens);
    drop_vector(tokens);
  }
  return 0;
}