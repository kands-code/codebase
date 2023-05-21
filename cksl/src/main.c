#include "ksl/token.h"
#include "ksl/utils.h"
#include "ksl/vector.h"

int main(void) {
  Vector *tokens =
      tokenizer("TypeBind[Point, { x : Float, y : Float }];\n"
                "p: Point := { x = 2.1, y = 4.2 }; (* get point *)\n"
                "LengthOfPoint[p: Point]: Float :="
                "Block[{ip: Point := { x = 1.0, y = 1.0 }, diff: Float = 0.2},"
                "Sqrt[(p -> x - ip->x)^2 + (p ->y - ip->y) ^2] + diff];"
                "Print[LengthOfPoint[{x = 1, y = 1}]]; (* => 0.2 *)");
  if (tokens == NULL) {
    log_warn("tokenizer failed");
  } else {
    show_vector(tokens);
    drop_vector(tokens);
  }
  return 0;
}