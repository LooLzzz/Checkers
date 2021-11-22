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
void validateJump(GameState *state, Array possibleJumps);
void makeMove(GameState *state);
int gameloop(GameState *state);

void updateWinState(GameState *state);
void getPossibleJumps(GameState *state, Array *array, int i, int j);
void getAllPossibleJumps(GameState *state, Array *moves);
