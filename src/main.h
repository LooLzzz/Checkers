#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "objects.h"
#include "state.h"
#include "utils.h"
#include "validation.h"

int main(int argc, char *argv[]);
int gameloop(GameState *state);

void getPossibleJumps(GameState *state, Array *array, int i, int j);
void getAllPossibleJumps(GameState *state, Array *moves);
