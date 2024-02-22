/*The task is to create a program that will help with statistics adjustments by appropriate selection of data. We assume that we record our customers' reviews in the e-shop. Reviews are coming gradually. 
For each review, we have the entered date, own numerical rating (whole positive number) and the text of the review. The input data is in non-descending order, but multiple reviews can be entered in one day. An example of an input review is:

+ 2023-11-10 10 Short_review

A review with a rating of 10 and the comment Short_review is entered here on 11/10/2023 (the comment does not contain white characters and has a maximum length of 4096 characters). The next review must be dated 11/10/2023 or later (not earlier).

When creating statistics, we want to find a continuous section of reviews from some starting day to some ending day such that the sum of the ratings given in this period is as close as possible to the specified value. So for example for input:

? 100

we want to find a non-empty time interval such that the sum of the ratings in this interval is as close as possible to 100. 
In general, there can be more such intervals, for example, two different intervals with a sum of 90 and a third with a sum of 110. If there are several intervals with equally similar ratings, we will consider result an interval with a later end date (see second sample run, first query). 
If there were two equally suitable intervals even for the same end date, we will give priority to the interval with the larger sum of ratings (second sample run, second query).

When searching for an interval, we work with a granularity of one day. If multiple reviews are entered in the start/end day of the found interval, we must include all of them in the result (cannot use only some of the start/end day reviews). 
This can be seen in the sample run on the query ? 57. The answer found is 45 (the whole day of 11.11), although the sum of 60 = 45 + 15 (the whole day of 11.11 and part of 10.11) would be closer.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define REV_SIZE 4096
#define DATA_SIZE 4

const int year[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
/**
 * @brief Checks if the year is a leap year
 * Inspired from https://stackoverflow.com/questions/6054016/c-program-to-find-day-of-week-given-date
 *
 * @param y Year
 * @return int 1 if the year is a leap year, 0 otherwise
 */
int isLeapYear(int y)
{
    return ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0));
}

/**
 * @brief Returns the number of days in a given month
 *
 * @param y Year
 * @param m Month
 * @return int Number of days in the month
 */
int maxDay(int y, int m)
{
    if (m == 2 && isLeapYear(y))
    {
        return 29;
    }

    return year[m];
}

/**
 * @brief Checks if day, month, year are not in range
 *
 * @param y Year
 * @param m Month
 * @param d Day
 * @return int 1 if the date is not in range, 0 otherwise
 */
int checkDate(int y, int m, int d)
{
    return (y < 1800 || y > 3000 || d < 1 || m < 1 || m > 12 || d > maxDay(y, m));
}

/**
 * @brief Returns the number of days from the beginning of the year to the given date
 *
 * @param y Year
 * @param m Month
 * @param d Day
 * @return int Number of days
 */
int dateToDays(int y, int m, int d) // returns date in days
{
    int days = d;
    for (int i = 1; i < m; i++)
    {
        days += year[i];
        if (i == 2 && isLeapYear(y))
        {
            days += 1; // Leap year
        }
    }

    // Add a day for every leap year
    for (int i = 1800; i < y; i++)
    {
        days += isLeapYear(i) ? 366 : 365;
    }

    return days;
}

typedef struct review
{
    char rev[REV_SIZE + 10];
    int y, m, d;
    int val;
    int dtd; // date to days value
} TReview;

/**
 * @brief Populates the TReview struct with the given values
 *
 * @param rev
 * @param y
 * @param m
 * @param d
 * @param val
 * @param str
 * @param dtd
 */
void fillReview(TReview *rev, int y, int m, int d, int val, const char *str, int dtd)
{
    // Fill in the struct members
    rev->y = y;
    rev->m = m;
    rev->d = d;
    rev->val = val;

    // Using strcpy to copy the string
    strcpy(rev->rev, str);

    rev->dtd = dtd;
}

typedef struct interval
{
    int begin;
    int end;
    int val;
    int diff;
} TInterval;

/**
 * @brief Populates the TInterval struct with the given values
 *
 * @param m
 * @param b
 * @param e
 * @param val
 * @param dif
 */
void fillInterval(TInterval *m, int b, int e, int val, int dif)
{
    // Fill in the struct members
    m->begin = b;
    m->end = e;
    m->val = val;
    m->diff = dif;
}

/**
 * @brief Checks if we should swap the intervals
 *
 * @param max Max interval
 * @param tmp Current interval to check
 * @param data Reviews
 * @return int 1 if the interval is "bigger" than the max inerval, 0 otherwise
 */
int swapIntervals(TInterval max, TInterval tmp, TReview *data)
{
    if (max.diff > tmp.diff)
    {
        return 1;
    }

    if (max.diff == tmp.diff)
    {
        // The new interval contains newer reviews than max
        if (data[max.end].dtd < data[tmp.end].dtd)
        {
            return 1;
        }

        // The new interval has more reviews than max
        if (data[max.end].dtd == data[tmp.end].dtd)
        {
            int winMax = max.end - max.begin;
            int winTmp = tmp.end - tmp.begin;

            if (winMax < winTmp)
            {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * @brief Checks if the begging/end of inteval is the first/last review of the day
 *
 * @param data Reviews
 * @param size Reviews size
 * @param x begin index
 * @param y end index
 * @return int 1 if the interval is NOT the first/last review of the day, 0 otherwise
 */
int checkEnds(TReview *data, int size, int x, int y)
{
    if ((x != 0) && ((data[x - 1].d == data[x].d) &&
                     (data[x - 1].m == data[x].m) &&
                     (data[x - 1].y == data[x].y)))
    {
        return 0; // false
    }

    if ((y != size - 1) && ((data[y].d == data[y + 1].d) &&
                            (data[y].m == data[y + 1].m) &&
                            (data[y].y == data[y + 1].y)))
    {
        return 0;
    }

    return 1;
}

/**
 * @brief Prints the error message and frees the allocated memory
 *
 * @param data
 * @param prefix
 */
void wrongInput(TReview *data, int *prefix)
{
    printf("Nespravny vstup.\n");
    free(data);
    free(prefix);
}

/**
 * @brief Prints the answer for query '?'
 *
 * @param interval
 * @param data
 */
void printQuestion(TInterval interval, TReview *data)
{
    printf("%d-%02d-%02d - %d-%02d-%02d: %d\n",
           data[interval.begin].y,
           data[interval.begin].m,
           data[interval.begin].d,
           data[interval.end].y,
           data[interval.end].m,
           data[interval.end].d,
           interval.val);

    for (int i = interval.begin; i <= interval.end; i++)
    {
        printf("  %d: %s\n", data[i].val, data[i].rev);
    }
}

/**
 * @brief Prints the answer for query '#'
 *
 * @param interval
 * @param data
 */
void printRequest(TInterval interval, TReview *data)
{
    printf("%d-%02d-%02d - %d-%02d-%02d: %d\n",
           data[interval.begin].y,
           data[interval.begin].m,
           data[interval.begin].d,
           data[interval.end].y,
           data[interval.end].m,
           data[interval.end].d,
           interval.val);
}

/**
 * @brief
 *
 * @param database Reviews
 * @param revCount Reviews count
 * @param value Searched value
 * @param[out] final Final output interval
 * @param arr Prefix
 */
void findInterval(TReview *database, int revCount, int value, TInterval *final, int *arr)
{
    // Range of interval thats checked, range: 0-?, 0-?-1, 0-?-2, ...,
    // starting with the whole array, then smaller one - thats moved thru the array, to try all possibilities
    int diff = revCount - 1;

    int i = 0;
    while (1)
    {
        if (i + diff <= revCount - 1) // checks if (range + starting index of interval) exceeds the end of array to not go over the last index
        {
            if (checkEnds(database, revCount, i, i + diff) == 0)
            {
                i++;
                continue;
            }

            TInterval tmp;
            tmp.begin = i;
            tmp.end = i + diff;

            if (diff != 0)
            {
                tmp.val = arr[i + diff + 1] - arr[i];
            }
            else
            {
                tmp.val = database[i].val;
            }

            tmp.diff = abs(value - tmp.val); // distance from searched value on numerical axis

            if (final->diff < 0)
            {
                *final = tmp;
            }
            else if (swapIntervals(*final, tmp, database) == 1)
            {
                *final = tmp;
            }

            i++; // move on to the next interval, of same range, moved one place to the left
            continue;
        }

        diff--;
        if (diff < 0) // already checked all possible ranges
        {
            break;
        }
        i = 0;
    }
}

/**
 * @brief Adds a review to the database, called when input '+'
 *
 * @param database Reviews
 * @param prefix
 * @param[out] revCount
 * @param[out] dataSize
 * @return int 1 if the input is wrong, 0 otherwise
 */
int addReview(TReview **database, int **prefix, int *revCount, int *dataSize)
{
    int year, month, day, score;
    char review[REV_SIZE + 10];

    if (*revCount == *dataSize)
    {
        // Realloc
        *dataSize += *dataSize / 2;
        *database = (TReview *)realloc(*database, *dataSize * sizeof(TReview));
        *prefix = (int *)realloc(*prefix, (*dataSize + 1) * sizeof(int));
    }

    int ret = scanf(" %d-%d-%d %d %4096s", &year, &month, &day, &score, review);
    if (ret != 5 || score < 1 || checkDate(year, month, day) == 1)
    {
        return 1;
    }

    int dtd = dateToDays(year, month, day);
    if (*revCount > 0 && dtd < (*database)[*revCount - 1].dtd)
    {
        return 1;
    }
    TReview datedReview;
    fillReview(&datedReview, year, month, day, score, review, dtd);
    (*database)[*revCount] = datedReview;
    (*revCount)++;
    (*prefix)[*revCount] = (*prefix)[*revCount - 1] + score;
    return 0;
}

/**
 * @brief Processes the input and prints the question or request message
 *
 * @param database
 * @param prefix
 * @param[out] revCount
 * @param[out] reqCount
 * @param[out] dataSize
 * @param isQuestion Set to 1 if the query is '?', 0 if the query is '#'
 * @return int 1 if the input is wrong, -1 if EOF, 0 otherwise
 */
int processInput(TReview **database, int **prefix, int *revCount, int *reqCount, int *dataSize, int isQuestion)
{
    TInterval final;
    fillInterval(&final, 0, 0, 0, -1); // negative one indictates first run of program, diff must be a positive value
    int value = -1;
    int ret = scanf(" %d", &value);
    if (ret == EOF && value < 0)
    {
        return 1;
    }
    if (ret == EOF)
    {
        return -1; // break;
    }
    if (ret != 1)
    {
        return 1;
    }
    if (value < 1)
    {
        return 1;
    }

    findInterval(*database, *revCount, value, &final, *prefix);

    if (isQuestion)
    {
        printQuestion(final, *database);
    }
    else
    {
        printRequest(final, *database);
    }

    fillInterval(&final, 0, 0, 0, -1);
    (*reqCount)++;

    return 0;
}

int main(void)
{
    char sign;
    int revCount = 0;
    int dataSize = DATA_SIZE;
    int *prefix = (int *)calloc(dataSize + 1, sizeof(int));
    TReview *database = (TReview *)malloc(dataSize * sizeof(TReview));
    int reqCount = 0;

    printf("Recenze:\n");

    while (1)
    {
        int ret = scanf(" %c", &sign);

        if (ret == EOF && (revCount < 1 || reqCount < 1)) // empty data, no queries
        {
            wrongInput(database, prefix);
            return 1;
        }

        if (ret == EOF)
        {
            break;
        }

        if (ret != 1)
        {
            wrongInput(database, prefix);
            return 1;
        }

        if (sign != '+' && sign != '?' && sign != '#')
        {
            wrongInput(database, prefix);
            return 1;
        }

        if (sign == '+')
        {
            if (addReview(&database, &prefix, &revCount, &dataSize) == 1)
            {
                wrongInput(database, prefix);
                return 1;
            }
        }
        else if ((sign == '?') && revCount > 0)
        {
            int ret = processInput(&database, &prefix, &revCount, &reqCount, &dataSize, 1);

            if (ret == 1) // worng input
            {
                wrongInput(database, prefix);
                return 1;
            }

            if (ret == -1) // scanf EOF, end program
            {
                free(database);
                free(prefix);
                return 0;
            }
        }
        else if (sign == '#' && revCount > 0)
        {
            int ret = processInput(&database, &prefix, &revCount, &reqCount, &dataSize, 0);

            if (ret == 1)
            {
                wrongInput(database, prefix);
                return 1;
            }

            if (ret == -1)
            {
                free(database);
                free(prefix);
                return 0;
            }
        }

        if ((sign == '?' || sign == '#') && revCount < 1) // no reviews were added
        {
            wrongInput(database, prefix);
            return 1;
        }
    }

    free(database);
    free(prefix);

    return 0;
}