#include "global.h"
#include "terminalGraphics.h"
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

void PrintBoard(struct Tile *GameBoard){ //prints to terminal
    printf(" a  b  c  d  e  f  g  h ""%s""[ ]"BLACKTILE WHITETEXT"\n", (turnPlayer == 1) ? "\e[31m" : "\e[34m");
    for (int i =0; i<64; i++){ //I am so sorry for the formatting
        char *tileColor;
        tileColor = ((i%2) != (i/8)%2) ? BLACKTILE : WHITETILE;
        printf("%s""[%s%s%c"WHITETEXT"%s]", 
        tileColor, (GameBoard[i].pieceColor == 1) ? REDTEXT : (GameBoard[i].pieceColor == -1) ? BLUETEXT : WHITETEXT, 
        tileColor, GameBoard[i].pieceType, tileColor); 
        if (i%8 == 7) {
            printf(BLACKTILE WHITETEXT" %c \n", '8' - i/8);
        }
    }
}

void promotePawn(struct Tile *PawnToPromote){ //includes important terminal graphics
        int promotion;
        printf("Promote pawn to Q, H, R, or B?");
        promotion = getchar();
        while ((getchar())!= '\n');
        if (promotion > 96) promotion -= 32;
        if (promotion == 'Q' || promotion == 'H' || promotion == 'R' || promotion == 'B')
        {PawnToPromote->pieceType = (char) promotion;}
        else {promotePawn(PawnToPromote);}
}

void Turn(struct Tile *GameBoard){ //terminal Turn, includes important terminal graphics
    char start[8];
    char end[8];
    int startIndex;
    int endIndex;
    do {
        printf("Enter the coordinates of the piece to move:");
        fgets(start, sizeof(start), stdin);
        if (start[0] == 'r' && start[1] == 'e' && start[2] == 's') {
            winner = (turnPlayer == 1) ? -1 : 1; 
            return;
        }
        startIndex = convertAndVerifyCoords(start);
        if (startIndex >=0 && startIndex < 64 && GameBoard[startIndex].pieceColor != turnPlayer) {
            printf("Wrong Color!\n");
            startIndex = 64;
        }
    } while (startIndex < 0 || startIndex >= 64);
    do {
        printf("Enter the coordinates of the destination tile:");
        fgets(end, sizeof(end), stdin);
        if (start[0] == 'r' && start[1] == 'e' && start[2] == 's') {
            winner = (turnPlayer == 1) ? -1 : 1; 
            return;
        }
        endIndex = convertAndVerifyCoords(end);
    } while (endIndex < 0 || endIndex >= 64);
    Move(GameBoard, startIndex, endIndex);
    PrintBoard(GameBoard);
}

void displayWinner(){ //terminal graphics
    switch (winner)
    {
    case 1:
        printf("White Wins!");
        break;
    case -1:
        printf("Black Wins!");
        break;
    
    default:
        printf("Its Tied!");
        break;
    }
}