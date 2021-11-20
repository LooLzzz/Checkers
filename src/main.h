#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "objects.h"
#include "state.h"
#include "utils.h"

int main(int argc, char *argv[]);

#define nextPlayer(activePlayer) activePlayer == PLAYER_1 ? PLAYER_2 : PLAYER_1

int isValidCell(char *cell);
void validateInput(GameState *state, int argsCount, char *src, char *dest);
void validateMove(GameState *state, char *src, char *dest);
void makeMove(GameState *state);
int gameloop(GameState *state);
