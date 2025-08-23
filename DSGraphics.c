#include "global.h"
#include "DSGraphics.h"
#include "chess.h"

void BuildBoard(struct Tile *GameBoard){ //Board reset and print
    for(int i = 0; i<64; i++) {
        if(i<16){
            GameBoard[i].pieceColor = -1;
        }
        else if(i>=48){
            GameBoard[i].pieceColor = 1;
        }
        else {
            GameBoard[i].pieceColor = 0;
        }

        if ((i<16 && i>7)||(i<56 && i>47)){
            GameBoard[i].pieceType = 'P';
        }
        else if (i>15 && i<48) {
            GameBoard[i].pieceType = 'U';
        }
        else if (i%8 == 0 || i%8 == 7){
            GameBoard[i].pieceType = 'R';
        }
        else if (i%8 == 1 || i%8 == 6){
            GameBoard[i].pieceType = 'H';
        }
        else if (i%8 == 2 || i%8 == 5){
            GameBoard[i].pieceType = 'B';
        }
        else if (i%8 == 3){
            GameBoard[i].pieceType = 'Q';
        }
        else if (i%8 == 4){
            GameBoard[i].pieceType = 'K';
        }
        GameBoard[i].moved = 0;
        if (i<32){
            threat[i] = 64;
        }
    }
    PrintBoard(GameBoard);
}

void PrintBoard(struct Tile *GameBoard){ //prints to DS

}

void promotePawn(struct Tile *PawnToPromote){ //prints to DS

}

void Turn(struct Tile *GameBoard){ //DS Turn, prints to DS

}

void displayWinner(){ //DS graphics

}