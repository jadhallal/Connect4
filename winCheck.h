#ifndef WINCHECK_H
#define WINCHECK_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define ROWS 6
#define COLS 7

typedef struct
{
    int R, C;
    char player;
    char **matrix;
    int result;
} Threadargs;

void *check_row(void *arg);
void *check_column(void *arg);
void *check_diag_down(void *arg);
void *check_diag_up(void *arg);

int winCheck(int R, int C, char player, char **matrix);

#endif
