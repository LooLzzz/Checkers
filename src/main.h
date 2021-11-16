#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 8


// TODO: Fix names according to colors
#define WHITE_BG "\e[48;5;254m" // white
#define RED_BG "\e[48;5;130m" // brown
#define BLACK_FG "\e[38;5;232m" // black
#define RED_FG "\e[38;5;222m" // lightyellow
#define ESC " \e[0m"

typedef struct _CELL
{
    char *piece;
    char bg; // [W]hite or [B]lack
    char fg; // [R]ed or [B]lack
} Cell;

typedef struct _GAME_STATE
{
    Cell board[BOARD_SIZE][BOARD_SIZE];
    int activeTurn; // {0: white, 1: black}
    int winner; // {0: no winner, 1: white, 2: black, 3: draw}
} GameState;

// TODO: this VVV
void saveState(char *fileName, GameState *state); // Board Turn Etc
void loadState(char *fileName, GameState *state);

void print_board(Cell board[][BOARD_SIZE]);
void initialize_board(Cell board[][BOARD_SIZE]);
char *char2color(char c, char is_bg);
int main(int argc, char *argv[]);


    /*
    * ⏺
    * ⬤
    * ⚪
    * ⚫
    * ⛒
    * 🅧
    * 🆇
    * Ⓧ
    * ⓧ
    * 🇽
    * 🔴
    * 🔵
    */