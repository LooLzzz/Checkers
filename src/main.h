#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "objects.h"
#include "state.h"
#include "utils.h"

int main(int argc, char *argv[]);

#define nextPlayer(activePlayer) (activePlayer == PLAYER_1      \
                                    ? PLAYER_2                  \
                                    : activePlayer == PLAYER_2  \
                                        ? PLAYER_1              \
                                        : PLAYER_NONE)

int isValidCell(char *cell);
void validateInput(GameState *state, int argsCount, char *src, char *dest, Move *move);
void fillAndValidateMove(GameState *state, Move *move);
void validateJump(GameState *state, Move *move, Array possibleJumps);
void makeMove(GameState *state, Move move);
int gameloop(GameState *state);

void updateWinState(GameState *state);
void getPossibleJumps(GameState *state, Array *array, int i, int j);
void getAllPossibleJumps(GameState *state, Array *moves);
