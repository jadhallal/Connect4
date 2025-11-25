#ifndef MEDIUMBOT_H
#define MEDIUMBOT_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "winCheck.h"
#include "functions.h"

#define ROWS 6
#define COLS 7

int isValidMove(int col, char **matrix);
char **copyMatrix(char **matrix);
void freeMatrix(char **matrix);
int hasTwoInRow(char player, char **matrix);
int mediumBot(char **matrix);

#endif
