#pragma once

#include <stdlib.h>
#include <string.h>

#include "objects.h"

#define ESC " \e[0m"               // clear formatting
#define WHITE_TILE "\e[48;5;254m"  // white bg
#define BLACK_TILE "\e[48;5;130m"  // brown bg
#define BLACK_PIECE "\e[38;5;232m" // black fg
#define WHITE_PIECE "\e[38;5;222m" // lightyellow fg

#define PLAYER1_TXT "\e[48;5;222m\e[38;5;232m" //lightyellow bg, black fg
#define PLAYER2_TXT "\e[48;5;232m\e[38;5;254m" //black bg, white fg

#define THEME_1                               \
    {                                         \
        .pieces = {WHITE_PIECE, BLACK_PIECE}, \
        .tiles  = {BLACK_TILE, WHITE_TILE},   \
        .players = {PLAYER1_TXT, PLAYER2_TXT} \
    }

#define THEME_2                               \
    {                                         \
        .pieces = {WHITE_PIECE, BLACK_PIECE}, \
        .tiles  = {WHITE_TILE, BLACK_TILE},   \
        .players = {PLAYER2_TXT, PLAYER1_TXT} \
    }

// #define SYMBOLS \
//     {           \
//         "  ",  \
//         " O", \
//         " K", \
//     }

// #define SYMBOLS \
//     {           \
//         "   ",  \
//         " ⛂ ", \
//         " ⛃ ", \
//     }

#define SYMBOLS \
    {           \
        "   ",  \
        " ⬤ ", \
        " ⛒ ", \
    }

char *piece2str(Piece piece);
char *tile2str(TILE_COLOR tile);
char *player2str(PLAYER player);
char *getEsc();



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
