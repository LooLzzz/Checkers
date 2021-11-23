#include "validation.h"


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
                if (state->lastMove.player == state->activePlayer && state->lastMove.moveType == MOVE_JUMP)
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
