#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

#define ROWS 6
#define COLS 7

void printMatrix(char **matrix);
int updateMatrix(int C, char player, char **matrix);
int fullTable(char **matrix);

#endif
