#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "objects.h"
#include "state.h"
#include "utils.h"

/**
 * Validate the cell string given, will try to fix the string if it's not valid
 * @return 1 if the cell is valid, 0 otherwise
 */
int isValidCell(char *cell);

void validateInput(GameState *state, int argsCount, char *src, char *dest, Move *move);
void fillAndValidateMove(GameState *state, Move *move);
void validateJump(GameState *state, Move *move, Array possibleJumps);
