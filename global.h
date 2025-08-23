#include <stdio.h>
#include <stdlib.h>

#ifndef GLOBAL_H
#define GLOBAL_H

//1 for white, -1 for black, 0 for neither
extern int turnPlayer;
extern int winner;
extern int enPassant;
extern int enPassantBuffer;
extern int threat[32];
extern int whiteKingIndex;
extern int blackKingIndex;

struct Tile {
    int pieceColor; //1, 0, -1
    char pieceType; //P, Q, R, B, K, H, U
    int moved; //0, 1
};


#endif