#ifndef HARDBOT_H
#define HARDBOT_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#ifndef DEPTH
#define DEPTH 12
#endif

/* Constants for your board */
#define ROWS 6
#define COLS 7
#define BOT 'B'
#define HUMAN 'A'

/* Check valid move (top cell empty) */
int is_valid_move(char **board, int col)
{
    if (col < 0 || col >= COLS)
        return 0;
    return (board[0][col] == '.');
}

/* Get next open row for a column */
int get_next_open_row(char **board, int col)
{
    for (int r = ROWS - 1; r >= 0; --r)
        if (board[r][col] == '.')
            return r;
    return -1;
}

/* Place piece */
void make_move(char **board, int row, int col, char piece)
{
    board[row][col] = piece;
}

/* Undo move */
void undo_move(char **board, int row, int col)
{
    board[row][col] = '.';
}

/* Check win */
int winning_move(char **board, char piece)
{
    // Horizontal
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
            if (board[r][c] == piece && board[r][c + 1] == piece &&
                board[r][c + 2] == piece && board[r][c + 3] == piece)
                return 1;
    // Vertical
    for (int c = 0; c < COLS; ++c)
        for (int r = 0; r < ROWS - 3; ++r)
            if (board[r][c] == piece && board[r + 1][c] == piece &&
                board[r + 2][c] == piece && board[r + 3][c] == piece)
                return 1;
    // Diagonal /
    for (int r = 3; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
            if (board[r][c] == piece && board[r - 1][c + 1] == piece &&
                board[r - 2][c + 2] == piece && board[r - 3][c + 3] == piece)
                return 1;
    // Diagonal
    for (int r = 0; r < ROWS - 3; ++r)
    for (int c = 0; c < COLS - 3; ++c)
        if (board[r][c] == piece && board[r + 1][c + 1] == piece &&
            board[r + 2][c + 2] == piece && board[r + 3][c + 3] == piece)
            return 1;

    return 0;
}

/* Evaluate a 4-cell window */
void evaluate_window(char window[4], char piece, int *score, int *oppScore, char opp)
{
    int countPiece = 0, countOpp = 0, countEmpty = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (window[i] == piece)
            countPiece++;
        else if (window[i] == opp)
            countOpp++;
        else if (window[i] == '.')
            countEmpty++;
    }

    if (countPiece == 4)
        *score += 1000;
    else if (countPiece == 3 && countEmpty == 1)
        *score += 50;
    else if (countPiece == 2 && countEmpty == 2)
        *score += 10;

    if (countOpp == 3 && countEmpty == 1)
        *oppScore += 80;
}

/* Heuristic score */
int score_position(char **board, char piece, char opp)
{
    int score = 0, oppThreat = 0;

    int center_col = COLS / 2;
    int centerCount = 0;
    for (int r = 0; r < ROWS; ++r)
        if (board[r][center_col] == piece)
            centerCount++;
    score += centerCount * 6;

    // Horizontal
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
        {
            char window[4];
            for (int i = 0; i < 4; ++i)
                window[i] = board[r][c + i];
            evaluate_window(window, piece, &score, &oppThreat, opp);
        }
    // Vertical
    for (int c = 0; c < COLS; ++c)
        for (int r = 0; r < ROWS - 3; ++r)
        {
            char window[4];
            for (int i = 0; i < 4; ++i)
                window[i] = board[r + i][c];
            evaluate_window(window, piece, &score, &oppThreat, opp);
        }
    // Diagonal 
    for (int r = 0; r < ROWS - 3; ++r)
    for (int c = 0; c < COLS - 3; ++c)
    {
        char window[4];
        for (int i = 0; i < 4; ++i)
            window[i] = board[r + i][c + i];
        evaluate_window(window, piece, &score, &oppThreat, opp);
    }
    // Diagonal /
    for (int r = 3; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
        {
            char window[4];
            for (int i = 0; i < 4; ++i)
                window[i] = board[r - i][c + i];
            evaluate_window(window, piece, &score, &oppThreat, opp);
        }

    score -= oppThreat * 2;
    return score;
}

/* Get valid moves (center first) */
int get_valid_moves(char **board, int *moves)
{
    int cnt = 0;
    int center = COLS / 2;
    if (is_valid_move(board, center))
        moves[cnt++] = center;
    for (int offset = 1; offset <= center; ++offset)
    {
        int left = center - offset;
        int right = center + offset;
        if (left >= 0 && is_valid_move(board, left))
            moves[cnt++] = left;
        if (right < COLS && is_valid_move(board, right))
            moves[cnt++] = right;
    }
    return cnt;
}

/* Minimax with alpha-beta pruning */
int minimax_ab(char **board, int depth, int alpha, int beta, int maximizingPlayer, char botPiece, char humanPiece, int *best_col_out)
{
    int moves[COLS];
    int n_moves = get_valid_moves(board, moves);

    int isTerminal = winning_move(board, botPiece) || winning_move(board, humanPiece) || (n_moves == 0);
    if (depth == 0 || isTerminal)
    {
        if (winning_move(board, botPiece))
            return 1000000;
        else if (winning_move(board, humanPiece))
            return -1000000;
        else
            return score_position(board, botPiece, humanPiece);
    }

    if (maximizingPlayer)
    {
        int value = INT_MIN;
        int best_col = moves[0];
        for (int i = 0; i < n_moves; ++i)
        {
            int col = moves[i];
            int row = get_next_open_row(board, col);
            if (row == -1)
                continue;
            make_move(board, row, col, botPiece);
            int newScore = minimax_ab(board, depth - 1, alpha, beta, 0, botPiece, humanPiece, NULL);
            undo_move(board, row, col);
            if (newScore > value)
            {
                value = newScore;
                best_col = col;
            }
            if (alpha < value)
                alpha = value;
            if (alpha >= beta)
                break;
        }
        if (best_col_out)
            *best_col_out = best_col;
        return value;
    }
    else
    {
        int value = INT_MAX;
        int best_col = moves[0];
        for (int i = 0; i < n_moves; ++i)
        {
            int col = moves[i];
            int row = get_next_open_row(board, col);
            if (row == -1)
                continue;
            make_move(board, row, col, humanPiece);
            int newScore = minimax_ab(board, depth - 1, alpha, beta, 1, botPiece, humanPiece, NULL);
            undo_move(board, row, col);
            if (newScore < value)
            {
                value = newScore;
                best_col = col;
            }
            if (beta > value)
                beta = value;
            if (alpha >= beta)
                break;
        }
        if (best_col_out)
            *best_col_out = best_col;
        return value;
    }
}

/* Public hardbot function: only takes matrix */
int hardbot(char **board)
{
    // Immediate win
    int moves[COLS];
    int n_moves = get_valid_moves(board, moves);
    for (int i = 0; i < n_moves; ++i)
    {
        int col = moves[i];
        int row = get_next_open_row(board, col);
        make_move(board, row, col, BOT);
        if (winning_move(board, BOT))
        {
            undo_move(board, row, col);
            return col;
        }
        undo_move(board, row, col);
    }

    // Block opponent
    for (int i = 0; i < n_moves; ++i)
    {
        int col = moves[i];
        int row = get_next_open_row(board, col);
        make_move(board, row, col, HUMAN);
        if (winning_move(board, HUMAN))
        {
            undo_move(board, row, col);
            return col;
        }
        undo_move(board, row, col);
    }

    int chosen = -1;
    minimax_ab(board, DEPTH, INT_MIN, INT_MAX, 1, BOT, HUMAN, &chosen);

    if (chosen == -1)
    {
        // fallback
        int center = COLS / 2;
        if (is_valid_move(board, center))
            return center;
        for (int c = 0; c < COLS; ++c)
            if (is_valid_move(board, c))
                return c;
    }

    return chosen;
}

#endif
