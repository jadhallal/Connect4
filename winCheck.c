#include "winCheck.h"

void *check_row(void *arg)
{
    Threadargs *a = (Threadargs *)arg;
    int count = 0;
    for (int i = 0; i < COLS; i++)
    {
        if (a->matrix[a->R][i] == a->player)
            count++;
        else
            count = 0;
        if (count == 4)
        {
            a->result = 1;
            return NULL;
        }
    }
    return NULL;
}

void *check_column(void *arg)
{
    Threadargs *a = (Threadargs *)arg;
    int count = 0;
    for (int i = 0; i < ROWS; i++)
    {
        if (a->matrix[i][a->C] == a->player)
            count++;
        else
            count = 0;
        if (count == 4)
        {
            a->result = 1;
            return NULL;
        }
    }
    return NULL;
}

void *check_diag_down(void *arg)
{
    Threadargs *a = (Threadargs *)arg;
    int count = 0;

    int r = a->R, c = a->C;
    while (r > 0 && c > 0)
    {
        r--;
        c--;
    }

    while (r < ROWS && c < COLS)
    {
        if (a->matrix[r][c] == a->player)
            count++;
        else
            count = 0;
        if (count == 4)
        {
            a->result = 1;
            return NULL;
        }
        r++;
        c++;
    }
    return NULL;
}

void *check_diag_up(void *arg)
{
    Threadargs *a = (Threadargs *)arg;
    int count = 0;

    int r = a->R, c = a->C;
    while (r < ROWS - 1 && c > 0)
    {
        r++;
        c--;
    }

    while (r >= 0 && c < COLS)
    {
        if (a->matrix[r][c] == a->player)
            count++;
        else
            count = 0;
        if (count == 4)
        {
            a->result = 1;
            return NULL;
        }
        r--;
        c++;
    }
    return NULL;
}

int winCheck(int R, int C, char player, char **matrix)
{
    C--;
    pthread_t threads[4];
    Threadargs args[4];

    for (int i = 0; i < 4; i++)
    {
        args[i].R = R;
        args[i].C = C;
        args[i].player = player;
        args[i].matrix = matrix;
        args[i].result = 0;
    }

    pthread_create(&threads[0], NULL, check_row, &args[0]);
    pthread_create(&threads[1], NULL, check_column, &args[1]);
    pthread_create(&threads[2], NULL, check_diag_down, &args[2]);
    pthread_create(&threads[3], NULL, check_diag_up, &args[3]);

    for (int i = 0; i < 4; i++)
        pthread_join(threads[i], NULL);

    return args[0].result ||
           args[1].result ||
           args[2].result ||
           args[3].result;
}
