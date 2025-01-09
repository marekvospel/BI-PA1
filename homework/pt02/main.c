#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int lo;
  int hi;
} NumRange;

int parse_input(NumRange *range, bool *show) {
  int end = -1;
  char command;

  int conversions =
      scanf(" %c < %d ; %d > %n", &command, &range->lo, &range->hi, &end);

  // No conversions, EOF
  if (end == -1 && conversions <= 0) {
    return -1;
  }

  // incomplete command || invalid command type || invalid range
  if (end == -1 || (command != '?' && command != '#') ||
      range->lo > range->hi || range->lo <= 0 || range->hi <= 0) {
    return 1;
  }
  *show = command == '?';

  return 0;
}

int gcd(int x, int y) {
  while (y != 0) {
    int tmp = x % y;
    x = y;
    y = tmp;
  }
  return x;
}

int max(int a, int b) {
  if (a > b)
    return a;
  return b;
}

int solutions(NumRange range, bool print) {
  int solutions = 0;

  for (int m = 2; m * m < range.hi; m++) {
    for (int n = m - 1; n > 0; n--) {
      if ((m - n) % 2 != 1 || gcd(m, n) != 1)
        continue;
      int a = 2 * m * n;
      int b = m * m - n * n;
      int c = m * m + n * n;

      if (b < a) {
        int tmp = a;
        a = b;
        b = tmp;
      }

      for (int k = 1; k * c <= range.hi; k++) {
        int ak = k * a;
        int bk = k * b;
        int ck = k * c;

        if (ak < range.lo || bk < range.lo || ck > range.hi)
          continue;
        if (print)
          printf("* %d %d %d\n", ak, bk, ck);
        solutions += 1;
      }
    }
  }

  return solutions;
}

int main() {
  printf("Problemy:\n");

  for (;;) {
    NumRange range;
    bool commandType;

    int result = parse_input(&range, &commandType);

    if (result < 0) {
      return 0;
    } else if (result > 0) {
      printf("Nespravny vstup.\n");
      return -1;
    }

    printf("Celkem: %d\n", solutions(range, commandType));
  }
}
