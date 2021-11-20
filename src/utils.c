#include "utils.h"

// TODO: theme selection functionality

char *piece2str(Piece piece, char *str)
{
    Theme theme                     = THEME_1; // THEME_2;
    char symbols[][MAX_SYMBOL_SIZE] = SYMBOLS;
    str[0]                          = '\0';

    // piece's color
    if (piece.player > 0)
        strcat(str, theme.pieces[piece.player - 1]);

    // piece's symbol
    strcat(str, symbols[piece.type]);
    return str;
}

char *tile2str(TILE_COLOR tile, char *str)
{
    Theme theme = THEME_1; // THEME_2;
    str[0]      = '\0';

    strcat(str, theme.tiles[tile]);
    return str;
}

char *player2str(PLAYER player, char *str)
{
    Theme theme = THEME_1; // THEME_2;
    str[0]      = '\0';

    strcat(str, theme.players[player != PLAYER_1]);
    strcat(str, player == PLAYER_1 ? " PLAYER1" : " PLAYER2");

    return str;
}

char *str2playercolor(char *str, PLAYER player)
{
    Theme theme = THEME_1; // THEME_2;
    char temp[MAX_STR_LEN];

    sprintf(temp, "%s%s", theme.players[player != PLAYER_1], str);
    strcpy(str, temp);
    return str;
}

/**
 * compares two strings, ignoring case
 * @return difference between last equal index ( a[i] - b[i] ). 0 means strings are equal.
 */
int _strcmpi(char *a, char *b)
{
    int i = 0;

    for (; a[i] && b[i]; i++)
        if (tolower(a[i]) != tolower(b[i]))
            return a[i] - b[i];

    return tolower(a[i]) - tolower(b[i]);
}

void str_toupper(char *str)
{
    int i = 0;
    for (; str[i]; i++)
        str[i] = toupper(str[i]);
}

void str_tolower(char *str)
{
    int i = 0;
    for (; str[i]; i++)
        str[i] = tolower(str[i]);
}
