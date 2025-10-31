#ifndef mediumBot_H
#define mediumBot_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "winCheck.h"
#include "functions.h"


int isValidMove(int col, char **matrix) {
    return (col >= 1 && col <= COLS && matrix[0][col-1] == '.');
}


char** copyMatrix(char** matrix) {
    char** newMatrix = malloc(ROWS * sizeof(char*));
    for (int r = 0; r < ROWS; r++) {
        newMatrix[r] = malloc((COLS + 1) * sizeof(char));
        for (int c = 0; c <= COLS; c++) {
            newMatrix[r][c] = matrix[r][c];
        }
    }
    return newMatrix;
}


void freeMatrix(char** matrix) {
    for (int r = 0; r < ROWS; r++) {
        free(matrix[r]);
    }
    free(matrix);
}

// build 3 in a row
int hasTwoInRow(char player, char **matrix) {
    for (int col = 1; col <= COLS; col++) {
        if (!isValidMove(col, matrix)) continue;

        char **temp = copyMatrix(matrix);
        int row = updateMatrix(col, player, temp);
        int found = 0;

        if (row != -1) {
            int count;

            // horizontal (rows)
            for (int r = 0; r < ROWS && !found; r++) {
                count = 0;
                for (int c = 0; c < COLS; c++) {
                    if (temp[r][c] == player) count++; else count = 0;
                    if (count >= 3) { found = 1; break; }
                }
            }

            // vertical (columns)
            for (int c = 0; c < COLS && !found; c++) {
                count = 0;
                for (int r = 0; r < ROWS; r++) {
                    if (temp[r][c] == player) count++; else count = 0;
                    if (count >= 3) { found = 1; break; }
                }
            }

            // diagonal down-right
            for (int start_r = 0; start_r < ROWS && !found; start_r++) {
                for (int start_c = 0; start_c < COLS && !found; start_c++) {
                    int r = start_r, c = start_c; count = 0;
                    while (r < ROWS && c < COLS) {
                        if (temp[r][c] == player) count++; else count = 0;
                        if (count >= 3) { found = 1; break; }
                        r++; c++;
                    }
                }
            }

            // diagonal down-left
            for (int start_r = 0; start_r < ROWS && !found; start_r++) {
                for (int start_c = 0; start_c < COLS && !found; start_c++) {
                    int r = start_r, c = start_c; count = 0;
                    while (r < ROWS && c >= 0) {
                        if (temp[r][c] == player) count++; else count = 0;
                        if (count >= 3) { found = 1; break; }
                        r++; c--;
                    }
                }
            }
        }

        freeMatrix(temp);
        if (found) return col;
    }
    return -1;
}


int mediumBot(char** matrix) {

    //Play a winning move if possible
    for (int i = 1; i <= COLS; i++) {
        if (!isValidMove(i, matrix)) continue;;

        char** temp = copyMatrix(matrix);
        int row = updateMatrix(i, 'B', temp);

        if (row != -1 && winCheck(row, i, 'B', temp)) {
            freeMatrix(temp);
            return i; 
        }
        freeMatrix(temp);
    }

    //Block opponent’s winning move if possible
    for (int i = 1; i <= COLS; i++) {
        if (!isValidMove(i, matrix)) continue;

        char** temp = copyMatrix(matrix);
        int row = updateMatrix(i, 'A', temp);

        if (row != -1 && winCheck(row, i, 'A', temp)) {
            freeMatrix(temp);
            return i; 
        }
        freeMatrix(temp);
    }

    //Build 3-in-a-row 
    int move = hasTwoInRow('B', matrix);
    if (move != -1) return move;
   

    // random move
    srand(time(NULL));
    int randomMove;
    do {
        randomMove = rand() % COLS + 1;
    } while (!isValidMove(randomMove, matrix));

    return randomMove;
}

#endif
