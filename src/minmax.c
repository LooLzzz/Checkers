#include "minmax.h"

extern bool debugMode;

NEGAMAX_RETURN negamax(Hashmap *map, int depth, GameState currentState, int alpha, int beta)
{
    int i;
    int bestScore = -INT_MAX;
    Move *currentMove, bestMove;
    Array moves;
    GameState nextState;
    NEGAMAX_RETURN res, *hashmap_res;
    uint64_t currentState_key = zobrist_hash(&currentState, depth);

    hashmap_res = (NEGAMAX_RETURN *)hashmap_get(map, currentState_key);
    if (hashmap_res)
        return *hashmap_res;

    if (depth == 0 || currentState.winner)
    {
        res.score = evaluateState(currentState);
        hashmap_res = (NEGAMAX_RETURN *)malloc(sizeof(NEGAMAX_RETURN));
        *hashmap_res = res;
        hashmap_put(map, currentState_key, hashmap_res);
        return res;
    }

    moves = array_init(sizeof(Move));
    getAllMoves(currentState, &moves);

    // printf("\tnum of possible moves: %d\n", moves.length); // DEBUG

    for (i = 0; i < moves.length; i++)
    {
        currentMove = (Move *)array_get(&moves, i);

        memcpy(&nextState, &currentState, sizeof(GameState));
        makeMove(&nextState, *currentMove);

        // score = -negamax(-value)
        res = negamax(map, depth - 1, nextState, -beta, -alpha);
        res.score *= -1;
        res.move = *currentMove;

        if (res.score > bestScore)
        {
            bestScore = res.score;
            bestMove  = *currentMove;
        }

        alpha = max(alpha, res.score);
        if (alpha > beta) // cutoff
            break;
    }

    res.move  = bestMove;
    res.score = bestScore;
    array_free(&moves);

    hashmap_res  = (NEGAMAX_RETURN *)malloc(sizeof(NEGAMAX_RETURN));
    *hashmap_res = res;
    hashmap_put(map, currentState_key, hashmap_res);

    return res;
}

int evaluateState(GameState state)
{
    int i, j;
    int score = 0;

    if (state.winner == state.activePlayer)
        score = INT_MAX;
    else if (state.winner == nextPlayer(state.activePlayer))
        score = INT_MIN;
    else
    {
        for (i = 0; i < BOARD_SIZE; i++)
            for (j = 0; j < BOARD_SIZE; j++)
            {
                /*
                 * piece types are enum, none=0, man=1, king=2
                 * leverage that for score calculation
                 */

                if (state.board[i][j].piece.player == state.activePlayer)
                    score += state.board[i][j].piece.type;
                else
                    score -= state.board[i][j].piece.type;
            }
    }

    return score;
}

void getAllMoves(GameState state, Array *moves)
{
    int i, j, k;
    Move move;
    Array possibleJumps = array_init(sizeof(Move));
    char dest[3]        = {0};
    int OFFESTS_LEN     = 8;
    int offsets[][2]    = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1},
        {-2, -2},
        {-2, 2},
        {2, -2},
        {2, 2},
    };

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (state.board[i][j].piece.player == state.activePlayer)
            {

                move.player = state.activePlayer;
                move.src.i  = i;
                move.src.j  = j;

                for (k = 0; k < OFFESTS_LEN; k++)
                {
                    move.crownPiece = 0;
                    move.moveType   = MOVE_INVALID;
                    move.dest.i     = i + offsets[k][0];
                    move.dest.j     = j + offsets[k][1];

                    coord2str(dest, move.dest.i, move.dest.j);
                    if (isValidCell(dest))
                    {
                        fillAndValidateMove(&state, &move);
                        getAllPossibleJumps(&state, &possibleJumps);
                        validateJump(&state, &move, possibleJumps);

                        if (move.moveType > 0)
                            // valid move
                            array_push(moves, &move);
                    }
                }
            }
        }
}
