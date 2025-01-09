#ifndef __PROGTEST__
#include "progtest.h"

TDATE makeDate(unsigned y, unsigned m, unsigned d) {
  TDATE res = {y, m, d};
  return res;
}
#endif /* __PROGTEST__ */

#include <stdbool.h>

typedef struct TWeekConnections {
  long long connections[7];
} TWeekConnections;

/* Some date utilities */
bool eqDate(TDATE a, TDATE b) {
  return a.m_Year == b.m_Year && a.m_Month == b.m_Month && a.m_Day == b.m_Day;
}
/// TDATE a > TDATE b
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
  return false;
}

/// TDATE a < TDATE b
bool ltDate(TDATE a, TDATE b) { return !eqDate(a, b) && !gtDate(a, b); }

bool isLeapYear(int year) {
  return year % 4000 == 0
             ? false
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

/// Date is valid by progtest rules
bool isDateValid(TDATE date) {
  if (date.m_Day == 0)
    return false;

  unsigned month_days = monthDays(date.m_Month, date.m_Year);
  if (date.m_Day > month_days)
    return false;

  return true;
}

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

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

TDATE subDayFromDate(TDATE date) {
  if (date.m_Day > 1) {
    date.m_Day -= 1;
  } else {
    if (date.m_Month > 1) {
      date.m_Month -= 1;
    } else {
      date.m_Month = 12;
      date.m_Year -= 1;
    }

    date.m_Day = monthDays(date.m_Year, date.m_Month);
  }

  return date;
}

TDATE addDayToDate(TDATE date) {
  if (date.m_Day < monthDays(date.m_Month, date.m_Year)) {
    date.m_Day += 1;
  } else {
    if (date.m_Month < 12) {
      date.m_Month += 1;
    } else {
      date.m_Month = 1;
      date.m_Year += 1;
    }

    date.m_Day = 1;
  }

  return date;
}

TDATE addDaysToDate(TDATE date, long long days) {
  TDATE from = date;
  long long addYears = days / 365;

  // Subtract years from days
  days -= addYears * 365;
  int leaps = 0;

  if (addYears > 0) {
    leaps = leapYears(date, addYears);
    days -= leaps;

    if (isLeapYear(date.m_Year) && gtDate(date, makeDate(date.m_Year, 2, 29))) {
      days += 1;
    }
  }
  date.m_Year += addYears;
  date.m_Day += days;

  if (leaps > 0 && isLeapYear(date.m_Year) &&
      (from.m_Month <= 2 && date.m_Month <= 2)) {
    date.m_Day += 1;
  }

  while ((int)date.m_Day < 1 ||
         (int)date.m_Day > (int)monthDays(date.m_Month, date.m_Year) ||
         (int)date.m_Month > 12 || (int)date.m_Month < 1) {
    unsigned int days = monthDays(date.m_Month, date.m_Year);
    if ((int)date.m_Day > (int)days) {
      date.m_Day -= (long long)days;
      date.m_Month += 1;
    } else if ((int)date.m_Day < 1) {
      date.m_Month -= 1;
      if (date.m_Month < 1) {
        date.m_Year -= 1;
        date.m_Month = 12;
      }
      date.m_Day += monthDays(date.m_Month, date.m_Year);
    }

    if ((int)date.m_Month > 12) {
      date.m_Year += 1;
      date.m_Month = 1;
    } else if ((int)date.m_Month < 1) {
      date.m_Year -= 1;
      date.m_Month = 12;
    }
  }

  return date;
}

/// 0 = monday, 6 = sunday
int dayOfWeek(TDATE date) {
  long long monDiff = daysBetweenDates(makeDate(1999, 12, 27), date);

  return monDiff % 7;
}

TWeekConnections getWeekConnections(unsigned perWorkDay, unsigned dowMask) {
  TWeekConnections weekConnections;
  weekConnections.connections[0] = perWorkDay * (dowMask & DOW_MON);
  weekConnections.connections[1] = perWorkDay * ((dowMask & DOW_TUE) >> 1);
  weekConnections.connections[2] = perWorkDay * ((dowMask & DOW_WED) >> 2);
  weekConnections.connections[3] = perWorkDay * ((dowMask & DOW_THU) >> 3);
  weekConnections.connections[4] = perWorkDay * ((dowMask & DOW_FRI) >> 4);
  weekConnections.connections[5] =
      (perWorkDay / 2 + (perWorkDay % 2 != 0)) * ((dowMask & DOW_SAT) >> 5);
  weekConnections.connections[6] =
      (perWorkDay / 3 + (perWorkDay % 3 != 0)) * ((dowMask & DOW_SUN) >> 6);

  return weekConnections;
}

long long countConnections(TDATE from, TDATE to, unsigned perWorkDay,
                           unsigned dowMask) {
  if (!isDateValid(from) || !isDateValid(to) || ltDate(to, from))
    return -1;

  TWeekConnections weekConnections = getWeekConnections(perWorkDay, dowMask);
  int wholeWeekConnections = 0;
  for (int i = 0; i < 7; i++)
    wholeWeekConnections += weekConnections.connections[i];
  long long total = 0;

  long long totalDays = daysBetweenDates(from, to);
  int fromDay = dayOfWeek(from);

  total += (long long)wholeWeekConnections * (totalDays / 7);

  for (int i = 0; i <= totalDays % 7; i++) {
    total += weekConnections.connections[(fromDay + i) % 7];
  }

  return total;
}

TDATE endDate(TDATE from, long long connections, unsigned perWorkDay,
              unsigned dowMask) {
  if (!isDateValid(from) || connections < 0 || perWorkDay == 0 || dowMask == 0)
    return makeDate(0, 0, 0);

  TWeekConnections weekConnections = getWeekConnections(perWorkDay, dowMask);
  long long wholeWeekConnections = 0;
  for (int i = 0; i < 7; i++)
    wholeWeekConnections += weekConnections.connections[i];
  long long addDays = 0;

  int fromDay = dayOfWeek(from);

  if (connections < weekConnections.connections[fromDay])
    return makeDate(0, 0, 0);

  long long weeks = connections / wholeWeekConnections;
  addDays += weeks * 7;

  long long remainingConnections = connections % wholeWeekConnections;
  for (int i = 0; i <= 7; i++) {
    long long todayConnections = weekConnections.connections[(fromDay + i) % 7];
    if (remainingConnections < todayConnections)
      break;
    remainingConnections -= todayConnections;
    addDays += 1;
  }

  // -1 because we the range includes the from date
  return addDaysToDate(from, addDays - 1);
}
