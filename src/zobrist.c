#include "zobrist.h"

zobrist_t keys = {.initialized = false};

static uint64_t get64rand()
{
    return (((uint64_t)rand() << 0) & 0x000000000000FFFFull) |
           (((uint64_t)rand() << 16) & 0x00000000FFFF0000ull) |
           (((uint64_t)rand() << 32) & 0x0000FFFF00000000ull) |
           (((uint64_t)rand() << 48) & 0xFFFF000000000000ull);
}

void zobrist_init()
{
    int d, i, j;

    for (d = 0; d < MAX_DEPTH + 1; d++)
        for (i = 0; i < BOARD_SIZE * BOARD_SIZE; i++)
            for (j = 0; j < ZOBRIST_PIECES_COUNT; j++)
                keys.hashtab[d][i][j] = get64rand();

    keys.initialized = true;
}

uint64_t zobrist_hash(GameState *state, int depth)
{
    if (!keys.initialized)
        zobrist_init();

    uint64_t hash = 0;
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (state->board[i][j].piece.type != PIECE_NONE)
            {
                if (state->board[i][j].piece.player == PLAYER_1)
                    hash ^= keys.hashtab[depth][i * BOARD_SIZE + j][state->board[i][j].piece.type - 1]; // will result in `hashtab[position][{0/1}]`
                else
                    hash ^= keys.hashtab[depth][i * BOARD_SIZE + j][state->board[i][j].piece.type + 1]; // will result in `hashtab[position][{2/3}]`
            }
        }

    return hash;
}
