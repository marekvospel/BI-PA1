#include <stdbool.h>
#include <stdio.h>

void print_firstline(int amount, int size) {
  putchar('+');
  for (int i = 0; i < (amount * size); i++) {
    putchar('-');
  }
  puts("+");
}

void print_lines(int amount, int size) {
  bool start_with_full = false;
  for (int i = 0; i < amount; i++) {

    for (int j = 0; j < size; j++) {
      bool square_full = start_with_full;
      putchar('|');
      for (int k = 0; k < amount; k++) {
        for (int l = 0; l < size; l++) {
          if (square_full) {
            putchar('X');
          } else {
            putchar(' ');
          }
        }
        square_full = !square_full;
      }
      puts("|");
    }

    start_with_full = !start_with_full;
  }
}

int main() {
  int amount, size;

  puts("Zadejte pocet poli:");

  if (scanf("%d", &amount) != 1 || amount < 1) {
    puts("Nespravny vstup.");
    return -1;
  }

  puts("Zadejte velikost pole:");

  if (scanf("%d", &size) != 1 || size < 1) {
    puts("Nespravny vstup.");
    return -1;
  }
  print_firstline(amount, size);
  print_lines(amount, size);
  print_firstline(amount, size);

  return 0;
}
