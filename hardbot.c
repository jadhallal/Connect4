#include "hardbot.h"

/* -------------------- Basic utils -------------------- */
int is_valid_move(char **board, int col)
{
    if (col < 0 || col >= COLS)
        return 0;

    return (board[0][col] == '.');
}

int get_next_open_row(char **board, int col)
{
    for (int r = ROWS - 1; r >= 0; --r)
        if (board[r][col] == '.')
            return r;

    return -1;
}

void make_move(char **board, int row, int col, char piece)
{
    board[row][col] = piece;
}

void undo_move(char **board, int row, int col)
{
    board[row][col] = '.';
}

/* -------------------- Win detection -------------------- */

int winning_move(char **board, char piece)
{
    /* Horizontal */
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
            if (board[r][c] == piece &&
                board[r][c + 1] == piece &&
                board[r][c + 2] == piece &&
                board[r][c + 3] == piece)
                return 1;

    /* Vertical */
    for (int c = 0; c < COLS; ++c)
        for (int r = 0; r < ROWS - 3; ++r)
            if (board[r][c] == piece &&
                board[r + 1][c] == piece &&
                board[r + 2][c] == piece &&
                board[r + 3][c] == piece)
                return 1;

    /* Diagonal / */
    for (int r = 3; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
            if (board[r][c] == piece &&
                board[r - 1][c + 1] == piece &&
                board[r - 2][c + 2] == piece &&
                board[r - 3][c + 3] == piece)

                return 1;

    /* Diagonal \ */
    for (int r = 0; r < ROWS - 3; ++r)
        for (int c = 0; c < COLS - 3; ++c)
            if (board[r][c] == piece &&
                board[r + 1][c + 1] == piece &&
                board[r + 2][c + 2] == piece &&
                board[r + 3][c + 3] == piece)
                return 1;

    return 0;
}

/* -------------------- Evaluation helpers -------------------- */
static void evaluate_window(
    char window[4],
    char me,
    char opp,
    int *myScore,
    int *oppThreat)
{
    int countMe = 0, countOpp = 0, countEmpty = 0;

    for (int i = 0; i < 4; ++i)
    {
        if (window[i] == me)
            countMe++;
        else if (window[i] == opp)
            countOpp++;
        else if (window[i] == '.')
            countEmpty++;
    }

    if (countMe == 4)
        *myScore += 10000;
    else if (countMe == 3 && countEmpty == 1)
        *myScore += 120;
    else if (countMe == 2 && countEmpty == 2)
        *myScore += 20;

    if (countOpp == 3 && countEmpty == 1)
        *oppThreat += 150;
    else if (countOpp == 2 && countEmpty == 2)
        *oppThreat += 25;
}

/* Horizontal window with openness context */
static void eval_horizontal_segment(
    char **board,
    int r,
    int c,
    char me,
    char opp,
    int *myScore,
    int *oppThreat)
{
    char window[4];
    for (int i = 0; i < 4; ++i)
        window[i] = board[r][c + i];

    int countMe = 0, countOpp = 0, countEmpty = 0;

    for (int i = 0; i < 4; ++i)
    {
        if (window[i] == me)
            countMe++;
        else if (window[i] == opp)
            countOpp++;
        else if (window[i] == '.')
            countEmpty++;
    }

    int left_empty = (c > 0 && board[r][c - 1] == '.');
    int right_empty = (c + 4 < COLS && board[r][c + 4] == '.');
    int open_ends = left_empty + right_empty;

    if (countMe == 4)
        *myScore += 10000;

    else if (countMe == 3 && countEmpty == 1)
    {
        if (open_ends == 2)
            *myScore += 200;
        else if (open_ends == 1)
            *myScore += 120;
        else
            *myScore += 80;
    }

    else if (countMe == 2 && countEmpty == 2)
    {
        if (open_ends == 2)
            *myScore += 40;
        else if (open_ends == 1)
            *myScore += 25;
        else
            *myScore += 10;
    }

    if (countOpp == 3 && countEmpty == 1)
    {
        if (open_ends == 2)
            *oppThreat += 220;
        else if (open_ends == 1)
            *oppThreat += 150;
        else
            *oppThreat += 80;
    }
    else if (countOpp == 2 && countEmpty == 2)
    {
        if (open_ends == 2)
            *oppThreat += 40;
        else if (open_ends == 1)
            *oppThreat += 25;
        else
            *oppThreat += 10;
    }
}

/* Heuristic score */
int score_position(char **board, char me, char opp)
{
    int score = 0;
    int oppThreat = 0;
    int center_col = COLS / 2;
    int centerCount = 0;

    for (int r = 0; r < ROWS; ++r)
        if (board[r][center_col] == me)
            centerCount++;

    score += centerCount * 8;

    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
            eval_horizontal_segment(board, r, c, me, opp, &score, &oppThreat);

    for (int c = 0; c < COLS; ++c)
        for (int r = 0; r < ROWS - 3; ++r)
        {
            char window[4];
            for (int i = 0; i < 4; ++i)
                window[i] = board[r + i][c];

            evaluate_window(window, me, opp, &score, &oppThreat);
        }

    for (int r = 0; r < ROWS - 3; ++r)
        for (int c = 0; c < COLS - 3; ++c)
        {
            char window[4];
            for (int i = 0; i < 4; ++i)
                window[i] = board[r + i][c + i];

            evaluate_window(window, me, opp, &score, &oppThreat);
        }

    for (int r = 3; r < ROWS; ++r)
        for (int c = 0; c < COLS - 3; ++c)
        {
            char window[4];
            for (int i = 0; i < 4; ++i)
                window[i] = board[r - i][c + i];

            evaluate_window(window, me, opp, &score, &oppThreat);
        }

    score -= oppThreat * 2;
    return score;
}

/* -------------------- Move ordering -------------------- */
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

/* -------------------- Minimax with alpha-beta -------------------- */
int minimax_ab(
    char **board,
    int depth,
    int alpha,
    int beta,
    int maximizingPlayer,
    char botPiece,
    char humanPiece,
    int *best_col_out)

{
    int moves[COLS];
    int n_moves = get_valid_moves(board, moves);
    int botWin = winning_move(board, botPiece);
    int humanWin = winning_move(board, humanPiece);
    int isTerminal = botWin || humanWin || (n_moves == 0);

    if (depth == 0 || isTerminal)
    {
        if (botWin)
            return 1000000 + depth;
        else if (humanWin)
            return -1000000 - depth;
        else
            return score_position(board, botPiece, humanPiece);
    }

    int colOrder[COLS];
    int scores[COLS];

    for (int i = 0; i < n_moves; ++i)
    {
        colOrder[i] = moves[i];
        int c = moves[i];
        int h = 0;
        if (c == COLS / 2)
            h += 3;
        if (c == COLS / 2 - 1 || c == COLS / 2 + 1)
            h += 2;
        scores[i] = h;
    }

    for (int i = 0; i < n_moves - 1; ++i)
        for (int j = i + 1; j < n_moves; ++j)
            if (scores[j] > scores[i])
            {
                int tc = colOrder[i];
                colOrder[i] = colOrder[j];
                colOrder[j] = tc;

                int ts = scores[i];
                scores[i] = scores[j];
                scores[j] = ts;
            }

    if (maximizingPlayer)
    {
        int value = INT_MIN;
        int best_col = colOrder[0];

        for (int i = 0; i < n_moves; ++i)
        {
            int col = colOrder[i];
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
        int best_col = colOrder[0];

        for (int i = 0; i < n_moves; ++i)
        {
            int col = colOrder[i];
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

/* -------------------- Public entry: hardbot -------------------- */
int hardbot(char **board)
{
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
        int center = COLS / 2;

        if (is_valid_move(board, center))
            return center;

        for (int c = 0; c < COLS; ++c)
            if (is_valid_move(board, c))
                return c;
    }

    return chosen;
}
