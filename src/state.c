#include "state.h"

extern bool debugMode;

void printState(GameState *state)
{
    int i, j;
    char str[MAX_PIECE_SIZE];

    // clear terminal before printing the next state
    if (!debugMode)
        system("clear");

    // print active player
    printf("\n%12s", ""); // whitespace padding
    player2str(state->activePlayer, str);
    printf("%s's turn %s\n", str, ESC);

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
            tile2str(state->board[i][j].tile, str);
            printf("%s", str);
            piece2str(state->board[i][j].piece, str);
            printf("%s %s", str, ESC);
        }
        printf("\n");
    }
    printf("\n");
}

void initializeState(GameState *state)
{
    bool is_even;
    int i, j;

    // initialize basic info
    state->activePlayer      = PLAYER_1;
    state->winner            = PLAYER_NONE;
    state->lastMove.moveType = MOVE_NONE;

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
    FILE *fp = fopen(fileName, "wb");

    fwrite(state, sizeof(GameState), 1, fp);
    fclose(fp);
}

void loadState(char *fileName, GameState *state, bool askContinue)
{
    char line[MAX_STR_LEN];
    FILE *fp = fopen(fileName, "rb");

    if (fp)
    {
        //file exists
        if (askContinue)
        {
            printf("Continue existing game? [Y/N]: ");
            while (1)
            {
                // scanf("%s", line);
                fgets(line, MAX_STR_LEN, stdin);
                line[0] = toupper(line[0]);
                if (line[0] == 'Y' || line[0] == 'N')
                    break;
                printf("[Y/N]: ");
            }
        }
        else
            line[0] = 'Y';

        if (line[0] == 'Y')
        {
            fread(state, sizeof(GameState), 1, fp);
            fclose(fp);
            return;
        }
    }
    // else

    // file doesn't exist or user wants to start a new game
    initializeState(state);
    saveState(fileName, state);
}

void deletePiece(Cell board[][BOARD_SIZE], Coord src)
{
    board[src.i][src.j].piece.player = PLAYER_NONE;
    board[src.i][src.j].piece.type   = PIECE_NONE;
}

void movePiece(Cell board[][BOARD_SIZE], Coord src, Coord dest)
{
    board[dest.i][dest.j].piece = board[src.i][src.j].piece;
    deletePiece(board, src);
}

void crownPiece(Cell board[][BOARD_SIZE], Coord src)
{
    board[src.i][src.j].piece.type = PIECE_KING;
}

void getPossibleJumps(GameState *state, Array *array, int i, int j)
{
    int k;
    Move move;
    char dest[3]     = {0};
    int offsets[][2] = {
        {-2, -2},
        {-2, 2},
        {2, -2},
        {2, 2},
    };

    move.player = state->activePlayer;
    move.src.i  = i;
    move.src.j  = j;

    for (k = 0; k < 4; k++)
    {
        move.crownPiece = 0;
        move.moveType   = MOVE_INVALID;
        move.dest.i     = i + offsets[k][0];
        move.dest.j     = j + offsets[k][1];
        strcpy(move.errorMessage, "Invalid move");

        coord2str(dest, move.dest.i, move.dest.j);
        if (isValidCell(dest))
        {
            fillAndValidateMove(state, &move);

            if (debugMode) //DEBUG
            {
                char src[3] = {0};
                coord2str(src, i, j);
                printf("%s -> %s:\n", src, dest);
                printf("\tmoveType = %d\n", move.moveType);
                printf("\tmsg = %s\n", move.errorMessage);
            }

            if (move.moveType == MOVE_JUMP)
                array_push(array, &move);
        }
    }
}

void getAllPossibleJumps(GameState *state, Array *moves)
{
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (state->board[i][j].piece.player == state->activePlayer)
                getPossibleJumps(state, moves, i, j);
        }
}

void makeMove(GameState *state, Move move)
{
    Array possibleJumps = array_init(sizeof(Move));
    Coord middleCoord   = {.i = (move.src.i + move.dest.i) / 2,
                         .j = (move.src.j + move.dest.j) / 2};

    // move to new location
    movePiece(state->board, move.src, move.dest);

    // crown dest piece if needed
    if (move.crownPiece)
        crownPiece(state->board, move.dest);

    if (move.moveType == MOVE_JUMP)
    {
        // delete captured piece from the board
        deletePiece(state->board, middleCoord);

        // are there any more jumps available for this piece?
        getPossibleJumps(state, &possibleJumps, move.dest.i, move.dest.j);

        if (possibleJumps.length == 0)
            // if there are no more jumps available, switch player
            state->activePlayer = nextPlayer(state->activePlayer);

        array_free(&possibleJumps);
    }
    else
        state->activePlayer = nextPlayer(state->activePlayer);

    // updates `state->winner` and `state->activePlayer` accordingly
    updateWinState(state);
}

void updateWinState(GameState *state)
{
    Array p1 = array_init(sizeof(Coord)); // PLAYER_1 pieces
    Array p2 = array_init(sizeof(Coord)); // PLAYER_2 pieces
    Coord coord;
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
        {
            coord.i = i;
            coord.j = j;
            switch (state->board[i][j].piece.player)
            {
                case PLAYER_1:
                    array_push(&p1, &coord);
                    break;

                case PLAYER_2:
                    array_push(&p2, &coord);
                    break;
            }
        }

    // if (p1.length == 0 && p2.length == 0)
    //     state->winner = DRAW;
    if (p1.length == 0)
        state->winner = PLAYER_2;
    else if (p2.length == 0)
        state->winner = PLAYER_1;
    else
        state->winner = PLAYER_NONE;

    // end game if there is a winner
    if (state->winner != PLAYER_NONE)
        state->activePlayer = PLAYER_NONE;

    array_free(&p1);
    array_free(&p2);
}
