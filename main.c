#include "global.h"
#include "chess.h"
//include only one of the graphics headers
#include "terminalGraphics.h"
// #include "DSGraphics.h"

//1 for white, -1 for black, 0 for neither
int turnPlayer = 1;
int winner = 0;
int enPassant = 64;
int enPassantBuffer = 0;
int threat[32];
int whiteKingIndex = 60;
int blackKingIndex = 4;


int main() {
    struct Tile* GameBoard = malloc(64 * sizeof(*GameBoard));
    BuildBoard(GameBoard);

   while (winner == 0) {
    Turn(GameBoard);
    }
    displayWinner();

    printf(BLACKTILE "\e[0;37m");    
    free(GameBoard);
    return 0;
}