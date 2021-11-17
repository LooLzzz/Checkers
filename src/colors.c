#include "colors.h"

char *char2color(char c, char is_bg)
{
    if (is_bg)
        switch (c)
        {
            case 'W':
                return WHITE_TILE;
            case 'R':
                return BLACK_TILE;
        }
    else // fg
        switch (c)
        {
            case 'B':
                return BLACK_PIECE;
            case 'R':
                return WHITE_PIECE;
        }

    return "";
}
