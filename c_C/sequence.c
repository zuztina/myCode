/*The task is to implement a program that will analyze the entered number sequence.

We assume a sequence of numbers. From this sequence we select continuous intervals, for example we can select the interval between the 5th and 10th entered number. 
The interval must contain at least two numbers from the input sequence. For the selected interval, we determine the sum of the numbers that make it up. In this way, we select all possible intervals from the input sequence and determine its sum for each interval. 
We are interested in how many pairs of mutually different intervals will have the same sum.
for: 1 5 2 4 2 2 2
there are a total of 21 possible intervals of length at least 2:

0..1: 1 5 sum=6
0..2: 1 5 2 sum=8
0..3: 1 5 2 4 sum=12
0..4: 1 5 2 4 2 sum=14
0..5: 1 5 2 4 2 2 sum=16
0..6: 1 5 2 4 2 2 2 sum=18
1..2: 5 2 sum=7
1..3: 5 2 4 sum=11
1..4: 5 2 4 2 sum=13
1..5: 5 2 4 2 2 sum=15
1..6: 5 2 4 2 2 2 sum=17
2..3: 2 4 sum=6
2..4: 2 4 2 sum=8
2..5: 2 4 2 2 sum=10
2..6: 2 4 2 2 2 sum=12
3..4: 4 2 sum=6
3..5: 4 2 2 sum=8
3..6: 4 2 2 2 sum=10
4..5: 2 2 sum=4
4..6: 2 2 2 sum=6
5..6: 2 2 sum=4

And from these 21 intervals, 12 pairs of different intervals with the same sum can be selected:
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// only 20000 number in sequnce, idk how big sequence of sums
#define MAX_LENGTH_NUM 2001
#define MAX_LENGTH_SUM 2000000
#define unsigned long long ull

void swapNum(int *n1, int *n2)
{
    int tmp = *n1;
    *n1 = *n2;
    *n2 = tmp;
}
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void printArray(int arr[], int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main(void)
{
    int numberCount = 0;
    int countOfSum = 0;
    int sumCounter = 0;
    int arrayNumber[MAX_LENGTH_NUM] = {};
    int arraySum[MAX_LENGTH_SUM] = {};
    int finalcount = 0;
    int tmpCount = 0;
    // reading numberarray
    printf("Posloupnost:\n");

    while (1)
    {
        int number = 0;
        int code = scanf("%d", &number);

        // OK end
        if ((code == EOF && numberCount > 0) )
        {
            break;
        }

        // Error end
        if (code != 1 || (code == EOF && numberCount == 0) || numberCount == 2000  )
        {
            printf("Nespravny vstup.\n");
            return 0;
        }

        arrayNumber[numberCount] = number;
        numberCount++;
    }
    // creating array of sums...then sorting.
    for (int i = 0; i < numberCount - 1; i++)
    {
        int suma = 0;
        suma = arrayNumber[i];

        for (int j = i + 1; j < numberCount; j++)
        {
            int tmpsum = 0;
            tmpsum = suma + arrayNumber[j];
            suma = tmpsum;
            arraySum[countOfSum] = suma;
            countOfSum++;
        }
    }
    qsort(arraySum, countOfSum, sizeof(int), cmpfunc);
    // count of identicaal sums
    for (int i = 0; i < countOfSum; i++) {

        if (arraySum[i] == arraySum[i + 1]) {
            sumCounter++;
            
        }
        
        if (arraySum[i] != arraySum[i + 1]) {
            if (sumCounter > 1) {
                tmpCount = (( (sumCounter + 1) * (sumCounter )) / 2);
                sumCounter=0;
                
            }
            else {
                tmpCount += sumCounter;
                sumCounter=0;
            }
            finalcount += tmpCount;
            tmpCount=0;
            sumCounter=0;  
        }
        
    }

    printf("Pocet dvojic: %d\n", finalcount);
    return 0;
}