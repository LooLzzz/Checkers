#include "main.h"

char inputFilename[MAX_STR_LEN] = {0};

int main(int argc, char *argv[])
{
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
    loadState(inputFilename, &state);

    // start game
    gameloop(&state);

    return 0;
}

Move validateInput(GameState *state, char *src, char *dest)
{
    int letterSrc       = toupper(src[0]) - 'A';
    int letterDest      = toupper(dest[0]) - 'A';
    int numSrc          = src[1] - '1';
    int numDest         = dest[1] - '1';
    PLAYER activePlayer = state->activePlayer;
    int dir             = activePlayer == PLAYER_1 ? 1 : -1;
    Move move           = {
        .moveType    = MOVE_INVALID,
        .crownPiece = 0,
        .src         = &state->board[numSrc][letterSrc].piece,
        .dest        = &state->board[numDest][letterDest].piece,
        .middlePiece = NULL,
    };

    // out of bounds
    if (!(letterSrc >= 0 || letterSrc < BOARD_SIZE ||
          letterDest >= 0 || letterDest < BOARD_SIZE ||
          numSrc >= 0 || numSrc < BOARD_SIZE ||
          numDest >= 0 || numDest < BOARD_SIZE))
        goto ret;

    // valid pieces
    if (move.src->type == PIECE_NONE ||
        move.src->player != activePlayer ||
        move.dest->type != PIECE_NONE)
        goto ret;

    // valid walk
    if ((numSrc + dir == numDest || (move.src->type == PIECE_KING && numSrc - dir == numDest)) &&
        (letterSrc + 1 == letterDest || letterSrc - 1 == letterDest))
    {
        move.moveType = MOVE_WALK; // valid walk
        goto ret;
    }

    // valid jump
    move.middlePiece = &state->board[(numSrc + numDest) / 2][(letterSrc + letterDest) / 2].piece;
    if ((numSrc + (2 * dir) == numDest || (move.src->type == PIECE_KING && numSrc - 2 * dir == numDest)) &&
        move.middlePiece->player != PLAYER_NONE &&
        move.middlePiece->player != activePlayer &&
        (letterSrc + 2 == letterDest || letterSrc - 2 == letterDest))
        move.moveType = MOVE_JUMP; // valid jump
    
    // Check reached end:
    // Player_1: numDest == BOARD_SIZE - 1
    // Player_2: numDest == 0
    // if (move.moveType != MOVE_INVALID && )

ret:
    return move;
}

void movePiece(Move move)
{
    //TODO: add support for player to eat more than one piece in a row
    //TODO: add support for pieces turning to kings

    printf("%s\n", move.moveType == MOVE_WALK ? "WALK" : "JUMP");

    move.dest->type   = move.src->type;
    move.dest->player = move.src->player;
    move.src->type    = PIECE_NONE;
    move.src->player  = PLAYER_NONE;

    if (move.moveType == MOVE_JUMP)
    {
        move.middlePiece->type   = PIECE_NONE;
        move.middlePiece->player = PLAYER_NONE;
    }
}

void gameloop(GameState *state)
{
    int args = -1;
    char src[MAX_STR_LEN], dest[MAX_STR_LEN];
    Move move;

    while (1)
    {
    start:
        printState(state);

        switch (args)
        {
            case -1:
                printf("Input (e.g \"B3 A4\"): ");
                break;
            case -2:
                printf("Input: ");
                break;
            default:
                printf("Invalid move, try again: ");
                break;
        }
        args = scanf("%s %s", src, dest);

        // TODO: check for exit clause in user's input

        move = validateInput(state, src, dest);
        if (move.moveType != MOVE_INVALID)
        {
            args = -2;
            movePiece(move);
            state->activePlayer = state->activePlayer == PLAYER_1 ? PLAYER_2 : PLAYER_1;
            saveState(inputFilename, state);
        }
        else
            goto start;
    }
}
