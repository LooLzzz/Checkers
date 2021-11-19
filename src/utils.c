#include "utils.h"

char *piece2str(Piece piece)
{
    // TODO: theme selection functionality
    Theme theme = THEME_1; // THEME_2;
    char symbols[][MAX_SYMBOL_SIZE] = SYMBOLS;
    char *res   = calloc(MAX_PIECE_SIZE, sizeof(char));

    // piece's color
    if (piece.player > 0)
        strcat(res, theme.pieces[piece.player - 1]);

    // piece's symbol
    strcat(res, symbols[piece.type]);

    return res;
}

char *tile2str(TILE_COLOR tile)
{
    // TODO: theme selection functionality
    Theme theme = THEME_1; // THEME_2;
    char *res   = calloc(MAX_PIECE_SIZE, sizeof(char));

    strcat(res, theme.tiles[tile]);
    return res;
}

char *player2str(PLAYER player)
{
    if (player == PLAYER_1)
        return "PLAYER1";
    else if (player == PLAYER_2)
        return "PLAYER2";
    return "";
}

char *getEsc()
{
    return ESC;
}
