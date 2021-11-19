#include "state.h"

void printState(GameState *state)
{
    int i, j;

    // clear console
    system("clear");

    // print active player
    printf("\n%12c%s's turn\n", 0, player2str(state->activePlayer));

    // print letter on the top
    printf("\n%3s", "");
    for (i = 0; i < BOARD_SIZE; i++)
        printf("%2c%2s", 'A' + i, "");
    printf("\n");

    // print pieces, tiles and numbers
    for (i = 0; i < BOARD_SIZE; i++)
    {
        // print numbers on the left
        printf("%2i ", i + 1);

        // print pieces and tiles
        for (j = 0; j < BOARD_SIZE; j++)
        {
            printf("%s%s%s",
                   tile2str(state->board[i][j].tile),
                   piece2str(state->board[i][j].piece),
                   getEsc());
        }
        printf("\n");
    }
    printf("\n");
}

void initializeState(GameState *state)
{
    char is_even;
    int i, j;

    // initialize basic info
    state->activePlayer = PLAYER_1;

    // initialize cells
    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
        {
            is_even = (i + j) % 2;

            // init cell as empty
            state->board[i][j].piece.type   = PIECE_NONE;
            state->board[i][j].piece.player = PLAYER_NONE;
            state->board[i][j].tile         = is_even ? TILE_BLACK : TILE_WHITE; // black or white

            if (i < PIECE_ROWS_AMOUNT && is_even)
            {
                // PLAYER_1's pieces
                state->board[i][j].piece.type   = PIECE_MAN;
                state->board[i][j].piece.player = PLAYER_1;
            }
            else if (i >= BOARD_SIZE - PIECE_ROWS_AMOUNT && is_even)
            {
                // PLAYER_2's pieces
                state->board[i][j].piece.type   = PIECE_MAN;
                state->board[i][j].piece.player = PLAYER_2;
            }
        }
}

void saveState(char *fileName, GameState *state)
{
    int i, j;
    FILE *fp = fopen(fileName, "wb");

    fwrite(&state->activePlayer, sizeof(int), 1, fp);
    fwrite(&state->winner, sizeof(int), 1, fp);

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            fwrite(&state->board[i][j], sizeof(Cell), 1, fp);

    fclose(fp);
}

void readStateFromFile(FILE *fp, GameState *state)
{
    int i, j;

    fread(&state->activePlayer, sizeof(int), 1, fp);
    fread(&state->winner, sizeof(int), 1, fp);

    // Read All Cells
    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            fread(&state->board[i][j], sizeof(Cell), 1, fp);
}

void loadState(char *fileName, GameState *state)
{
    int i;
    char c;
    FILE *fp = fopen(fileName, "rb");

    if (fp)
    {
        //file exists
        printf("Continue existing game? [Y/N]: ");
        do
        {
            scanf("%c", &c);
            c = toupper(c);
        } while (c != 'Y' && c != 'N');

        if (c == 'Y')
        {
            readStateFromFile(fp, state);
            fclose(fp);
            return;
        }
    }

    // file doesn't exist or user wants to start a new game
    initializeState(state);
    saveState(fileName, state);
}
