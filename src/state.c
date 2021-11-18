#include "state.h"

void printState(GameState *state)
{
    int i, j;
    char *fg, *bg;
    Cell cell;

    // clear console
    system("clear");

    //TODO: add board state variable
    printf("%11c%s's turn\n", 0, "WHITE");

    // print letter on the top
    printf("\n  ");
    for (i = 0; i < 8; i++)
        printf("%2c  ", 'A' + i);
    printf("\n");

    // print tiles
    for (i = 0; i < 8; i++)
    {
        printf("%i ", i + 1); // print numbers on the left
        for (j = 0; j < 8; j++)
        {
            cell = state->board[i][j];
            fg   = char2color(cell.fg, 0);
            bg   = char2color(cell.bg, 1);

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

            if (i < (BOARD_SIZE / 2) - 1 && is_even)
            {
                strcpy(state->board[i][j].piece, "⬤");
                state->board[i][j].fg = 'B';
            }
            else if (i > BOARD_SIZE / 2 && is_even)
            {
                strcpy(state->board[i][j].piece, "⬤");
                state->board[i][j].fg = 'R';
            }
            else
            {
                // empty cell
                strcpy(state->board[i][j].piece, " ");
                state->board[i][j].fg = 0;
            }
            state->board[i][j].bg = is_even ? 'R' : 'W';
        }

    state->activeTurn = 0;
    state->winner     = 0;
}

void saveState(char *fileName, GameState *state)
{
    int i, j;
    FILE *fp = fopen(fileName, "wb");

    fwrite(&state->activeTurn, sizeof(int), 1, fp);
    fwrite(&state->winner, sizeof(int), 1, fp);

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            fwrite(&state->board[i][j], sizeof(Cell), 1, fp);

    fclose(fp);
}

void readStateFromFile(FILE *fp, GameState *state)
{
    int i, j;
    fread(&state->activeTurn, sizeof(int), 1, fp);
    fread(&state->winner, sizeof(int), 1, fp);

    // Read All Cells
    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            fread(&state->board[i][j], sizeof(Cell), 1, fp);
}

void loadState(char *fileName, GameState *state)
{
    FILE *fp = fopen(fileName, "rb");
    if (fp)
    {
        readStateFromFile(fp, state);
        fclose(fp);
    }
    else
    {
        initializeState(state);
        saveState(fileName, state);
    }
}
