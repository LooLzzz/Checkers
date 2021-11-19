#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "objects.h"
#include "utils.h"

void printState(GameState *state);
void initializeState(GameState *state);
void saveState(char *fileName, GameState *state);
void loadState(char *fileName, GameState *state);
void readStateFromFile(FILE *fp, GameState *state);
