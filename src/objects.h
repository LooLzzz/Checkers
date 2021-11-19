#pragma once

#define MAX_PIECE_SIZE 32
#define MAX_SYMBOL_SIZE 7
#define MAX_STR_LEN 256

#define BOARD_SIZE 8
#define PIECE_ROWS_AMOUNT 3 // each player has 3 rows of pieces

typedef struct _THEME
{
    char pieces[2][MAX_PIECE_SIZE];
    char tiles[2][MAX_PIECE_SIZE];
    char players[2][MAX_PIECE_SIZE];
} Theme;

typedef enum _PLAYER
{
    PLAYER_NONE = 0,
    PLAYER_1,
    PLAYER_2,
    DRAW,
} PLAYER;

typedef enum _TILE_COLOR
{
    TILE_BLACK = 0,
    TILE_WHITE,
} TILE_COLOR;

typedef enum _PIECE_TYPE
{
    PIECE_NONE = 0,
    PIECE_MAN,
    PIECE_KING,
} PIECE_TYPE;

typedef enum _MOVE_TYPE
{
    MOVE_INVALID = 0,
    MOVE_WALK,
    MOVE_JUMP,
} MOVE_TYPE;

/**
 * Piece struct
 * @param type piece type, one of [ PIECE_NONE, PIECE_MAN, PIECE_KING ]
 * @param player the player whose piece is owned by
 */
typedef struct _PIECE
{
    /**
     * piece type, one of [ PIECE_NONE, PIECE_MAN, PIECE_KING ]
     */
    PIECE_TYPE type;

    /**
     * player the player whose piece is owned by
     */
    PLAYER player;
} Piece;

/**
 * Cell struct
 * @param piece piece type, see piece struct for more info
 * @param tile tile color, one of [ TILE_BLACK, TILE_WHITE ]
 */
typedef struct _CELL
{
    /** 
     * @param type piece type, one of [ PIECE_NONE, PIECE_MAN, PIECE_KING ]
     * @param player the player whose piece is owned by
     */
    Piece piece;

    /**
     * tile color, one of [ TILE_BLACK, TILE_WHITE ]
     */
    TILE_COLOR tile;
} Cell;

/**
 * Move struct
 * @param moveType move type, one of [ INVALID, WALK, JUMP ]
 * @param crownPiece Did the piece reach the end, make king [ 0 = False, 1 = True ]
 * @param middlePiece the piece that was eaten, NULL if no piece was eaten
 * @param src the piece that we are moving
 * @param dest the piece that we are going to land in
 */
typedef struct _MOVE
{
    MOVE_TYPE moveType;
    char crownPiece;
    Piece *middlePiece;
    Piece *src;
    Piece *dest;
} Move;

/**
 * Board state struct
 * @param board the cells of the board
 * @param activePlayer The player whose turn it is
 * @param winner The player who won the game
 */
typedef struct _GAME_STATE
{
    /**
     * The board is a 2D array of cells
     * @param piece piece type, see piece struct for more info.
     * @param tileColor piece color, one of [ TILE_BLACK, TILE_WHITE ]
     */
    Cell board[BOARD_SIZE][BOARD_SIZE];

    /**
     * The player whose turn it is
     * @param PLAYER_NONE means the game is over.
     * @param PLAYER_1 first player's turn
     * @param PLAYER_2 second player's turn
     */
    PLAYER activePlayer;

    /**
     * The player who won the game
     * @param PLAYER_NONE means the game is still going.
     * @param PLAYER_1 first player won
     * @param PLAYER_2 second player won
     * @param DRAW the game ended in a draw
     */
    PLAYER winner;
} GameState;
