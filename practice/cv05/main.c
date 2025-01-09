#include <math.h>
#include <stdio.h>

int load_input(double *x, double *y, int *op) {

  if (scanf(" %lg", x) != 1) {
    return -1;
  }

  int scanned = -1;
  if (scanf(" +%n", &scanned) == 0 && scanned != -1) {
    *op = 0;
  } else if (scanf(" -%n", &scanned) == 0 && scanned != -1) {
    *op = 1;
  } else if (scanf(" *%n", &scanned) == 0 && scanned != -1) {
    *op = 2;
  } else if (scanf(" /%n", &scanned) == 0 && scanned != -1) {
    *op = 3;
  } else {
    return -1;
  }

  scanned = -1;
  if (scanf(" %lg =%n", y, &scanned) != 1 || scanned == -1) {
    return -1;
  }

  if (*op == 3 && *y == 0)
    return -1;

  return 0;
}

int main() {
  double x, y;
  int op;

  printf("Zadejte vzorec:\n");
  if (load_input(&x, &y, &op) != 0) {
    printf("Nespravny vstup.\n");
    return -1;
  }

  double result = 0;

  switch (op) {
  case 0:
    result = x + y;
    break;
  case 1:
    result = x - y;
    break;
  case 2:
    result = x * y;
    break;
  case 3:
    result = x / y;
    if (result > 0)
      result = floor(result);
    else
      result = ceil(result);
    break;
  }

  printf("%lg\n", result);

  return 0;
}
