#include "easyBot.h"

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
