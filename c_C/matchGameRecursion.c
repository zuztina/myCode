#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* The task is to write a program that can solve the "jumping matches" puzzle.
on the table there are n matches in a row next to each other. The task is to create "crosses" from these matches, i.e. move the match to positions left or right and place it crosswise over the lying match. The match can only be moved once while it is lying on its own (so the cross cannot be disassembled). The match can be moved to the left or right, when moving you need to jump over the specified number of lying matches. A skipped single match counts as 1, a cross counts as 2.
Example for n=12 (number of matches) and k=4 (jump size): 
  0   1   2   3   4   5   6   7   8   9  10  11  
  |   |   |   |   |   |   |   |   |   |   |   |   
4R
  |   |   |   |       |   |   |   |   X   |   |   
8L
  |   |   X   |       |   |   |       X   |   |   
5R
  |   |   X   |           |   |       X   X   |   
7L
  |   X   X   |           |           X   X   |   
0R
      X   X   X           |           X   X   |   
6R
      X   X   X                       X   X   X
The input of the program are two integers n and k. The number n represents the number of widths (n > 1) and k represents the jump size (k ≥ 0).
The output of the program is all the sequences of moving matches, which can be used to create crosses for the given input. Each move sequence is on a separate line. 
Individual moves are separated by commas on the line. For each move, the match number (counted from zero) and the direction of movement (L, R) are determined. 
The last line of the output gives the number of different solutions that exist.
*/
// one move
typedef struct
{
    int matchIndex;
    char wayOfMove;
} move;

void printArray(move* arr, int length)
{
    for (int i = 0; i < length-1; i++)
    {
        printf("%d%c,", arr[i].matchIndex, arr[i].wayOfMove);
    }
    printf("%d%c", arr[length-1].matchIndex, arr[length-1].wayOfMove);
    printf("\n");
}

void copyIntArray(int arr[], int copy[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        copy[i] = arr[i];
    }
}

void copyMoveArray(move arr[], move copy[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        copy[i] = arr[i];
    }
}

// returns 0 or 1 according to if the move was made
int tryMoveLeft(int indexOfMovedMatch, int *arrMatch, int jump)
{
    int jumpCounter = 0;
    int checkedIndex = indexOfMovedMatch - 1;

    // jump
    while (jumpCounter < jump && checkedIndex >= 0)
    {
        jumpCounter += arrMatch[checkedIndex];
        checkedIndex--;
    }

    // skip empty slots
    while (checkedIndex > 0 && arrMatch[checkedIndex] == 0)
    {
        checkedIndex--;
    }

    // check if move was possible
    if (jumpCounter == jump && checkedIndex >= 0 && arrMatch[checkedIndex] == 1)
    {
        // 2 represents a cross
        arrMatch[checkedIndex] = 2;
        arrMatch[indexOfMovedMatch] = 0;
        return 1;
    }

    return 0;
}

// returns 0 or 1 according to if the move was made
int tryMoveRight(int indexOfMovedMatch, int *arrMatch, int matchesCount, int jump)
{
    int jumpCounter = 0;
    int checkedIndex = indexOfMovedMatch + 1;

    // jump
    while (jumpCounter < jump && checkedIndex < matchesCount)
    {
        jumpCounter += arrMatch[checkedIndex];
        checkedIndex++;
    }

    // skip empty slots
    while (checkedIndex < matchesCount && arrMatch[checkedIndex] == 0)
    {
        checkedIndex++;
    }

    // check if move was possible
    if (jumpCounter == jump && checkedIndex < matchesCount && arrMatch[checkedIndex] == 1)
    {
        arrMatch[checkedIndex] = 2;
        arrMatch[indexOfMovedMatch] = 0;
        return 1;
    }

    return 0;
}

int matchMoves(int *arrMatch, move *arrMove, int movesArrayLength, int currentMoveIndex, int matchesCount, int jump)
{
    int allDone = 1;
    for(int i=0; i < matchesCount; i++) {
        if(arrMatch[i] == 1) {
            allDone = 0;
            break;
        }
    }

    // all matches are in crosses, no work left, print a result and return 
    if(allDone) {
        printArray(arrMove, matchesCount/2);
        return 1;
    }

    int solutionsCount = 0;
    for (int i = 0; i < matchesCount; i++)
    {
        if(arrMatch[i] != 1) {
            continue;
        }

        // move to left
        int *matchArrayForLeftMove = (int *)calloc(matchesCount, sizeof(int));
        copyIntArray(arrMatch, matchArrayForLeftMove, matchesCount);
        if (tryMoveLeft(i, matchArrayForLeftMove, jump))
        {
            // move was made. Save it and call matchMoves again to see if it will lead to valid solution(s). 

            move *arrMoveForLeftMove = (move *)calloc(movesArrayLength, sizeof(move));
            copyMoveArray(arrMove, arrMoveForLeftMove, movesArrayLength);
            arrMoveForLeftMove[currentMoveIndex].matchIndex = i;
            arrMoveForLeftMove[currentMoveIndex].wayOfMove = 'L';
            solutionsCount += matchMoves(matchArrayForLeftMove, arrMoveForLeftMove, movesArrayLength, currentMoveIndex+1, matchesCount, jump);
            free(arrMoveForLeftMove);
        }
        free(matchArrayForLeftMove);

        // move to right
        int *matchArrayForRightMove = (int *)calloc(matchesCount, sizeof(int));
        copyIntArray(arrMatch, matchArrayForRightMove, matchesCount);
        if (tryMoveRight(i, matchArrayForRightMove, matchesCount, jump))
        {
            // move was made. Save it and call matchMoves again to see if it will lead to valid solution(s). 

            move *arrMoveForRightMove = (move *)calloc(movesArrayLength, sizeof(move));
            copyMoveArray(arrMove, arrMoveForRightMove, movesArrayLength);
            arrMoveForRightMove[currentMoveIndex].matchIndex = i;
            arrMoveForRightMove[currentMoveIndex].wayOfMove = 'R';
            solutionsCount += matchMoves(matchArrayForRightMove, arrMoveForRightMove, movesArrayLength, currentMoveIndex+1, matchesCount, jump);
            free(arrMoveForRightMove);
        }
        free(matchArrayForRightMove);
    }  
    
    return solutionsCount;
}

int main()
{
    int matchesCount = 0;
    int jumpedMatches = 0;

    printf("Pocet sirek:\n");
    if (scanf("%d", &matchesCount) != 1 || matchesCount < 1)
    {
        printf("Nespravny vstup.\n");
        return 0;
    }
    printf("Velikost skoku:\n");
    if (scanf("%d", &jumpedMatches) != 1 || jumpedMatches < 0)
    {
        printf("Nespravny vstup.");
        return 0;
    }
    
    // not possible for odd count of matches
    if (matchesCount % 2 != 0)
    {
        printf("Celkem reseni: 0\n");
        return 0;
    }
    int arraylenght = matchesCount;
    int *matchArray = (int *)calloc(arraylenght, sizeof(int));

    // fills array with 1s, represents one match
    for (int i = 0; i < arraylenght; i++)
    {
        matchArray[i] = 1;
    }
    int moveSize = matchesCount / 2;

    move *movesArray = (move *)calloc(moveSize, sizeof(move));
    
    int countOfMoves = matchMoves(matchArray, movesArray, moveSize, 0, matchesCount, jumpedMatches);
    printf("Celkem reseni: %d\n", countOfMoves);
    free(matchArray);
    free(movesArray);
    return 0;
}