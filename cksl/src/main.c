#include "ksl/parser.h"
#include "ksl/token.h"
#include "ksl/utils.h"
#include "ksl/vector.h"
#include <stddef.h>

int main(void) {
  vector_Token *tokens =
      tokenizer("TypeBind[Point, { x : Float, y : Float }];\n"
                "p: Point := { x = 2.1, y = 4.2 }; (* get point *)\n"
                "LengthOfPoint[p: Point]: Float :="
                "Block[{ip: Point := { x = 1.0, y = 1.0 }, diff: Float = 0.2},"
                "Sqrt[(p -> x - ip->x)^2 + (p ->y - ip->y) ^2] + diff];"
                "Print[LengthOfPoint[{x = 1, y = 1}]]; (* => 0.2 *)");
  if (tokens == NULL) {
    log_warn("tokenizer failed");
  } else {
    size_t expr_cnt = 0;
    vector_Token **sp_toks = split_Tokens(tokens, &expr_cnt);
    for (size_t i = 0; i < expr_cnt; ++i) {
      log_info("=== sp %zu ===", i);
      show_vector_Token(sp_toks[i], 0);
    }
    drop_vector_Token(tokens);
  }
  return 0;
}
