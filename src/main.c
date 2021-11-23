#include "main.h"

char inputFilename[MAX_STR_LEN] = {0};
GAMEMODE gamemode               = GAME_SP;
PLAYER self                     = PLAYER_NONE;

bool debugMode = false;

int main(int argc, char *argv[])
{
    char line[MAX_STR_LEN];
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

    // ask player for singleplayer or multiplayer mode
    printf("[S]ingleplayer or [M]ultiplayer?: ");
    while (1)
    {
        fgets(line, MAX_STR_LEN, stdin);
        line[0] = toupper(line[0]);
        if (line[0] == 'S' || line[0] == 'M')
        {
            gamemode = (line[0] == 'M') ? GAME_MP : GAME_SP;
            break;
        }
        printf("[SP/MP]: ");
    }

    // ask player for player number
    if (gamemode == GAME_MP)
    {
        printf("Choose player number [1/2]: ");
        while (1)
        {
            fgets(line, MAX_STR_LEN, stdin);
            line[0] = toupper(line[0]);
            if (line[0] == '1' || line[0] == '2')
            {
                self = (line[0] == '1') ? PLAYER_1 : PLAYER_2;
                break;
            }
            printf("[1/2]: ");
        }
    }

    // initialize game state
    system("clear");
    if (debugMode)
        printf("<< Debug mode enabled >>\n");
    loadState(inputFilename, &state, true);

    // main gameloop
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
    gameloop_start:
        printState(state);

        if (state->lastMove.moveType == MOVE_NONE)
        {
            /** first loop **/
            printf("Options: [like \"B3 A4\", \"exit\", \"restart\"] \n");
        }
        else if (state->lastMove.moveType == MOVE_INVALID)
        {
            /** invalid move **/
            printf("%s, try again\n", state->lastMove.errorMessage);
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
        }

        if (gamemode == GAME_MP)
            while (state->activePlayer != self)
            {
                printf("<< Waiting for other player's input >>\n");
                loadState(inputFilename, state, false);
                msleep(500);
                goto gameloop_start;
            }

        // print input line
        printf("Input: ");

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
