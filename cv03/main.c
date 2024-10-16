#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int h, m, s, ms;
} Time;

int parse_time(Time *time) {
  char msc[] = {0, 0, 0};
  int res = scanf(" %d : %d : %d , %c", &time->h, &time->m, &time->s, &msc[0]);

  if (res < 4 || msc[0] < '0' || msc[0] > '9') {
    return -1;
  }

  msc[1] = getchar();
  if (isspace(msc[1]) || (msc[1] < '0' && msc[1] > '9')) {
    time->ms = (msc[0] - '0') * 100;
    return 0;
  }

  msc[2] = getchar();
  if (isspace(msc[2]) || (msc[2] < '0' && msc[2] > '9')) {
    time->ms = (msc[0] - '0') * 100 + (msc[1] - '0') * 10;
    return 0;
  }

  time->ms = (msc[0] - '0') * 100 + (msc[1] - '0') * 10 + (msc[2] - '0');
  msc[0] = getchar();

  if (!isspace(msc[0]) || time->h < 0 || time->m < 0 || time->s < 0) {
    return -1;
  }

  return 0;
}

bool is_valid_time(Time *time) {

  if (time->h >= 24 || time->m >= 60 || time->s >= 60 || time->ms >= 1000) {
    return false;
  }

  return true;
}

int time_diff(Time *timeDiff, Time timeStart, Time timeEnd) {
  timeDiff->h = timeEnd.h - timeStart.h;
  timeDiff->m = timeEnd.m - timeStart.m;
  timeDiff->s = timeEnd.s - timeStart.s;
  timeDiff->ms = timeEnd.ms - timeStart.ms;

  if (timeDiff->ms < 0) {
    timeDiff->s -= 1;
    timeDiff->ms += 1000;
  }

  if (timeDiff->s < 0) {
    timeDiff->m -= 1;
    timeDiff->s += 60;
  }

  if (timeDiff->m < 0) {
    timeDiff->h -= 1;
    timeDiff->m += 60;
  }

  if (timeDiff->h < 0 || timeDiff->m < 0 || timeDiff->s < 0 ||
      timeDiff->ms < 0) {
    return 1;
  } else if (timeDiff->h == 0 && timeDiff->m == 0 && timeDiff->s == 0 &&
             timeDiff->ms == 0) {

    return 0;
  } else {
    return -1;
  }
}

int main() {
  Time startTime;
  Time endTime;

  printf("Zadejte cas t1:\n");
  if (parse_time(&startTime) != 0 || !is_valid_time(&startTime)) {
    printf("Nespravny vstup.\n");
    return -1;
  }

  printf("Zadejte cas t2:\n");
  if (parse_time(&endTime) != 0 || !is_valid_time(&endTime)) {
    printf("Nespravny vstup.\n");
    return -1;
  }

  Time timeDiff;

  if (time_diff(&timeDiff, startTime, endTime) > 0) {
    printf("Nespravny vstup.\n");
    return -1;
  }

  printf("Doba: %2d:%02d:%02d,%03d\n", timeDiff.h, timeDiff.m, timeDiff.s,
         timeDiff.ms);

  return 0;
}
