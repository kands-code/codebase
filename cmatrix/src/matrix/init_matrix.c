#include "matrix/matrix.h"
#include <stdint.h>

static long cachedFib(uint8_t number, long cache1, long cache2) {
  if (number == 0) {
    return cache1;
  } else if (number == 1) {
    return cache2;
  }
  return cachedFib(number - 1, cache2, cache1 + cache2);
}

long fib(uint8_t number) { return cachedFib(number, 0, 1); }