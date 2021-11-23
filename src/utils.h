#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "objects.h"

#define ESC "\e[0m"                  // clear formatting
#define WHITE_TILE "\e[48;5;254m"    // white bg
#define BLACK_TILE "\e[48;5;130m"    // brown bg
#define BLACK_PIECE "\e[1;38;5;232m" // black fg
#define WHITE_PIECE "\e[1;38;5;222m" // lightyellow fg

#define PLAYER1_TXT "\e[48;5;222m\e[38;5;232m" // lightyellow bg, black fg
#define PLAYER2_TXT "\e[48;5;232m\e[38;5;254m" // black bg, white fg

#define THEME_1                                \
    {                                          \
        .pieces  = {WHITE_PIECE, BLACK_PIECE}, \
        .tiles   = {BLACK_TILE, WHITE_TILE},   \
        .players = {PLAYER1_TXT, PLAYER2_TXT}  \
    }

#define THEME_2                                \
    {                                          \
        .pieces  = {WHITE_PIECE, BLACK_PIECE}, \
        .tiles   = {WHITE_TILE, BLACK_TILE},   \
        .players = {PLAYER2_TXT, PLAYER1_TXT}  \
    }

#define SYMBOLS  \
    {            \
        "   ",   \
        " ⬤ ",   \
        " ■ ",   \
    }

// #define SYMBOLS  \
//     {            \
//         "   ",   \
//         " O ",   \
//         " K ",   \
//     }

#define nextPlayer(activePlayer) (activePlayer == PLAYER_1      \
                                    ? PLAYER_2                  \
                                    : activePlayer == PLAYER_2  \
                                        ? PLAYER_1              \
                                        : PLAYER_NONE)

#define moveType2str(type) \
    type == MOVE_JUMP      \
        ? " => Jump "      \
        : ""

#define crowned2str(crowned) \
    crowned                  \
        ? " => Crowned! "    \
        : ""

#define cmpcoords(a, b) a.i == b.i && a.j == b.j

char *piece2str(Piece piece, char *str);
char *tile2str(TILE_COLOR tile, char *str);
char *player2str(PLAYER player, char *str);
char *str2playercolor(char *str, PLAYER player);

void coord2str(char *str, int i, int j);

int str_cmpi(char *a, char *b);
void str_strip(char *str);
void str_toupper(char *str);
void str_tolower(char *str);

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec);

/*
 * ⏹
 * ■
 * ⯀
 */
