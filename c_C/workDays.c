/*The task is to implement two functions that will facilitate the calculation of working days in a given date interval. The required functions have the following interfaces:

bool isWorkDay ( int y, int m, int d )
    The function decides whether the given day is a working day or not. The parameters are the year, month and day to be validated. The return value is true if the specified day is a working day, or false if the specified day is a holiday (Saturday, Sunday, holiday) or an invalid date was specified.
TResult countDays ( int y1, int m1, int d1, int y2, int m2, int d2 )
    The function calculates the number of days and weekdays in the specified interval. The interval is specified by year, month and day of start (y1, m1, d1) and end (y2, m2, d2). We consider the interval to be closed, i.e. we include both border days in the calculation. The return value of the function is a structure with components m_TotalDays (number of days in the specified interval) and m_WorkDays (number of working days in the specified interval). If the function receives invalid parameter values ​​(see below), the function returns a structure where both components are set to -1.
TResult
    structure is declared in the test environment, your implementation will use it, but must not change the declaration. The structure contains two components:

        m_TotalDays - total number of days a
        m_WorkDays - number of working days.

The correct values ​​of the input parameters must meet:
    the year is greater than 2000 (we consider all dates before the year 2000 invalid),
    month is valid (1 to 12),
    the day is valid (1 to the number of days in the month),
    in the countDays function, interval start date ≤ interval end date.
We traditionally consider Monday to Friday as working days. Furthermore, we do not consider public holidays as working days. We consider public holidays according to the following overview:
    1.1, 1.5, 8.5, 5.7, 6.7, 28.9, 28.10, 17.11, 24.12, 25.12 and 26.12
calculations also include leap years
*/
#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct
{
  int m_TotalDays;
  int m_WorkDays;
} TResult;
#endif /* __PROGTEST__ */
#define SIZE_HOLIDAYS 11
typedef struct
{
  int size;
  int day[15];
  int month[15];
} THolidays;
THolidays holidays = {SIZE_HOLIDAYS, {1, 1, 8, 5, 6, 28, 28, 17, 24, 25, 26}, {1, 5, 5, 7, 7, 9, 10, 11, 12, 12, 12}};
int year[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int isLeapYear(int y) // https://stackoverflow.com/questions/6054016/c-program-to-find-day-of-week-given-date
{
  return ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)); 
}
int DatetoDays(int y, int m, int d)
{
  int days = d;
  for (int i = 1; i < m; i++)
  {
    days += year[i];
    if (i == 2 && isLeapYear(y))
    {
      days += 1; // leapyear
    }
  }
  // add a day for every leap year
  for (int i = 2000; i < y; i++)
  {
    days += isLeapYear(i) ? 366 : 365; //
  }

  return days;
}
int daysBetween(int y1, int m1, int d1, int y2, int m2, int d2)
{
  int days1 = DatetoDays(y1, m1, d1);
  int days2 = DatetoDays(y2, m2, d2);
  return days2 - days1 + 1;
}
bool checkDate(int y, int m, int d)
{
  if (y < 2000 || m < 1 || m > 12 || d < 1 || d > 31)
  {
    return false;
  }
  if (isLeapYear(y) == 0 && m == 2 && d > 28)
  {
    return false;
  }
  if (isLeapYear(y) == 1 && m == 2 && d > 29)
  {
    return false;
  }
  if (m != 2 && d > year[m])
  {
    return false;
  }
  return true;
}
bool isHoliday(int m, int d)
{
  for (int i = 0; i <= holidays.size; i++)
  {
    if (m == holidays.month[i] && d == holidays.day[i])
    {
      return true;
    }
  }
  return false;
}
int retweekday(int y, int m, int d)
{
  return (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7;
}
bool isWorkDay(int y, int m, int d)
{
  if (checkDate(y, m, d) == false)
  {
    return false;
  }
  if (isHoliday(m, d) == true)
  {
    return false;
  }
  int weekday = retweekday(y, m, d);
  if (weekday == 0 || weekday == 6)
  {
    return false;
  }
  return true;
}
int countWorkDays(int y1, int m1, int d1, int y2, int m2, int d2)
{
  int count = 0;
  int start = DatetoDays(y1, m1, d1);
  int end = DatetoDays(y2, m2, d2);
  int monthMax = year[m1];
  if (isLeapYear(y1) == true && m1 == 2)
  { // leap year, feb 29 days
    monthMax += 1;
  }
  for (int i = start; i <= end; i++)
  {
    if (isWorkDay(y1, m1, d1) == true)
    {
      count++;
    }
    if (d1 == monthMax && m1 != 12)
    {
      m1++;
      d1 = 1;
      monthMax = year[m1];
      if (isLeapYear(y1) == true && m1 == 2)
      { // leap year, feb 29 days
        monthMax += 1;
      }
      continue;
    }
    if (d1 == monthMax && m1 == 12)
    {
      m1 = 1;
      d1 = 1;
      y1++;
      continue;
    }
    d1++;
  }
  return count;
}

TResult countDays(int y1, int m1, int d1,
                  int y2, int m2, int d2)
{
  TResult result;
  if(checkDate(y1, m1, d1)==false || checkDate(y2, m2, d2)==false || DatetoDays(y1, m1, d1) > DatetoDays(y2, m2, d2) ){
    result.m_TotalDays=-1;
    result.m_WorkDays=-1;
    return result;
  }
  result.m_TotalDays = daysBetween(y1, m1, d1, y2, m2, d2);
  result.m_WorkDays = countWorkDays(y1, m1, d1, y2, m2, d2);
  return result;
}

#ifndef __PROGTEST__
int main(int argc, char *argv[])
{
  TResult r;

  assert(isWorkDay(2023, 10, 10));

  assert(!isWorkDay(2023, 11, 11));

  assert(!isWorkDay(2023, 11, 17));

  assert(!isWorkDay(2023, 11, 31));

  assert(!isWorkDay(2023, 2, 29));

  assert(!isWorkDay(2004, 2, 29));

  assert(isWorkDay(2008, 2, 29));

  assert(!isWorkDay(2001, 2, 29));

  assert(!isWorkDay(1996, 1, 2));

  r = countDays(2023, 11, 1,
                2023, 11, 30);
  assert(r.m_TotalDays == 30);
  assert(r.m_WorkDays == 21);

  r = countDays(2023, 11, 1,
                2023, 11, 17);
  assert(r.m_TotalDays == 17);
  assert(r.m_WorkDays == 12);

  r = countDays(2023, 11, 1,
                2023, 11, 1);
  assert(r.m_TotalDays == 1);
  assert(r.m_WorkDays == 1);

  r = countDays(2023, 11, 17,
                2023, 11, 17);
  assert(r.m_TotalDays == 1);
  assert(r.m_WorkDays == 0);
  
  r = countDays(2023, 1, 1,
                2023, 12, 31);
  assert(r.m_TotalDays == 365);
  assert(r.m_WorkDays == 252);

  r = countDays(2024, 1, 1,
                2024, 12, 31);
  assert(r.m_TotalDays == 366);
  assert(r.m_WorkDays == 254);
  
  r = countDays(2000, 1, 1,
                2023, 12, 31);
  assert(r.m_TotalDays == 8766);
  assert(r.m_WorkDays == 6072);
  
  r = countDays(2001, 2, 3,
                2023, 7, 18);
  assert(r.m_TotalDays == 8201);
  assert(r.m_WorkDays == 5682);
  
  r = countDays(2021, 3, 31,
                2023, 11, 12);
  assert(r.m_TotalDays == 957);
  assert(r.m_WorkDays == 666);

  r = countDays(2001, 1, 1,
                2000, 1, 1);
  assert(r.m_TotalDays == -1);
  assert(r.m_WorkDays == -1);

  r = countDays(2001, 1, 1,
                2023, 2, 29);
  assert(r.m_TotalDays == -1);
  assert(r.m_WorkDays == -1);

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */