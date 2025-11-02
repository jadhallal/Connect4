#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 6
#define COLS 7

int EasyBot(char **matrix)
{
    srand(time(NULL));
    int r = (rand() % COLS) + 1;
    while (matrix[0][r] == 'A' || matrix[0][r] == 'B')
    {
        r = (rand() % COLS) + 1;
    }
    return r;
}
