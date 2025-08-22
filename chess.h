#include "global.h"

#ifndef CHESS_H
#define CHESS_H

void nextTurn();
int convertAndVerifyCoords(char *coordinate);
void BuildBoard(struct Tile *GameBoard);
void setPossibleMove(struct Tile *GameBoard, int attackerIndex, int defenderIndex);
void possibleMoves(struct Tile *GameBoard, int index);
void testForCheck(struct Tile *GameBoard);
void Move(struct Tile *GameBoard, int start, int end);

#endif