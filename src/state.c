#include "state.h"

void printState(GameState *state)
{
    int i, j;
    Cell cell;

    // clear console
    system("clear");

    //TODO: add board state variable
    printf("%11c%s's turn\n", '\0', "WHITE");

    // print letter on the top
    printf("\n  ");
    for (i = 0; i < 8; i++)
        printf("%2c  ", 'A' + i);
    printf("\n");

    for (int i = 0; i < 8; i++)
    {
        printf("%i ", i + 1); // print numbers on the left
        for (int j = 0; j < 8; j++)
        {
            cell     = state->board[i][j];
            char *fg = char2color(cell.fg, 0);
            char *bg = char2color(cell.bg, 1);

            printf("%s%s %s %s", fg, bg, cell.piece, ESC);
        }
        printf("\n");
    }
    printf("\n");
}

void initializeState(GameState *state)
{
    bool is_even;
    int i, j;

    // initialize the board with empty cells
    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
        {
            is_even = (i + j) % 2;

            if (i < 3 && is_even)
            {
                state->board[i][j].piece = "⬤";
                state->board[i][j].fg    = 'B';
            }
            else if (i > BOARD_SIZE - 4 && is_even)
            {
                state->board[i][j].piece = "⬤";
                state->board[i][j].fg    = 'R';
            }
            else
            {
                // empty cell
                state->board[i][j].piece = " ";
                state->board[i][j].fg    = 0;
            }
            state->board[i][j].bg = is_even ? 'R' : 'W';
        }
}

void saveState(char *fileName, GameState *state)
{
    //TODO
}

void loadState(char *fileName, GameState *state)
{
    //TODO
    initializeState(state);
}
