#include <stdio.h>
#include <stdlib.h>

//1 for white, -1 for black, 0 for neither
int turnPlayer = 1;
int winner = 0;
int enPassant = 64;
int enableEnPassant = 0;
int threat[32];
int whiteKingIndex = 60;
int blackKingIndex = 4;

void nextTurn() {
    if(turnPlayer == 1) turnPlayer = -1;
    else turnPlayer = 1;
    if (enableEnPassant != 0) enableEnPassant = 0;
    else if (enPassant != 64) {enPassant = 64;}
}

int convertAndVerifyCoords(char *coordinate) { //might not need
    if (coordinate[0] < 'a' || coordinate[0] > 'h' || coordinate[1] < '1' || coordinate[1] > '8') {
        printf("INVALID COORDINATES!\n");
        return -1; //invalid coordinates
    }
    return coordinate[0]-'a'+(8*(8-coordinate[1]+'0'));
}

struct Tile {
    int pieceColor; //1, 0, -1
    char pieceType; //P, Q, R, B, K, H, U
    int moved; //0, 1
};

//remove later

#define BLACKTILE "\e[40m"
#define WHITETILE "\e[47m"
#define BLUETEXT "\e[0;34m"
#define REDTEXT "\e[0;31m"
#define WHITETEXT "\e[0;37m"

void PrintBoard(struct Tile *GameBoard){ //prints to terminal
    printf(" a  b  c  d  e  f  g  h ""%s""[ ]"BLACKTILE WHITETEXT"\n", (turnPlayer == 'W') ? "\e[31m" : "\e[34m");
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
//end of remove later

void BuildBoard(struct Tile *GameBoard){
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

}

void promotePawn(struct Tile *PawnToPromote){
        int promotion;
        printf("Promote pawn to Q, H, R, or B?");
        promotion = getchar();
        while ((getchar())!= '\n');
        if (promotion > 96) promotion -= 32;
        PawnToPromote->pieceType = (char) promotion;
}

void setPossibleMove(struct Tile *GameBoard, int attackerIndex, int defenderIndex){ //helper function, dont call
    for(int i = 0; i<32; i++){
        if (threat[i] == attackerIndex){
            break;
        }
        if (threat[i] == 64){
            threat[i] = attackerIndex;
            break;
        }
    }
}

void possibleMoves(struct Tile *GameBoard, int index){ //checks and sets possible moves for a piece
    switch (GameBoard[index].pieceType)
    {
    case 'U':
    break;
    case 'P':
        if (GameBoard[index-(8*GameBoard[index].pieceColor)].pieceType == 'U'){//move successful pawn move exception
            setPossibleMove(GameBoard, index, index-(8*GameBoard[index].pieceColor));
            if (GameBoard[index].moved == 0 && GameBoard[index-(16*GameBoard[index].pieceColor)].pieceType == 'U'){
                setPossibleMove(GameBoard, index, index-(16*GameBoard[index].pieceColor));
            }    
        }
        if (GameBoard[index+1-(8*GameBoard[index].pieceColor)].pieceColor == GameBoard[index].pieceColor*(-1) && index % 8 != 0){
            setPossibleMove(GameBoard, index, index+1-(8*GameBoard[index].pieceColor));
        }
        if (GameBoard[index-1-(8*GameBoard[index].pieceColor)].pieceColor == GameBoard[index].pieceColor*(-1) && index % 8 != 7){
            setPossibleMove(GameBoard, index, index+1-(8*GameBoard[index].pieceColor));
        }
        //en passant
        if (enPassant != 64){
            setPossibleMove(GameBoard, index, enPassant);
        }
        break;
    case 'K':
        if ((index-9) >= 0 && index % 8 != 0){
            if (GameBoard[index-9].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-9);
            }
        }
        if ((index-8) >= 0){
            if (GameBoard[index-8].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-8);
            }
        }
        if ((index-7) >= 0 && index % 8 != 7){
            if (GameBoard[index-7].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-7);
            }
        }
        if (index % 8 != 0){
            if (GameBoard[index-1].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-1);
            }
        }
        if (index % 8 != 7){
            if (GameBoard[index+1].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+1);
            }
        }
        if ((index+7) <= 63 && index % 8 != 0){
            if (GameBoard[index+7].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+7);
            }
        }
        if ((index+8) <= 63){
            if (GameBoard[index+8].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+8);
            }
        }
        if ((index+9) <= 63 && index % 8 != 0){
            if (GameBoard[index+9].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+9);
            }
        }
    break;
    case 'R':
        for(int i = 1; i < 8; i++){
            if ((index-i+1)%8 != 0 && GameBoard[index-i].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-i);
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if ((index+i-1)%8 != 7 && GameBoard[index+i].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+i);
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if (index-i*8 >= 0 && GameBoard[index-i*8].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-i*8);
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if (index+i*8 <= 63 && GameBoard[index+i*8].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+i*8);
            }
            else break;
        }
    break;
    case 'H':
        if (index-10 >= 0 && (index-1)%8 != 0 && index%8 != 0 && GameBoard[index-10].pieceColor != GameBoard[index].pieceColor){
            setPossibleMove(GameBoard, index, index-10);
        }
        if (index-17 >= 0 && (index)%8 != 0 && GameBoard[index-17].pieceColor != GameBoard[index].pieceColor){
            setPossibleMove(GameBoard, index, index-17);
        }
        if (index-6 >= 0 && (index-1)%8 != 7 && index%8 != 7 && GameBoard[index-6].pieceColor != GameBoard[index].pieceColor){
            setPossibleMove(GameBoard, index, index-6);
        }
        if (index-15 >= 0 && (index)%8 != 7 && GameBoard[index-15].pieceColor != GameBoard[index].pieceColor){
            setPossibleMove(GameBoard, index, index-15);
        }
        if (index+10 >= 0 && (index+1)%8 != 7 && index%8 != 7 && GameBoard[index+10].pieceColor != GameBoard[index].pieceColor){
            setPossibleMove(GameBoard, index, index+10);
        }
        if (index+17 >= 0 && (index)%8 != 7 && GameBoard[index+17].pieceColor != GameBoard[index].pieceColor){
            setPossibleMove(GameBoard, index, index+17);
        }
        if (index+6 >= 0 && (index+1)%8 != 0 && index%8 != 0 && GameBoard[index+6].pieceColor != GameBoard[index].pieceColor){
            setPossibleMove(GameBoard, index, index+6);
        }
        if (index+15 >= 0 && (index)%8 != 0 && GameBoard[index+15].pieceColor != GameBoard[index].pieceColor){
            setPossibleMove(GameBoard, index, index+15);
        }
    break;
    case 'B':
        for(int i = 1; i < 8; i++){
            if ((index-i+1)%8 != 0 && index-(9*i) >=0 && GameBoard[index-(9*i)].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-(9*i));
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if ((index+i-1)%8 != 7 && index-(7*i) >= 0 && GameBoard[index-(7*i)].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-(7*i));
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if ((index-i+1)%8 != 0 && index+(7*i) <= 63 && GameBoard[index+(7*i)].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+(7*i));
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if ((index+i-1)%8 != 7 && index+(9*i) <= 63 && GameBoard[index+(9*i)].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+(9*i));
            }
            else break;
        }
    break;
    case 'Q':
        //rook code
        for(int i = 1; i < 8; i++){
            if ((index-i+1)%8 != 0 && GameBoard[index-i].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-i);
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if ((index+i-1)%8 != 7 && GameBoard[index+i].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+i);
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if (index-i*8 >= 0 && GameBoard[index-i*8].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-i*8);
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if (index+i*8 <= 63 && GameBoard[index+i*8].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+i*8);
            }
            else break;
        }
        //bishop code
        for(int i = 1; i < 8; i++){
            if ((index-i+1)%8 != 0 && index-(9*i) >=0 && GameBoard[index-(9*i)].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-(9*i));
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if ((index+i-1)%8 != 7 && index-(7*i) >= 0 && GameBoard[index-(7*i)].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index-(7*i));
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if ((index-i+1)%8 != 0 && index+(7*i) <= 63 && GameBoard[index+(7*i)].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+(7*i));
            }
            else break;
        }
        for(int i = 1; i < 8; i++){
            if ((index+i-1)%8 != 7 && index+(9*i) <= 63 && GameBoard[index+(9*i)].pieceColor != GameBoard[index].pieceColor){
                setPossibleMove(GameBoard, index, index+(9*i));
            }
            else break;
        }
    break;
    default:
        break;
    }
}

void possibleMovesAll(struct Tile *GameBoard){ //maybe delete or change to check checker 
    for (int i = 0; i<64; i++) {
        possibleMoves(GameBoard, i);
    }
}

int verifyMove(struct Tile *GameBoard, int start, int end){
    possibleMoves(GameBoard, start);
    for (int i = 0; i<32; i++){
        if (threat[i] == end){
            if (GameBoard[start].pieceType == 'P' && GameBoard[start].moved == 0 && GameBoard[start-(16*GameBoard[start].pieceColor)].pieceType == 'U'){
                enableEnPassant = 1;
                enPassant = start-(8*GameBoard[start].pieceColor);
            } //enable en passant
            return 1;
        } //move successful
        if (threat[i] == 64){
            break; //no threat found
        }
    }
    return 0;
}

void Move(struct Tile *GameBoard, int start, int end){
    if (verifyMove(GameBoard, start, end) == 0) {
        printf("THAT PIECE CAN'T MOVE THERE!\n");
        return; //invalid move
    }
    if (end == whiteKingIndex || end == blackKingIndex) winner = turnPlayer;
    GameBoard[start].moved = 'Y';
    GameBoard[end] = GameBoard[start];
    GameBoard[start] = (struct Tile) {0,'U',0};
    nextTurn();
}

void Turn(struct Tile *GameBoard){ //terminal Turn
    char start[8];
    char end[8];
    printf("Enter the coordinates of the piece to move:");
    fgets(start, sizeof(start), stdin);
    if (start[0] == 'r' && start[1] == 'e' && start[2] == 's') {
        winner = (turnPlayer == 'W') ? 'B' : 'W'; 
        return;
    }
    printf("Enter the coordinates of the destination tile:");
    fgets(end, sizeof(end), stdin);
    int startIndex = convertAndVerifyCoords(start);
    int endIndex = convertAndVerifyCoords(end);
    Move(GameBoard, startIndex, endIndex);
    PrintBoard(GameBoard);
}


int main(){
    struct Tile* GameBoard = malloc(64* sizeof *GameBoard);
    BuildBoard(GameBoard);
    PrintBoard(GameBoard);

   while (winner == 0) {
    Turn(GameBoard);
    }

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

    printf(BLACKTILE "\e[0;37m");    
    free(GameBoard);
    return 0;
}