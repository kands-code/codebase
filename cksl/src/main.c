#include "ksl/parser.h"
#include "ksl/token.h"
#include "ksl/utils.h"
#include "ksl/vector.h"
#include <stddef.h>

int main(void) {
  // vector_Token *tokens =
  //     tokenizer("TypeBind[Point, { x : Float, y : Float }];\n"
  //               "p: Point := { x = 2.1, y = 4.2 }; (* get point *)\n"
  //               "LengthOfPoint[p: Point]: Float :="
  //               "Block[{ip: Point := { x = 1.0, y = 1.0 }, diff: Float =
  //               0.2}," "Sqrt[(p -> x - ip->x)^2 + (p ->y - ip->y) ^2] +
  //               diff];" "Print[LengthOfPoint[{x = 1, y = 1}]]; (* => 0.2
  //               *)");
  // vector_Token *tokens = tokenizer("p: Point := { x = 2.0, y = 1.0 }");
  vector_Token *tokens = tokenizer("s: String := \"12.3\"; x: Integer := 13;");
  if (tokens == NULL) {
    log_warn("tokenizer failed");
  } else {
    vector_Expression *exprs = parser(tokens);
    if (exprs != NULL) {
      show_vector_Expression(exprs, 0);
      drop_vector_Expression(exprs);
    }
    drop_vector_Token(tokens);
  }
  return 0;
}
