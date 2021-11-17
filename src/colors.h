#pragma once

#define ESC " \e[0m" // clear formatting

#define WHITE_TILE "\e[48;5;254m"  // white bg
#define BLACK_TILE "\e[48;5;130m"  // brown bg
#define BLACK_PIECE "\e[38;5;232m" // black fg
#define WHITE_PIECE "\e[38;5;222m" // lightyellow fg

char *char2color(char c, char is_bg);
