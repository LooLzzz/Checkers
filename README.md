# Checkers

This program is for playing checkers on the command line.
Once started, you may use choose a mode, singleplayer or multiplayer.

## Game

### Singleplayer

Singleplayer will play against the computer.

The AI is using the minimax algorithm with alpha-beta pruning at depth 5.
In order to make the AI more efficient, it is using hashmap with zobrist hashing in order to store the board states it has encountered.

### Multiplayer

Multiplayer will play against another player, from two different terminals.

Once the first player has started the game, the second player can connect to the first player and play against him.
Each terminal will write its board state to the same file, and that way the game is able to be synchronized between two different terminals.  
**Each player must have `RW` previleges to the file.**

---

## Build

Use the following command to build and run the program:

```bash
make run
```

### Notes

In order to see the board properly, the terminal must have a proper unicode font enabled.

It is possible to play the game without unicode.  
in `utils.h` file:

* uncomment the wanted `SYMBOLS` and comment the others
* change the symobls to the ones you want
* run the build command
* enjoy the game without unicode

---

## Previews

### New game

![New-Game](https://user-images.githubusercontent.com/8081213/142846808-d86b8919-ce6b-4b00-8c8f-3ae9fd55f765.gif)

### Doublejump To King

![Doublejump-To-King](https://user-images.githubusercontent.com/8081213/142846080-be0385e1-671b-4af0-b949-780631005d41.gif)

### Pseudo Multiplayer

![Pseudo-Multiplayer](https://user-images.githubusercontent.com/8081213/143029118-cd416aef-246e-44ec-bd1d-30c9a09713f0.gif)
