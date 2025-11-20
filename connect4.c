#include <stdio.h>
#include <stdlib.h>
#include "easyBot.h"
#include "mediumBot.h"
#include "winCheck.h"
#include "functions.h"
#include "hardbot.h"

int main()
{
    char **matrix;
    matrix = (char **)malloc(ROWS * sizeof(char *));
    for (int i = 0; i < ROWS; i++)
        matrix[i] = (char *)malloc(COLS * sizeof(char));

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            matrix[i][j] = '.';

    printf("Choose the mode:\n1 (multiplayer)\n2 (easy bot)\n3 (medium bot)\n4 (hard bot)\n");
    int n;
    if (scanf("%d", &n) != 1)
    {
        printf("Please enter a valid number!\n");
        exit(1);
    }

    while (n < 1 || n > 4)
    {
        printf("Please choose a valid number (1-2-3-4): ");
        if (scanf("%d", &n) != 1)
        {
            printf("Please enter a valid number!\n");
            exit(1);
        }
    }

    int win = 0;
    char playerA = 'A';
    char playerB = 'B';
    char currentPlayer = playerA;
    int easyBotMode = (n == 2);
    int mediumBotMode = (n == 3);
    int hardBotMode = (n == 4);

    int again = 0;
    while (again == 0)
    {
        if (n == 2 || n == 3 || n == 4)
        {
            printf("\nDo you want to start first?\n");
            printf("1 (yes)\n2 (no)\n");
            int first;
            if (scanf("%d", &first) != 1)
            {
                printf("Please enter a valid number!\n");
                exit(1);
            }

            if (first == 1)
                currentPlayer = playerA;
            else
                currentPlayer = playerB;
        }

        printMatrix(matrix);

        while (win == 0)
        {
            int column;

            // Bot turn
            if ((easyBotMode || mediumBotMode || hardBotMode) && currentPlayer == playerB)
            {
                if (easyBotMode)
                {
                    column = EasyBot(matrix);
                    printf("Easy Bot chose column %d\n", column);
                }
                else if (mediumBotMode)
                {
                    printf("checking\n");
                    column = mediumBot(matrix);
                    printf("Medium Bot chose column %d\n", column);
                }
                else if (hardBotMode)
                {
                    column = hardbot(matrix) + 1;
                    printf("Hard Bot chose column %d\n", column);
                }
            }
            else // Player turn
            {
                printf("\nPlayer %c, choose a column (1-7): \n", currentPlayer);

                if (scanf("%d", &column) != 1)
                {
                    printf("Please enter a valid number!\n");
                    exit(1);
                }

                if (column < 1 || column > COLS)
                {
                    printf("Invalid column. Choose between 1 and %d.\n", COLS);
                    continue;
                }
            }

            int row = updateMatrix(column, currentPlayer, matrix);
            if (row == -1)
                continue;

            printMatrix(matrix);

            win = winCheck(row, column, currentPlayer, matrix);
            if (win == 1)
            {
                if (easyBotMode && currentPlayer == playerB)
                    printf("\nEasy Bot wins!\n");
                else if (mediumBotMode && currentPlayer == playerB)
                    printf("\nMedium Bot wins!\n");
                else if (hardBotMode && currentPlayer == playerB)
                    printf("\nHard Bot wins!\n");
                else
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


        printf("\nDo you want to play again?\n");
        printf("0 (yes)\n1 (no)\n");
        if (scanf("%d", &again) != 1)
        {
            printf("Please enter a valid number!\n");
            exit(1);
        }
        if (again == 0)
        {
            // Reset the game
            for (int i = 0; i < ROWS; i++){
                for (int j = 0; j < COLS; j++){
                    matrix[i][j] = '.';
                    win = 0;
                }
            }
        }
    }

    for (int i = 0; i < ROWS; i++)
        free(matrix[i]);
    free(matrix);

    return 0;
}