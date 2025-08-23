#include "global.h"

#ifdef DSGRAPHICS_H
#error "TerminalGraphics.h and DSGraphics.h cannot be included in the same file"
#endif

#ifndef TERMINALGRAPHICS_H
#define TERMINALGRAPHICS_H

#define BLACKTILE "\e[40m"
#define WHITETILE "\e[47m"
#define BLUETEXT "\e[0;34m"
#define REDTEXT "\e[0;31m"
#define WHITETEXT "\e[0;37m"

void BuildBoard(struct Tile *GameBoard);
void PrintBoard(struct Tile *GameBoard);
void promotePawn(struct Tile *PawnToPromote);
void Turn(struct Tile *GameBoard);
void displayWinner();

#endif