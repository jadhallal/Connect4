#include <stdio.h>

#define ROWS 6
#define COLS 7

int winCheck(int R, int C, char player, char **matrix)
{
    C = C - 1; 

    // row check
    int count = 0;
    for (int i = 0; i < COLS; i++)
    {
        if (matrix[R][i] == player)
            count++;
        else
            count = 0;
        if (count == 4)
            return 1;
    }

    // column check
    // flag : check later if correct column checking
    count = 0;
    for (int i = 0; i < ROWS; i++)
    {
        if (matrix[i][C] == player)
            count++;
        else
            count = 0;
        if (count == 4)
            return 1;
    }

    // diagonal checking up left till down right
    count = 0;
    int r = R, c = C;
    while (r > 0 && c > 0)
    {
        r--;
        c--;
    }
    while (r < ROWS && c < COLS)
    {
        if (matrix[r][c] == player)
            count++;
        else
            count = 0;
        if (count == 4)
            return 1;
        r++;
        c++;
    }

    // diagonal checking up right till down left
    count = 0;
    r = R; c = C;
    while (r < ROWS - 1 && c > 0)
    {
        r++;
        c--;
    }
    while (r >= 0 && c < COLS)
    {
        if (matrix[r][c] == player)
            count++;
        else
            count = 0;
        if (count == 4)
            return 1;
        r--;
        c++;
    }

    return 0;
}