#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "hashmap.h"
#include "list.h"
#include "objects.h"

#define ZOBRIST_PIECES_COUNT 4
#define MAX_DEPTH 5

typedef enum _ZOBRIST_PIECES
{
    ZOBRIST_PLAYER1_MAN = 0,
    ZOBRIST_PLAYER1_KING,
    ZOBRIST_PLAYER2_MAN,
    ZOBRIST_PLAYER2_KING,
} ZOBRIST_PIECES;

typedef struct zobrist_s
{
    uint64_t hashtab[MAX_DEPTH + 1][BOARD_SIZE * BOARD_SIZE][ZOBRIST_PIECES_COUNT];
    bool initialized;
} zobrist_t;

void zobrist_init();
uint64_t zobrist_hash(GameState *state, int depth);
