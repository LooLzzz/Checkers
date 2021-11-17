#include "state.h"

#define MAX_PIECE_SIZE 8

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
    
    state->activeTurn = 0;
    state->winner = 0;
}

void writeCellToFile(FILE *fp, Cell cell)
{
     fwrite(&cell.fg, sizeof(char), 1, fp);
     fwrite(&cell.bg, sizeof(char), 1, fp);
     fwrite(cell.piece, sizeof(char), MAX_PIECE_SIZE, fp);
}

void readCellFromFile(FILE *fp, Cell *cell)
{
    //Allocate memory for the piece
    cell->piece = (char*) malloc(sizeof(char) * MAX_PIECE_SIZE);
    if (cell->piece == NULL)
    {
        printf("Error allocating memory\n");
        exit(1);
    }


    fread(&(cell->fg), sizeof(char), 1, fp);
    fread(&(cell->bg), sizeof(char), 1, fp);
    fread(cell->piece, sizeof(char), MAX_PIECE_SIZE, fp);
}

void saveState(char *fileName, GameState *state)
{
    FILE *fp = fopen(fileName, "wb");
    fwrite(&(state->activeTurn), sizeof(int), 1, fp);
    fwrite(&(state->winner), sizeof(int), 1, fp);

    for(int row = 0; row < BOARD_SIZE; row++)
    {
        for(int col = 0; col < BOARD_SIZE; col++)
        {
            writeCellToFile(fp, state->board[row][col]);
        }
    }
    fclose(fp);
}

void readStateFromFile(FILE *fp, GameState *state)
{
    fread(&(state->activeTurn), sizeof(int), 1, fp);
    fread(&(state->winner), sizeof(int), 1, fp);

    // Read All Cells
    for(int row = 0; row < BOARD_SIZE; row++)
    {
        for(int col = 0; col < BOARD_SIZE; col++)
        {
            readCellFromFile(fp, &state->board[row][col]);
        }
    }
}

void loadState(char *fileName, GameState *state)
{
    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL)
    {
        printf("Error: file not found so initializing\n");
        initializeState(state);
        saveState(fileName, state);
    }
    else
    {
        readStateFromFile(fp, state);
        fclose(fp);
    }
    
}
