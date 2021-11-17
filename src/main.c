#include "main.h"

int main(int argc, char *argv[])
{
    char *inputFilename;
    GameState state;

    // filename must be provided
    if (argc == 2)
        inputFilename = argv[1];
    else
    {
        printf("Usage: %s <input file>\n", "./checkers");
        return 1;
    }

    loadState(inputFilename, &state); // will use `initializeState()` if file doesn't exist or empty
    printState(&state);

    //TODO: implement game loop

    return 0;
}