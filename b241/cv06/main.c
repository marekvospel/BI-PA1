#ifndef __PROGTEST__
#include <assert.h>
#include <stdio.h>
#endif /* __PROGTEST__ */

typedef struct TDate {
  unsigned m_Year;
  unsigned m_Month;
  unsigned m_Day;
} TDATE;

TDATE makeDate(unsigned y, unsigned m, unsigned d) {
  TDATE res = {y, m, d};
  return res;
}

bool eqDate(TDATE a, TDATE b) {
  return a.m_Year == b.m_Year && a.m_Month == b.m_Month && a.m_Day == b.m_Day;
}

bool gtDate(TDATE a, TDATE b) {
  if (a.m_Year != b.m_Year) {
    return a.m_Year > b.m_Year;
  }

  if (a.m_Month != b.m_Month) {
    return a.m_Month > b.m_Month;
  }

  if (a.m_Day != b.m_Day) {
    return a.m_Day > b.m_Day;
  }

  // Equal
  return 0;
}

/// TDATE a < TDATE b
bool ltDate(TDATE a, TDATE b) { return !eqDate(a, b) && !gtDate(a, b); }

int leapYears(TDATE from, int years) {
  int leapYears = 0;

  // Leap years / 4
  int closest = (4 - from.m_Year % 4) % 4;
  leapYears += (long long)((years - closest) / 4) + (years >= closest);

  // Leap years / 100
  closest = (100 - from.m_Year % 100) % 100;
  leapYears -= (long long)((years - closest) / 100) + (years >= closest);

  // Leap years / 400
  closest = (400 - from.m_Year % 400) % 400;
  leapYears += (long long)((years - closest) / 400) + (years >= closest);

  // Leap years / 4000
  closest = (4000 - from.m_Year % 4000) % 4000;
  leapYears -= (long long)((years - closest) / 4000) + (years >= closest);

  return leapYears;
}

bool isLeapYear(int year) {
  return year % 4000 == 0
             ? 0
             : (year % 400 == 0) || ((year % 100 != 0) && (year % 4 == 0));
}

unsigned monthDays(int month, int year) {
  if (month > 12 || month < 1)
    return 0;
  if (month == 2)
    return 28 + isLeapYear(year);
  const unsigned months[12] = {31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  return months[month - 1];
}
long long daysBetweenDates(TDATE from, TDATE to) {
  long long totalDays = 0;

  if (to.m_Year > from.m_Year) {
    int yearDiff = to.m_Year - from.m_Year;

    totalDays += (long long)yearDiff * 365;

    // Leap years adds including current and ending month. Thus the following
    // conditions
    totalDays += leapYears(from, yearDiff);

    if (isLeapYear(from.m_Year) && gtDate(from, makeDate(from.m_Year, 2, 29))) {
      totalDays -= 1;
    }
    if (isLeapYear(to.m_Year) && ((from.m_Month <= 2 && to.m_Month > 2) ||
                                  (from.m_Month <= 2 && to.m_Month <= 2))) {
      totalDays -= 1;
    }
  }

  if (to.m_Month > from.m_Month) {
    for (unsigned int month = from.m_Month; month < to.m_Month; month++) {
      totalDays += monthDays(month, to.m_Year);
    }
  } else if (to.m_Month != from.m_Month) {
    for (unsigned int month = to.m_Month; month < from.m_Month; month++) {
      totalDays -= monthDays(month, to.m_Year);
    }
  }

  totalDays += (long long)to.m_Day - (long long)from.m_Day;

  return totalDays;
}

int dateToIndex(int day, int month, int year, int *idx) {
  if (year < 2000 || month > 12 || month < 1 || day < 1 ||
      day > (int)monthDays(month, year))
    return 0;

  *idx =
      (int)daysBetweenDates(makeDate(year, 1, 1), makeDate(year, month, day)) +
      1;

  return 1;
}

#ifndef __PROGTEST__
int main(int argc, char *argv[]) {
  int idx;
  assert(dateToIndex(1, 1, 2000, &idx) == 1 && idx == 1);
  assert(dateToIndex(1, 2, 2000, &idx) == 1 && idx == 32);
  assert(dateToIndex(29, 2, 2000, &idx) == 1 && idx == 60);
  assert(dateToIndex(29, 2, 2001, &idx) == 0);
  assert(dateToIndex(1, 12, 2000, &idx) == 1 && idx == 336);
  assert(dateToIndex(31, 12, 2000, &idx) == 1 && idx == 366);
  assert(dateToIndex(1, 1, 1999, &idx) == 0);
  assert(dateToIndex(6, 7, 3600, &idx) == 1 && idx == 188);
  assert(dateToIndex(29, 2, 3600, &idx) == 1 && idx == 60);
  assert(dateToIndex(29, 2, 4000, &idx) == 0);
  return 0;
}
#endif /* __PROGTEST__ */
