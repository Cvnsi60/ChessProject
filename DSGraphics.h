#include "global.h"

#ifdef TERMINALGRAPHICS_H
#error "DSGraphics.h and TerminalGraphics.h cannot be included in the same file"
#endif

#ifndef DSGRAPHICS_H
#define DSGRAPHICS_H

void BuildBoard(struct Tile *GameBoard);
void PrintBoard(struct Tile *GameBoard);
void promotePawn(struct Tile *PawnToPromote);
void Turn(struct Tile *GameBoard);
void displayWinner();

#endif