#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main() {
  int option = 0;
  int res = 0;

  printf("ml' nob:\n");

  if ((res = scanf("%d", &option)) != 1) {
    printf("Neh mi'\n");
    return EINVAL;
  }

  char c = getchar();
  if (c != EOF && !isspace(c)) {
    printf("bIjatlh 'e' yImev\n");
    return EINVAL;
  }

  if (option < 0 || option > 8) {
    printf("Qih mi' %d\n", option);
    return EINVAL;
  } else {
    printf("Qapla'\n");
  }

  switch (option) {
  case (0):
    printf("noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH "
           "may' DoHlu'chugh lujbe'lu'.\n");
    break;
  case (1):
    printf("bortaS bIr jablu'DI' reH QaQqu' nay'.\n");
    break;
  case (2):
    printf("Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, "
           "pagh ghaH SuvwI''e'.\n");
    break;
  case (3):
    printf("bISeH'eghlaH'be'chugh latlh Dara'laH'be'.\n");
    break;
  case (4):
    printf("qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n");
    break;
  case (5):
    printf("Suvlu'taHvIS yapbe' HoS neH.\n");
    break;
  case (6):
    printf("Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n");
    break;
  case (7):
    printf("Heghlu'meH QaQ jajvam.\n");
    break;
  case (8):
    printf("leghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
    break;
  }

  return 0;
}
