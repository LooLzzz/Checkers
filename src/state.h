#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "objects.h"
#include "utils.h"

void printState(GameState *state, char clearTerminal);
void initializeState(GameState *state);
void saveState(char *fileName, GameState *state);
void loadState(char *fileName, GameState *state);

void deletePiece(Cell board[][BOARD_SIZE], Coord src);
void movePiece(Cell board[][BOARD_SIZE], Coord src, Coord dest);
void crownPiece(Cell board[][BOARD_SIZE], Coord src);
