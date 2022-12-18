#include <time.h>

#include "include/timedate.h"

/* getCurrentDate() ---------------------------------
    Revision    : 1.0.0

    Comments:
    Extracts the year, month and day from the current time.
    Sets the int in the YYYYMMDD format.
   -------------------------------------------------- */
void getCurrentDate(int *_piToday)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int iYear = tm.tm_year + 1900;
    int iMonth = tm.tm_mon + 1;
    int iDay = tm.tm_mday;

    *_piToday = iYear * 10000 + iMonth * 100 + iDay;
}

/* addDays() ----------------------------------------
    Revision    : 1.0.0

    Comments:
    Extracts the year, month and day from the date variable.
    Uses the "tm" structure and the "mktime" function from the time library
    to add the specified number of days to the given date.
    Updates the date with the new values in the YYYYMMDD format.
   -------------------------------------------------- */
void addDays(int *_piDate, int _iDays)
{
    int iDay = *_piDate % 100;
    int iMonth = (*_piDate / 100) % 100;
    int iYear = *_piDate / 10000;

    struct tm tm = {0};
    tm.tm_mday = iDay;
    tm.tm_mon = iMonth - 1;    // "tm_mon" is in the range [0, 11]
    tm.tm_year = iYear - 1900; // "tm_year" is the number of years since 1900
    time_t time = mktime(&tm);
    time += _iDays * 24 * 60 * 60; // add the specified number of seconds
    tm = *localtime(&time);

    //
    *_piDate = (tm.tm_year + 1900) * 10000 + (tm.tm_mon + 1) * 100 + tm.tm_mday;
}