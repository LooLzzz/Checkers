#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"

#define BOARD_SIZE 8
#define MAX_PIECE_SIZE 12

typedef struct _CELL
{
    char piece[MAX_PIECE_SIZE];
    char bg; // [W]hite or [B]lack
    char fg; // [R]ed or [B]lack
} Cell;

typedef struct _GAME_STATE
{
    Cell board[BOARD_SIZE][BOARD_SIZE];
    int activeTurn; // {0: white, 1: black}
    int winner;     // {0: no winner, 1: white, 2: black, 3: draw}
} GameState;

void printState(GameState *state);
void initializeState(GameState *state);
void saveState(char *fileName, GameState *state);
void loadState(char *fileName, GameState *state);
void readStateFromFile(FILE *fp, GameState *state);
