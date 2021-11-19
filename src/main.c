#include "main.h"

int main(int argc, char *argv[])
{
    char inputFilename[MAX_STR_LEN] = {0};
    GameState state;

    // state's filename must be provided
    if (argc == 2)
    {
        int idx = strrchr(argv[0], '/') - argv[0];
        if (idx > 0)
            memcpy(inputFilename, argv[0], sizeof(char) * ++idx);
        strcat(inputFilename, argv[1]);
    }
    else
    {
        printf("Usage: %s <input file>\n", "./checkers");
        return 1;
    }

    // initialize game state
    loadState(inputFilename, &state);
    
    // start game
    gameloop(state);

    return 0;
}

void gameloop(GameState state)
{
    //TODO: implement game loop
    printState(&state);
}