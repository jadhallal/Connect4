#ifndef HARDBOT_H
#define HARDBOT_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#ifndef DEPTH
#define DEPTH 12
#endif

#define ROWS 6
#define COLS 7
#define BOT 'B'
#define HUMAN 'A'

int is_valid_move(char **board, int col);
int get_next_open_row(char **board, int col);
void make_move(char **board, int row, int col, char piece);
void undo_move(char **board, int row, int col);

int winning_move(char **board, char piece);
int score_position(char **board, char me, char opp);
int get_valid_moves(char **board, int *moves);

int minimax_ab(
    char **board,
    int depth,
    int alpha,
    int beta,
    int maximizingPlayer,
    char botPiece,
    char humanPiece,
    int *best_col_out
);

int hardbot(char **board);

#endif
