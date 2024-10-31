#include "progtest.h"
#include <assert.h>
#include <stdbool.h>

bool isLeapYear(int year);
long long daysBetweenDates(TDATE from, TDATE to);
int dayOfWeek(TDATE date);
TDATE addDaysToDate(TDATE date, long long days);
TDATE addDayToDate(TDATE date);

typedef struct TWeekConnections {
  long long connections[7];
} TWeekConnections;

void extra_asserts() {
  TDATE d;

  // Leap year stuff
  assert(isLeapYear(2000));
  assert(isLeapYear(2004));
  assert(!isLeapYear(2100));
  assert(isLeapYear(2400));
  assert(!isLeapYear(4000));
  assert(!isLeapYear(2002));

  // Days between dates
  assert(daysBetweenDates(makeDate(2004, 1, 1), makeDate(2005, 1, 1)) == 366);
  assert(daysBetweenDates(makeDate(2001, 1, 1), makeDate(2002, 1, 1)) == 365);
  assert(daysBetweenDates(makeDate(2001, 1, 1), makeDate(2004, 1, 1)) ==
         3 * 365);
  assert(daysBetweenDates(makeDate(2001, 5, 1), makeDate(2004, 5, 1)) ==
         2 * 365 + 366);
  assert(daysBetweenDates(makeDate(2001, 1, 1), makeDate(2004, 1, 2)) ==
         3 * 365 + 1);
  assert(daysBetweenDates(makeDate(2001, 1, 1), makeDate(2005, 1, 1)) ==
         3 * 365 + 366);
  assert(daysBetweenDates(makeDate(2099, 1, 1), makeDate(2101, 1, 1)) ==
         2 * 365);
  assert(daysBetweenDates(makeDate(2000, 1, 1), makeDate(2001, 1, 1)) == 366);
  assert(daysBetweenDates(makeDate(4000, 1, 1), makeDate(4001, 1, 1)) == 365);
  assert(daysBetweenDates(makeDate(4000, 1, 1), makeDate(4001, 2, 1)) ==
         365 + 31);

  assert(daysBetweenDates(makeDate(2000, 1, 1), makeDate(2000, 1, 5)) == 4);
  assert(daysBetweenDates(makeDate(2000, 1, 1), makeDate(2000, 1, 31)) == 30);
  assert(daysBetweenDates(makeDate(2000, 1, 1), makeDate(2000, 2, 3)) ==
         2 + 31);
  assert(daysBetweenDates(makeDate(2000, 2, 1), makeDate(2000, 3, 1)) == 29);
  assert(daysBetweenDates(makeDate(2001, 2, 1), makeDate(2001, 3, 1)) == 28);
  assert(daysBetweenDates(makeDate(2001, 1, 1), makeDate(2001, 4, 1)) ==
         31 + 28 + 31);
  assert(daysBetweenDates(makeDate(2001, 2, 1), makeDate(2001, 6, 1)) ==
         28 + 31 + 30 + 31);
  assert(daysBetweenDates(makeDate(2001, 5, 1), makeDate(2001, 8, 1)) ==
         31 + 30 + 31);
  assert(daysBetweenDates(makeDate(2001, 5, 1), makeDate(2001, 10, 1)) ==
         31 + 30 + 31 + 31 + 30);
  assert(daysBetweenDates(makeDate(2001, 5, 1), makeDate(2001, 12, 5)) ==
         31 + 30 + 31 + 31 + 30 + 31 + 30 + 4);
  assert(daysBetweenDates(makeDate(2001, 8, 1), makeDate(2001, 10, 8)) ==
         31 + 30 + 7);
  assert(daysBetweenDates(makeDate(2000, 10, 1), makeDate(2000, 12, 1)) ==
         31 + 30);
  assert(daysBetweenDates(makeDate(2000, 6, 1), makeDate(2000, 11, 1)) ==
         30 + 31 + 31 + 30 + 31);

  assert(daysBetweenDates(makeDate(2002, 1, 1), makeDate(2002, 11, 1)) ==
         31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31);
  assert(daysBetweenDates(makeDate(2001, 12, 31), makeDate(2002, 11, 8)) ==
         1 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 7);

  assert(daysBetweenDates(makeDate(2001, 12, 31), makeDate(2002, 10, 8)) ==
         1 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 7);
  assert(daysBetweenDates(makeDate(2001, 12, 31), makeDate(2002, 6, 8)) ==
         1 + 31 + 28 + 31 + 30 + 31 + 7);
  assert(daysBetweenDates(makeDate(2001, 10, 1), makeDate(2002, 9, 8)) ==
         31 + 30 + 31 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 7);
  assert(daysBetweenDates(makeDate(2001, 4, 1), makeDate(2002, 3, 1)) ==
         30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31 + 31 + 28);
  assert(daysBetweenDates(makeDate(2001, 3, 1), makeDate(2002, 2, 1)) == 337);

  assert(daysBetweenDates(makeDate(1999, 12, 31), makeDate(2000, 1, 1)) == 1);

  assert(daysBetweenDates(makeDate(2000, 2, 29), makeDate(2004, 2, 29)) ==
         1461);

  // Days in week
  assert(dayOfWeek(makeDate(1999, 12, 27)) == 0);
  assert(dayOfWeek(makeDate(1999, 12, 28)) == 1);
  assert(dayOfWeek(makeDate(1999, 12, 31)) == 4);
  assert(dayOfWeek(makeDate(2000, 1, 1)) == 5);
  assert(dayOfWeek(makeDate(2000, 1, 2)) == 6);
  assert(dayOfWeek(makeDate(2989, 7, 23)) == 3);
  assert(dayOfWeek(makeDate(3999, 12, 31)) == 4);
  assert(dayOfWeek(makeDate(4000, 1, 1)) == 5);
  assert(dayOfWeek(makeDate(4000, 3, 1)) == 1);

  assert(countConnections(makeDate(2024, 1, 32), makeDate(2024, 2, 32), 1,
                          DOW_ALL) == -1);
  assert(countConnections(makeDate(2024, 1, 0), makeDate(2024, 1, 3), 1,
                          DOW_ALL) == -1);
  assert(countConnections(makeDate(2024, 0, 15), makeDate(2024, 1, 3), 1,
                          DOW_ALL) == -1);

  assert(countConnections(makeDate(2024, 10, 28), makeDate(2024, 11, 3), 6,
                          DOW_SUN) == 2);
  assert(countConnections(makeDate(2024, 10, 28), makeDate(2024, 10, 29), 6,
                          DOW_TUE) == 6);
  assert(countConnections(makeDate(2024, 10, 28), makeDate(2024, 10, 28), 6,
                          DOW_MON) == 6);

  assert(countConnections(makeDate(2024, 10, 28), makeDate(2024, 11, 10), 6,
                          DOW_SUN) == 4);

  // @Maroš
  assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 1), 1,
                          DOW_ALL) == 1);
  assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 2), 1,
                          DOW_ALL) == 2);
  assert(countConnections(makeDate(2025, 10, 1), makeDate(2024, 10, 2), 1,
                          DOW_ALL) == -1);

  // Vojta
  assert(countConnections(makeDate(2000, 10, 1), makeDate(2001, 10, 1), 1,
                          DOW_ALL) == 366);
  assert(countConnections(makeDate(12429, 8, 20), makeDate(49444, 4, 15), 25,
                          DOW_MON | DOW_TUE | DOW_WED | DOW_THU | DOW_SUN) ==
         210515054);

  // progtest :)
  assert(countConnections(makeDate(2004, 11, 27), makeDate(2004, 12, 7), 11,
                          DOW_MON | DOW_TUE | DOW_WED | DOW_THU) == 66);
  assert(countConnections(makeDate(2004, 11, 27), makeDate(2011, 4, 12), 11,
                          DOW_MON | DOW_TUE | DOW_WED | DOW_THU) == 14630);

  assert(daysBetweenDates(makeDate(2004, 1, 27), makeDate(2012, 8, 26)) ==
         3134);

  assert(countConnections(makeDate(2004, 1, 27), makeDate(2012, 8, 26), 34,
                          DOW_MON | DOW_TUE | DOW_THU | DOW_SAT) == 53278);
  assert(daysBetweenDates(makeDate(2004, 1, 27), makeDate(2012, 8, 26)) ==
         3134);

  assert(daysBetweenDates(makeDate(2004, 1, 27), makeDate(2005, 1, 27)) == 366);
  assert(daysBetweenDates(makeDate(2004, 4, 27), makeDate(2005, 4, 27)) == 365);
  assert(daysBetweenDates(makeDate(2003, 1, 27), makeDate(2004, 1, 27)) == 365);
  assert(daysBetweenDates(makeDate(2003, 1, 27), makeDate(2004, 4, 27)) == 456);
  assert(daysBetweenDates(makeDate(2004, 1, 27), makeDate(2008, 1, 27)) ==
         365 * 3 + 366);
  assert(daysBetweenDates(makeDate(2004, 5, 27), makeDate(2008, 5, 27)) ==
         365 * 3 + 366);
  assert(daysBetweenDates(makeDate(2004, 5, 27), makeDate(2008, 5, 27)) ==
         365 * 3 + 366);

  assert(daysBetweenDates(makeDate(2004, 1, 27), makeDate(2008, 5, 27)) ==
         1582);
  assert(daysBetweenDates(makeDate(2004, 5, 27), makeDate(2008, 1, 27)) ==
         1340);

  assert(daysBetweenDates(makeDate(2020, 2, 28), makeDate(2020, 3, 1)) == 2);

  assert(daysBetweenDates(makeDate(2004, 1, 27), makeDate(2005, 2, 3)) ==
         366 + 4 + 3);
  assert(daysBetweenDates(makeDate(2004, 4, 27), makeDate(2005, 3, 27)) ==
         365 - 31);
  assert(daysBetweenDates(makeDate(2003, 1, 27), makeDate(2004, 1, 29)) ==
         365 + 2);
  assert(daysBetweenDates(makeDate(2003, 1, 27), makeDate(2004, 4, 22)) ==
         456 - 5);
  assert(daysBetweenDates(makeDate(2004, 1, 27), makeDate(2008, 1, 27)) ==
         365 * 3 + 366);
  assert(daysBetweenDates(makeDate(2004, 5, 27), makeDate(2008, 5, 27)) ==
         365 * 3 + 366);
  assert(daysBetweenDates(makeDate(2004, 5, 27), makeDate(2008, 5, 27)) ==
         365 * 3 + 366);

  d = addDaysToDate(makeDate(3950, 1, 1), 730);
  assert(d.m_Year == 3952 && d.m_Month == 1 && d.m_Day == 1);

  TDATE start = makeDate(3950, 1, 1);
  for (int i = 0; i < 100; i++) {
    TDATE end = start;
    for (int diff = 0; diff < 366 * 50; diff++) {
      printf("TDATE {%d %d %d} - TDATE {%d %d %d} (%d)\n", start.m_Year,
             start.m_Month, start.m_Day, end.m_Year, end.m_Month, end.m_Day,
             diff);
      TDATE d = addDaysToDate(start, diff);
      assert(d.m_Year == end.m_Year && d.m_Month == end.m_Month &&
             d.m_Day == end.m_Day);
      assert(daysBetweenDates(start, end) == diff);

      end = addDayToDate(end);
    }

    start = addDayToDate(start);
  }

  // Alpa
  d = endDate(makeDate(4001, 1, 18), 20, 1, DOW_MON);
  assert(d.m_Year == 4001 && d.m_Month == 6 && d.m_Day == 11);
  d = endDate(makeDate(4001, 1, 3), 20, 1, DOW_MON);
  assert(d.m_Year == 4001 && d.m_Month == 5 && d.m_Day == 28);
  d = endDate(makeDate(4001, 1, 25), 20, 1, DOW_FRI);
  assert(d.m_Year == 4001 && d.m_Month == 6 && d.m_Day == 15);
  d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_THU);
  assert(d.m_Year == 2026 && d.m_Month == 9 && d.m_Day == 2);
  d = endDate(makeDate(4001, 1, 18), 20, 1, DOW_MON);
  assert(d.m_Year == 4001 && d.m_Month == 6 && d.m_Day == 11);

  // Time to do add date to date :))
  d = addDaysToDate(makeDate(2001, 3, 9), 5);
  assert(d.m_Year == 2001 && d.m_Month == 3 && d.m_Day == 14);
  d = addDaysToDate(makeDate(2001, 3, 9), 22);
  assert(d.m_Year == 2001 && d.m_Month == 3 && d.m_Day == 31);
  d = addDaysToDate(makeDate(2001, 3, 9), 23);
  assert(d.m_Year == 2001 && d.m_Month == 4 && d.m_Day == 1);
  d = addDaysToDate(makeDate(2000, 3, 9), 5);
  assert(d.m_Year == 2000 && d.m_Month == 3 && d.m_Day == 14);
  d = addDaysToDate(makeDate(2000, 2, 29), 366);
  assert(d.m_Year == 2001 && d.m_Month == 3 && d.m_Day == 1);
  d = addDaysToDate(makeDate(2000, 2, 29), 365);
  assert(d.m_Year == 2001 && d.m_Month == 2 && d.m_Day == 28);
  d = addDaysToDate(makeDate(2000, 2, 28), 365);
  assert(d.m_Year == 2001 && d.m_Month == 2 && d.m_Day == 27);
  d = addDaysToDate(makeDate(2000, 2, 28), 366);
  assert(d.m_Year == 2001 && d.m_Month == 2 && d.m_Day == 28);

  d = addDaysToDate(makeDate(2000, 1, 1), 1204);
  assert(d.m_Year == 2003 && d.m_Month == 4 && d.m_Day == 19);
  d = addDaysToDate(makeDate(2000, 1, 1), 1204);
  assert(d.m_Year == 2003 && d.m_Month == 4 && d.m_Day == 19);
  d = addDaysToDate(makeDate(2000, 9, 3), 26312);
  assert(d.m_Year == 2072 && d.m_Month == 9 && d.m_Day == 17);

  d = addDaysToDate(makeDate(2000, 2, 29), 3 * 365 + 366);
  assert(d.m_Year == 2004 && d.m_Month == 2 && d.m_Day == 29);

  d = addDaysToDate(makeDate(1999, 12, 29), 30);
  assert(d.m_Year == 2000 && d.m_Month == 1 && d.m_Day == 28);
  d = addDaysToDate(makeDate(1998, 12, 29), 30 + 365);
  assert(d.m_Year == 2000 && d.m_Month == 1 && d.m_Day == 28);

  d = addDaysToDate(makeDate(2004, 1, 1), 366);
  assert(d.m_Year == 2005 && d.m_Month == 1 && d.m_Day == 1);
  d = addDaysToDate(makeDate(2004, 1, 1), 365);
  assert(d.m_Year == 2004 && d.m_Month == 12 && d.m_Day == 31);

  d = endDate(makeDate(2024, 13, 1), 100, 1, DOW_THU);
  assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
  d = endDate(makeDate(2024, 2, 30), 100, 1, DOW_THU);
  assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);

  // @Maroš
  d = endDate(makeDate(2024, 10, 1), 1, 100, DOW_ALL);
  assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
  d = endDate(makeDate(2024, 10, 1), 1, 1, DOW_ALL);
  assert(d.m_Year == 2024 && d.m_Month == 10 && d.m_Day == 1);
  d = endDate(makeDate(2024, 10, 1), 2, 1, DOW_ALL);
  assert(d.m_Year == 2024 && d.m_Month == 10 && d.m_Day == 2);
  d = endDate(makeDate(2024, 10, 1), 92, 1, DOW_ALL);
  assert(d.m_Year == 2024 && d.m_Month == 12 && d.m_Day == 31);
  d = endDate(makeDate(2024, 10, 1), 93, 1, DOW_ALL);
  assert(d.m_Year == 2025 && d.m_Month == 1 && d.m_Day == 1);
  d = endDate(makeDate(2003, 8, 25), 4397, 26, DOW_SUN);
  assert(d.m_Year == 2013 && d.m_Month == 1 && d.m_Day == 5);
  d = endDate(makeDate(2008, 7, 24), 2557, 17, DOW_SUN);
  assert(d.m_Year == 2016 && d.m_Month == 9 && d.m_Day == 24);

  d = addDaysToDate(makeDate(2000, 1, 1), 1825);
  assert(d.m_Year == 2004 && d.m_Month == 12 && d.m_Day == 30);
}

int main() {
  TDATE d;
  extra_asserts();
  assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1,
                          DOW_ALL) == 31);
  assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 10,
                          DOW_ALL) == 266);
  assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1,
                          DOW_WED) == 5);
  assert(countConnections(makeDate(2024, 10, 2), makeDate(2024, 10, 30), 1,
                          DOW_WED) == 5);
  assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10,
                          DOW_TUE) == 10);
  assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10,
                          DOW_WED) == 0);
  assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 5,
                          DOW_MON | DOW_FRI | DOW_SAT) == 7462);
  assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 0,
                          DOW_MON | DOW_FRI | DOW_SAT) == 0);
  assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 100,
                          0) == 0);
  assert(countConnections(makeDate(2024, 10, 10), makeDate(2024, 10, 9), 1,
                          DOW_MON) == -1);
  assert(countConnections(makeDate(2024, 2, 29), makeDate(2024, 2, 29), 1,
                          DOW_ALL) == 1);
  assert(countConnections(makeDate(2023, 2, 29), makeDate(2023, 2, 29), 1,
                          DOW_ALL) == -1);
  assert(countConnections(makeDate(2100, 2, 29), makeDate(2100, 2, 29), 1,
                          DOW_ALL) == -1);
  assert(countConnections(makeDate(2400, 2, 29), makeDate(2400, 2, 29), 1,
                          DOW_ALL) == 1);
  assert(countConnections(makeDate(4000, 2, 29), makeDate(4000, 2, 29), 1,
                          DOW_ALL) == -1);

  d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_ALL);
  assert(d.m_Year == 2025 && d.m_Month == 1 && d.m_Day == 8);
  d = endDate(makeDate(2024, 10, 1), 100, 6, DOW_ALL);
  assert(d.m_Year == 2024 && d.m_Month == 10 && d.m_Day == 20);
  d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_WORKDAYS);
  assert(d.m_Year == 2025 && d.m_Month == 2 && d.m_Day == 17);
  d = endDate(makeDate(2024, 10, 1), 100, 4, DOW_WORKDAYS);
  assert(d.m_Year == 2024 && d.m_Month == 11 && d.m_Day == 4);
  d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_THU);
  assert(d.m_Year == 2026 && d.m_Month == 9 && d.m_Day == 2);
  d = endDate(makeDate(2024, 10, 1), 100, 2, DOW_THU);
  assert(d.m_Year == 2025 && d.m_Month == 9 && d.m_Day == 17);
  d = endDate(makeDate(2024, 10, 1), 100, 0, DOW_THU);
  assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
  d = endDate(makeDate(2024, 10, 1), 100, 1, 0);
  assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
  return EXIT_SUCCESS;
}
