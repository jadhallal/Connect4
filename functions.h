#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <stdio.h>
#include <stdlib.h> 

#define ROWS 6
#define COLS 7

void printMatrix(char **matrix)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("1 2 3 4 5 6 7\n");
}

int updateMatrix(int C, char player, char **matrix)
{
    int col = C - 1;

    if (matrix[0][col] == 'A' || matrix[0][col] == 'B')
    {
        printf("Column is full. Choose another one\n");
        return -1;
    }

    for (int i = ROWS - 1; i >= 0; i--)
    {
        if (matrix[i][col] == '.')
        {
            matrix[i][col] = player;
            return i;
        }
    }

    return -1;
}



int fullTable(char **matrix)
{
    for (int i = 0; i < COLS; i++)
    {
        if (matrix[0][i] == '.')
            return 0; 
    }
    return 1; 
}
#endif