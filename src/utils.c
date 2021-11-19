#include "utils.h"

// TODO: theme selection functionality

char *piece2str(Piece piece)
{
    char *res                       = calloc(MAX_PIECE_SIZE, sizeof(char));
    Theme theme                     = THEME_1; // THEME_2;
    char symbols[][MAX_SYMBOL_SIZE] = SYMBOLS;

    // piece's color
    if (piece.player > 0)
        strcat(res, theme.pieces[piece.player - 1]);

    // piece's symbol
    strcat(res, symbols[piece.type]);

    return res;
}

char *tile2str(TILE_COLOR tile)
{
    char *res   = calloc(MAX_PIECE_SIZE, sizeof(char));
    Theme theme = THEME_1; // THEME_2;

    strcat(res, theme.tiles[tile]);
    return res;
}

char *player2str(PLAYER player)
{
    char *res   = calloc(MAX_PIECE_SIZE, sizeof(char));
    Theme theme = THEME_1; // THEME_2;

    strcat(res, theme.players[player != PLAYER_1]);
    strcat(res, player == PLAYER_1 ? " PLAYER1" : " PLAYER2");

    return res;
}

char *getEsc()
{
    return ESC;
}
