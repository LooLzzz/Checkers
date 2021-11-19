#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "utils.h"
#include "objects.h"

int main(int argc, char *argv[]);
Move validateInput(GameState *state, char *src, char *dest);
void movePiece(Move move);
void gameloop(GameState *state);