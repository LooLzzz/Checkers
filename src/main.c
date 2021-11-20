#include "main.h"

char inputFilename[MAX_STR_LEN] = {0};

int main(int argc, char *argv[])
{
    bool restartFlag;
    GameState state;

    // state's filename must be provided
    if (argc == 2)
    {
        int idx = strrchr(argv[0], '/') - argv[0];
        if (idx > 0)
            memcpy(inputFilename, argv[0], sizeof(char) * ++idx);
        strcat(inputFilename, argv[1]);
    }
    else
    {
        printf("Usage: %s <input file>\n", "./checkers");
        return 1;
    }

    // initialize game state
    system("clear");
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

    printf("Bye Bye..");
    return 0;
}

int gameloop(GameState *state)
{
    int argsCount = 0;
    char line[MAX_STR_LEN], rest[MAX_STR_LEN];
    char src[MAX_STR_LEN], dest[MAX_STR_LEN];

    while (1)
    {
        // printState(state, 0); // dont clear terminal
        printState(state, 1); // clear terminal

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
            str2playercolor(line, nextPlayer(state->activePlayer));
            printf("Last move: %s%s\n", line, ESC);

            // print input line
            printf("Input: ");
        }

        scanf("\n");
        gets(line);
        // fgets(line, MAX_STR_LEN, stdin);
        // scanf("%s[^\n]", line);

        // check for exit clause
        if (_strcmpi(line, "exit") == 0)
            return 0;

        if (_strcmpi(line, "restart") == 0)
            return 1;

        // seperate line into src and dest
        argsCount = sscanf(line, " %s %s %c", src, dest, rest);

        // validate input
        state->lastMove.moveType = MOVE_NONE;
        validateInput(state, argsCount, src, dest);
        validateMove(state, src, dest);

        if (state->lastMove.moveType <= 0) // an invalid move
            continue;

        // move piece
        makeMove(state);
        state->activePlayer = nextPlayer(state->activePlayer);
        saveState(inputFilename, state);
    }
}

void validateInput(GameState *state, int argsCount, char *src, char *dest)
{
    // argument count
    if (argsCount != 2)
    {
        state->lastMove.moveType = MOVE_INVALID;
        strcpy(state->lastMove.errorMessage, "Expected two arguments");
        return;
    }

    // each argument must be a valid cell
    if (!isValidCell(src))
    {
        state->lastMove.moveType = MOVE_INVALID;
        strcpy(state->lastMove.errorMessage, "Invalid source cell");
        return;
    }

    if (!isValidCell(dest))
    {
        state->lastMove.moveType = MOVE_INVALID;
        strcpy(state->lastMove.errorMessage, "Invalid destination cell");
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

void validateMove(GameState *state, char *src, char *dest)
{
    int letterSrc  = src[0] - 'A';
    int letterDest = dest[0] - 'A';
    int numSrc     = src[1] - '1';
    int numDest    = dest[1] - '1';

    PLAYER activePlayer = state->activePlayer;
    int dir             = activePlayer == PLAYER_1 ? 1 : -1;
    int lastRow         = activePlayer == PLAYER_1 ? BOARD_SIZE - 1 : 0;
    Piece *p_src        = &state->board[numSrc][letterSrc].piece;
    Piece *p_dest       = &state->board[numDest][letterDest].piece;
    Piece *p_middle     = NULL;
    Move move           = {
        .moveType     = MOVE_INVALID,
        .crownPiece   = 0,
        .src          = {numSrc, letterSrc},
        .dest         = {numDest, letterDest},
        .middle       = {(numSrc + numDest) / 2, (letterSrc + letterDest) / 2},
        .errorMessage = "Invalid move",
    };

    if (state->lastMove.moveType == MOVE_INVALID)
        return;

    // are pieces valid
    if (p_src->type == PIECE_NONE ||
        p_src->player != activePlayer ||
        p_dest->type != PIECE_NONE)
    {
        strcpy(move.errorMessage, "Invalid piece");
        goto end;
    }

    // is walk valid
    if (move.src.i + dir == move.dest.i || (p_src->type == PIECE_KING && move.src.i - dir == move.dest.i))
        if (move.src.j + 1 == move.dest.j || move.src.j - 1 == move.dest.j)
        {
            move.moveType = MOVE_WALK; // valid walk
            goto end;
        }

    // is jump valid
    p_middle = &state->board[(numSrc + numDest) / 2][(letterSrc + letterDest) / 2].piece;
    if (move.src.i + (2 * dir) == move.dest.i ||
        (p_src->type == PIECE_KING && move.src.i - 2 * dir == move.dest.i))
        if (p_middle->player != PLAYER_NONE && p_middle->player != activePlayer)
            if (move.src.j + 2 == move.dest.j || move.src.j - 2 == move.dest.j)
            {
                printf("2\n");
                move.moveType = MOVE_JUMP; // valid jump
                goto end;
            }

end:
    // should piece be crowned
    if (move.moveType > 0) // a valid move
        if (p_src->type != PIECE_KING && move.dest.i == lastRow)
            move.crownPiece = 1;

    state->lastMove = move;
}

void makeMove(GameState *state)
{
    //TODO: add support for player to eat more than one piece in a row

    Move move = state->lastMove;

    // move to new location
    movePiece(state->board, move.src, move.dest);

    // delete middle piece if needed
    if (move.moveType == MOVE_JUMP)
        deletePiece(state->board, move.middle);

    // crown dest piece if needed
    if (move.crownPiece)
        crownPiece(state->board, move.dest);
}
