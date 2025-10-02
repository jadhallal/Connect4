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

int fullTable(char **matrix)
{
    for (int i = 0; i < COLS; i++)
    {
        if (matrix[0][i] == '.')
            return 0; 
    }
    return 1; 
}

int main()
{
    char **matrix;
    matrix = (char **)malloc(ROWS * sizeof(char *));
    for (int i = 0; i < ROWS; i++)
        matrix[i] = (char *)malloc(COLS * sizeof(char));

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            matrix[i][j] = '.';
        }
    }

    printMatrix(matrix);
    int win = 0;
    char playerA = 'A';
    char playerB = 'B';
    char currentPlayer = playerA;

    while (win == 0)
    {
        printf("\nPlayer %c, choose a column (1-7): \n", currentPlayer);
        int column;
        if(scanf("%d", &column) != 1){
            printf("Please enter a valid nummber!");
            exit(1);
        }
        
        if (column < 1 || column > 7 )
        {
            printf("Invalid column. Choose between 1 and %d.\n", COLS);
            continue;
            
        }

        int row = updateMatrix(column, currentPlayer, matrix);
        if (row == -1)
            continue;

        printMatrix(matrix);
        win = winCheck(row, column, currentPlayer, matrix);
        if (win == 1)
        {
            printf("\nPlayer %c wins!\n", currentPlayer);
            break;
        }

        if (fullTable(matrix) == 1)
        {
            printf("\nIt's a draw!\n");
            break;
        }

        currentPlayer = (currentPlayer == playerA) ? playerB : playerA;
    }

    for (int i = 0; i < ROWS; i++)
        free(matrix[i]);
    free(matrix);

    return 0;
}
