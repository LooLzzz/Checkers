#include "main.h"

char inputFilename[MAX_STR_LEN] = {0};
bool debugMode                  = 0;

int main(int argc, char *argv[])
{
    bool restartFlag;
    GameState state;

    // state's filename must be provided
    if (argc < 2)
    {
        printf("Usage: %s <input file> <debugMode?: {0,1}>\n", argv[0]);
        return 1;
    }
    else
    {
        strcat(inputFilename, argv[1]);
        if (argc > 2)
            sscanf(argv[2], "%d", &debugMode);
    }

    // initialize game state
    system("clear");
    if (debugMode)
        printf("<< Debug mode enabled >>\n");
    loadState(inputFilename, &state);

    while (1)
    {
        // start the game
        restartFlag = gameloop(&state);

        if (restartFlag)
        {
            // restart the game
            initializeState(&state);
            saveState(inputFilename, &state);
            continue;
        }

        // exit
        break;
    }

    if (state.winner)
        printf("%s\n",
               state.winner == PLAYER_1
                   ? str2playercolor("PLAYER1 won!", state.winner)
                   : str2playercolor("PLAYER2 won!", state.winner));
    else if (state.winner == DRAW)
        printf("Draw!\n");

    printf("Exiting..");
    return 0;
}

int gameloop(GameState *state)
{
    int argsCount = 0;
    char line[MAX_STR_LEN], rest[MAX_STR_LEN];
    char src[MAX_STR_LEN], dest[MAX_STR_LEN];
    Array possibleJumps = array_new(sizeof(Move));
    Move currentMove;

    while (state->activePlayer)
    {
        printState(state, !debugMode);

        if (state->lastMove.moveType == MOVE_NONE)
        {
            /** first loop **/
            printf("Options: [like \"B3 A4\", \"exit\", \"restart\"] \n");
            printf("Input: ");
        }
        else if (state->lastMove.moveType == MOVE_INVALID)
        {
            /** invalid move **/
            printf("%s, try again: ", state->lastMove.errorMessage);
        }
        else
        {
            /** valid move **/

            // print last move
            sprintf(line, " %c%c -> %c%c%s%s",
                    state->lastMove.src.j + 'A',
                    state->lastMove.src.i + '1',
                    state->lastMove.dest.j + 'A',
                    state->lastMove.dest.i + '1',
                    moveType2str(state->lastMove.moveType),
                    crowned2str(state->lastMove.crownPiece));
            str2playercolor(line, state->lastMove.player);
            printf("Last move: %s %s\n", line, ESC);

            // print input line
            printf("Input: ");
        }

        scanf("\n");
        fgets(line, MAX_STR_LEN, stdin);
        // str_strip(line);
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        // check for exit clause
        if (str_cmpi(line, "exit") == 0)
            return 0;

        if (str_cmpi(line, "restart") == 0)
            return 1;

        // seperate line into src and dest
        argsCount = sscanf(line, " %s %s %c", src, dest, rest);

        // validate input
        validateInput(state, argsCount, src, dest, &currentMove);

        // currentMove initialization
        currentMove.player     = state->activePlayer;
        currentMove.moveType   = MOVE_INVALID;
        currentMove.crownPiece = 0;
        currentMove.src.i      = src[1] - '1';
        currentMove.src.j      = src[0] - 'A';
        currentMove.dest.i     = dest[1] - '1';
        currentMove.dest.j     = dest[0] - 'A';
        strcpy(currentMove.errorMessage, "Invalid move");

        // validate the move and set `currentMove: {.moveType, .crownPiece}` accordingly
        fillAndValidateMove(state, &currentMove);

        // if move is valid, check for jumps
        if (currentMove.moveType)
        {
            getAllPossibleJumps(state, &possibleJumps);
            validateJump(state, &currentMove, possibleJumps);

            if (debugMode) //DEBUG
            {
                Coord c_src, c_dest;

                printf("<< number of possible jumps: %d >>\n", possibleJumps.length);
                for (int idx = 0; idx < possibleJumps.length; idx++)
                {
                    c_src  = ((Move *)array_get(&possibleJumps, idx))->src;
                    c_dest = ((Move *)array_get(&possibleJumps, idx))->dest;
                    printf("\t# %c%c -> %c%c\n",
                           c_src.j + 'A',
                           c_src.i + '1',
                           c_dest.j + 'A',
                           c_dest.i + '1');
                }
            }
            array_free(&possibleJumps);
        }

        // an invalid move, go back to start of the loop
        if (currentMove.moveType < 1)
        {
            state->lastMove = currentMove;
            continue;
        }

        // move piece
        makeMove(state, currentMove);

        // are there any more jumps available for this piece?
        if (currentMove.moveType == MOVE_JUMP)
        {
            getPossibleJumps(state, &possibleJumps, currentMove.dest.i, currentMove.dest.j);

            if (possibleJumps.length == 0)
                // if there are no more jumps available, switch player
                state->activePlayer = nextPlayer(state->activePlayer);
            array_free(&possibleJumps);
        }
        else
            state->activePlayer = nextPlayer(state->activePlayer);

        // updates `state->winner` and `state->activePlayer` accordingly
        updateWinState(state);

        state->lastMove = currentMove;
        saveState(inputFilename, state);
    }
}

void validateInput(GameState *state, int argsCount, char *src, char *dest, Move *move)
{
    // argument count
    if (argsCount != 2)
    {
        move->moveType = MOVE_INVALID;
        strcpy(move->errorMessage, "Expected two arguments");
        return;
    }

    // each argument must be a valid cell
    if (!isValidCell(src))
    {
        move->moveType = MOVE_INVALID;
        strcpy(move->errorMessage, "Invalid source cell");
        return;
    }

    if (!isValidCell(dest))
    {
        move->moveType = MOVE_INVALID;
        strcpy(move->errorMessage, "Invalid destination cell");
        return;
    }
}

/**
 * Validate the cell string given, will try to fix the string if it's not valid
 * @return 1 if the cell is valid, 0 otherwise
 */
int isValidCell(char *cell)
{
    char c;
    int len = strlen(cell);
    str_toupper(cell);

    if (len != 2)
        return 0;

    // swap cell[0]<->cell[1]
    if (isdigit(cell[0]) && isalpha(cell[1]))
    {
        c       = cell[0];
        cell[0] = cell[1];
        cell[1] = c;
    }

    // out of bounds
    if (cell[0] < 'A' || cell[0] > 'A' + BOARD_SIZE - 1)
        return 0;
    if (cell[1] < '1' || cell[1] > BOARD_SIZE + '0')
        return 0;

    return 1;
}

void fillAndValidateMove(GameState *state, Move *move)
{
    int i, j;
    int dir         = state->activePlayer == PLAYER_1 ? 1 : -1;
    int lastRow     = state->activePlayer == PLAYER_1 ? BOARD_SIZE - 1 : 0;
    Piece *p_src    = &state->board[move->src.i][move->src.j].piece;
    Piece *p_dest   = &state->board[move->dest.i][move->dest.j].piece;
    Piece *p_middle = NULL;

    // if (move->moveType == MOVE_INVALID)
    //     return;

    // are pieces valid
    if (p_src->type == PIECE_NONE ||
        p_src->player != state->activePlayer ||
        p_dest->type != PIECE_NONE)
    {
        strcpy(move->errorMessage, "Invalid piece");
        goto end;
    }
    else
    {
        i        = (move->src.i + move->dest.i) / 2;
        j        = (move->src.j + move->dest.j) / 2;
        p_middle = &state->board[i][j].piece;
    }

    // is walk valid
    if (move->src.i + dir == move->dest.i || (p_src->type == PIECE_KING && move->src.i - dir == move->dest.i))
        if (move->src.j + 1 == move->dest.j || move->src.j - 1 == move->dest.j)
        {
            move->moveType = MOVE_WALK; // valid walk
            goto end;
        }

    // is jump valid
    if (move->src.i + (2 * dir) == move->dest.i ||
        (p_src->type == PIECE_KING && move->src.i - (2 * dir) == move->dest.i))
        if (move->src.j + 2 == move->dest.j || move->src.j - 2 == move->dest.j)
            if (p_middle->type != PIECE_NONE && p_middle->player == nextPlayer(state->activePlayer))
            {
                move->moveType = MOVE_JUMP; // valid jump
                goto end;
            }

end:
    // should piece be crowned
    if (move->moveType > 0) // a valid move
        if (p_src->type != PIECE_KING && move->dest.i == lastRow)
            move->crownPiece = 1;
}

void validateJump(GameState *state, Move *move, Array possibleJumps)
{
    // make sure there are jumps to be taken
    if (possibleJumps.length > 0)
    {
        switch (move->moveType)
        {
            case MOVE_JUMP:
                // user requested a jump
                if (state->lastMove.moveType == MOVE_JUMP)
                    // if last move was a jump, make sure current one is part of the same jump chain
                    if (!(state->lastMove.dest.i == move->src.i &&
                          state->lastMove.dest.j == move->src.j))
                    {
                        move->moveType = MOVE_INVALID;
                        strcpy(move->errorMessage, "Jump must be part of the same chain");
                    }
                break;

            case MOVE_WALK:
                // user requested a walk, but there are jumps available
                move->moveType = MOVE_INVALID;
                strcpy(state->lastMove.errorMessage, "Jump must be taken if available");
                break;
        }
    }
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

void getAllPossibleJumps(GameState *state, Array *array)
{
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (state->board[i][j].piece.player == state->activePlayer)
                getPossibleJumps(state, array, i, j);
        }
}

void makeMove(GameState *state, Move move)
{
    Coord middleCoord = {.i = (move.src.i + move.dest.i) / 2,
                         .j = (move.src.j + move.dest.j) / 2};

    // move to new location
    movePiece(state->board, move.src, move.dest);

    // delete middle piece if needed
    if (move.moveType == MOVE_JUMP)
        deletePiece(state->board, middleCoord);

    // crown dest piece if needed
    if (move.crownPiece)
        crownPiece(state->board, move.dest);
}

void updateWinState(GameState *state)
{
    Array p1 = array_new(sizeof(Coord)); // PLAYER_1 pieces
    Array p2 = array_new(sizeof(Coord)); // PLAYER_2 pieces
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
