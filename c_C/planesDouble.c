/*We want to implement a program that, given the coordinates of the planes, will decide where there is a potential collision, i.e. which planes are closest to each other.

The input of the program is the coordinates and designation of the aircraft. For simplicity, we assume that the coordinates of the planes are plane, that is, the position of the plane is determined by the x and y coordinates. The coordinates are in the form of decimal numbers. The coordinates are separated by a comma, the coordinates are followed by a colon and the name (identification) of the aircraft. The aircraft name is a string without spaces and whitespace characters, maximum 199 characters long, and is not unique (there may be two or more aircraft with the same name in the input). There may be many aircraft entered in this way, their number is not known in advance. Input ends with active end-of-file (EOF on stdin). The input format is obvious from the samples below.

The output of the program is the distance between the pair of nearest planes. If there are more pairs of planes in the same (smallest) distance, the program will print the number of pairs of planes in this (smallest) distance. The following is a list of all pairs of aircraft that have this (smallest) distance between them. The list of pairs of aircraft has no specified order (the lines of the list can be arranged arbitrarily, as well as the planes in a pair on the line). During the comparison, the test environment will adjust the listing if necessary.

The program must handle the input data. If the input data is incorrect, the program detects this, displays an error message, and exits. An error is considered:

    non-numeric coordinates (no valid decimal number),
    missing/excess separators (comma, colon),
    less than two aircraft on entry (otherwise minimum distance is not defined).
*/
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#define INITIAL_SIZE_ARRAY 2
#define MULTI_SIZE_FACTOR 2
#define PLANE_NAME_SIZE 200

typedef struct
{
    double coordinateX;
    double coordinateY;
    char planeName[PLANE_NAME_SIZE];
} planeInfo;

typedef struct
{
    int indexA;
    int indexB;
} planePair;

// calculates the distance of planes based on their coordinates on x and y
double planeDistance(double x1, double x2, double y1, double y2)
{
    return sqrt(pow(fabs(x1 - x2), 2) + pow(fabs(y1 - y2), 2));
}

// prints an array, that consists of pairs of planes from two arrays
void printArray(planeInfo *arr1, planePair *arr2, int length)
{
    for (int i = 0; i < length; i++)
    {
        int a = arr2[i].indexA;
        int b = arr2[i].indexB;
        printf("%s - %s\n", arr1[a].planeName, arr1[b].planeName);
    }
}

int main(void)
{
    planeInfo *planeList = (planeInfo *)calloc(INITIAL_SIZE_ARRAY, sizeof(planeInfo));
    planePair *twoPlanes = (planePair *)calloc(INITIAL_SIZE_ARRAY, sizeof(planePair));
    int countOfPlanes = 0;
    int sizeArray = INITIAL_SIZE_ARRAY;

    printf("Pozice letadel:\n");
    int arrayIndex = 0;
    while (1)
    {
        if (countOfPlanes >= sizeArray)
        {
            sizeArray *= MULTI_SIZE_FACTOR;
            planeList = (planeInfo *)realloc(planeList, sizeArray * sizeof(planeInfo));
        }

        int code = scanf(
            " %lf,%lf: %199s ",
            &planeList[arrayIndex].coordinateX,
            &planeList[arrayIndex].coordinateY,
            planeList[arrayIndex].planeName);
        
        // OK end
        if ((code == EOF && countOfPlanes >= 2))
        {
            break;
        }

        // Error end
        if (code != 3 || (code == EOF && countOfPlanes <= 1))
        {
            printf("Nespravny vstup.\n");
            free(planeList);
            free(twoPlanes);
            return 0;
        }
        countOfPlanes++;
        arrayIndex++;
    }

    // minimumDistance set to DBL_MAX to surely change during the first run of the for-cycle 
    double minimumDistance = __DBL_MAX__;
    int countOfClosePlanes = 0;
    double tmpMin = 0;

    for (int i = 0; i < countOfPlanes - 1; i++)
    {

        for (int j = i + 1; j < countOfPlanes; j++)
        {

            tmpMin = planeDistance(planeList[i].coordinateX, planeList[j].coordinateX, planeList[i].coordinateY, planeList[j].coordinateY);

            if (countOfClosePlanes >= sizeArray)
            {
                sizeArray *= MULTI_SIZE_FACTOR;
                twoPlanes = (planePair *)realloc(twoPlanes, sizeArray * sizeof(planePair));
            }

            // comparing equality of two float numbers to eliminate error on positions after decimal point 
            if (fabs(minimumDistance - tmpMin) <= 1000 * DBL_EPSILON * fabs(minimumDistance + tmpMin))
            {
                twoPlanes[countOfClosePlanes].indexA = i;
                twoPlanes[countOfClosePlanes].indexB = j;
                countOfClosePlanes++;
            }
            
            // significant inequality, new minimumDistance, freeing array with planepairs for old minumumDistance
            else if (tmpMin < minimumDistance)
            {
                minimumDistance = tmpMin;
                free(twoPlanes);
                twoPlanes = (planePair *)calloc(INITIAL_SIZE_ARRAY, sizeof(planePair));
                sizeArray = INITIAL_SIZE_ARRAY;
                countOfClosePlanes = 0;
                twoPlanes[countOfClosePlanes].indexA = i;
                twoPlanes[countOfClosePlanes].indexB = j;
                countOfClosePlanes++;
            }
        }
    }
    printf("Vzdalenost nejblizsich letadel: %lf\n", minimumDistance);
    printf("Nalezenych dvojic: %d\n", countOfClosePlanes);
    printArray(planeList, twoPlanes, countOfClosePlanes);
    free(planeList);
    free(twoPlanes);
    return 0;
}
