#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "objects.h"
#include "state.h"
#include "validation.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

typedef struct _NEGAMAX_RETURN
{
    int score;
    Move move;
} NEGAMAX_RETURN;

NEGAMAX_RETURN negamax(int depth, GameState currentState, int alpha, int beta);
int evaluateState(GameState state);

void getAllMoves(GameState state, Array *moves);
