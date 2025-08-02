#include <stdio.h>
#include <stdlib.h>


#define BLACKTILE "\e[40m"
#define WHITETILE "\e[47m"
#define BLUETEXT "\e[0;34m"
#define REDTEXT "\e[0;31m"
#define WHITETEXT "\e[0;37m"

char nextToMove = 'W';
char winner = 'N';
char enPassant[] = "ff\n";
char enableEnPassant = 'N';

void nextTurn() {
    if(nextToMove == 'W') nextToMove = 'B';
    else nextToMove ='W';
    if (enableEnPassant != 'N') enableEnPassant = 'N';
    else if (enPassant != "ff") {enPassant[0] = 'f'; enPassant[1] = 'f';}
}



int convertAndVerifyCoords(char *coordinate) {
    return coordinate[0]-'a'+(8*(8-coordinate[1]+'0'));
}

struct Piece {
    char color; //W, B, U
    char type; //P, Q, R, B, K, H, U
    char moved; //Y, N
};

struct Tile {
    char column;
    char row;
    char *tileColor;
    struct Piece occupied;
};

void BuildBoard(struct Tile *GameBoard){
    for(int i = 0; i<64; i++){
        GameBoard[i].column = 'a' + (i % 8);
        GameBoard[i].row = '8' - (i / 8);
        GameBoard[i].tileColor = ((i%2)!=((i/8)%2)) ? BLACKTILE : WHITETILE;
        GameBoard[i].occupied.moved = 'N';
    }
    for (int i = 16; i<48; i++){
        GameBoard[i].occupied.color = 'U';
        GameBoard[i].occupied.type = 'U';
    }
    for (int i = 8; i<16; i++){
        GameBoard[i].occupied.color = 'B';
        GameBoard[i].occupied.type = 'P';
    }
    for (int i = 48; i<56; i++){
        GameBoard[i].occupied.color = 'W';
        GameBoard[i].occupied.type = 'P';
    }

    GameBoard[0].occupied.color = 'B';  GameBoard[0].occupied.type = 'R';   GameBoard[7].occupied.color = 'B';  GameBoard[7].occupied.type = 'R';
    GameBoard[1].occupied.color = 'B';  GameBoard[1].occupied.type = 'H';   GameBoard[6].occupied.color = 'B';  GameBoard[6].occupied.type = 'H';
    GameBoard[2].occupied.color = 'B';  GameBoard[2].occupied.type = 'B';   GameBoard[5].occupied.color = 'B';  GameBoard[5].occupied.type = 'B';
    GameBoard[3].occupied.color = 'B';  GameBoard[3].occupied.type = 'Q';   GameBoard[4].occupied.color = 'B';  GameBoard[4].occupied.type = 'K';

    GameBoard[56].occupied.color = 'W';  GameBoard[56].occupied.type = 'R';   GameBoard[63].occupied.color = 'W';  GameBoard[63].occupied.type = 'R';
    GameBoard[57].occupied.color = 'W';  GameBoard[57].occupied.type = 'H';   GameBoard[62].occupied.color = 'W';  GameBoard[62].occupied.type = 'H';
    GameBoard[58].occupied.color = 'W';  GameBoard[58].occupied.type = 'B';   GameBoard[61].occupied.color = 'W';  GameBoard[61].occupied.type = 'B';
    GameBoard[59].occupied.color = 'W';  GameBoard[59].occupied.type = 'Q';   GameBoard[60].occupied.color = 'W';  GameBoard[60].occupied.type = 'K';

    printf("\nType \"res\" as the piece to move to resign:\n");
}


void PrintBoard(struct Tile *GameBoard){
    printf(" a  b  c  d  e  f  g  h ""%s""[ ]"BLACKTILE WHITETEXT"\n", (nextToMove == 'W') ? "\e[31m" : "\e[34m");
    for (int i =0; i<63; i+=2){ //I am so sorry for the formatting
        printf("%s""[%s%s%c"WHITETEXT"%s]" "%s[%s%s%c"WHITETEXT"%s]", 
        GameBoard[i].tileColor, (GameBoard[i].occupied.color == 'W') ? REDTEXT : (GameBoard[i].occupied.color == 'B') ? BLUETEXT : WHITETEXT, GameBoard[i].tileColor, GameBoard[i].occupied.type, GameBoard[i].tileColor,
        GameBoard[i+1].tileColor, (GameBoard[i+1].occupied.color == 'W') ? REDTEXT : (GameBoard[i+1].occupied.color == 'B') ? BLUETEXT : WHITETEXT, GameBoard[i+1].tileColor, GameBoard[i+1].occupied.type, GameBoard[i+1].tileColor); 
        if ((i+1)%8 == 7) {
            printf(BLACKTILE WHITETEXT" %c \n", '8' - i/8);
        }
    }
}

void promotePawn(struct Tile *PawnToPromote){
        int promotion;
        printf("Promote pawn to Q, H, R, or B?");
        promotion = getchar();
        while ((getchar())!= '\n');
        if (promotion > 96) promotion -= 32;
        PawnToPromote->occupied.type = (char) promotion;
}

void Move(struct Tile *GameBoard, char *start, char *end){
    
    int startIndex = convertAndVerifyCoords(start);
    int endIndex = convertAndVerifyCoords(end);
    int largerIndex = (startIndex > endIndex) ? startIndex : endIndex;
    int smallerIndex = (startIndex > endIndex) ? endIndex : startIndex;
    
    struct Tile *moveTarget = &GameBoard[startIndex];
    struct Tile *moveDestination = &GameBoard[endIndex];


    if (moveTarget->occupied.color == moveDestination->occupied.color || moveTarget->occupied.type == 'U') {
        printf("INVALID MOVE!\n");
        return;
    }
    if (moveTarget->occupied.color != nextToMove){
        printf("IT'S NOT YOUR TURN!\n");
        return;
    }
    if (moveDestination == moveTarget){
        printf("SELECTIONS CAN'T BE THE SAME!\n");
        return;
    }
    switch (moveTarget->occupied.type)
    {
    case 'P':
        int neg = (moveTarget->occupied.color == 'B') ? -1 : 1;
        if (moveDestination->column == moveTarget->column && moveDestination->occupied.type == 'U') {
            if (moveDestination->row == moveTarget->row + (1*neg)){ //move successful
                if (moveDestination->occupied.type == 'K') winner = nextToMove;
                moveTarget->occupied.moved = 'Y';
                moveDestination->occupied = moveTarget->occupied;
                moveTarget->occupied = (struct Piece) {'U','U','N'};
                if (moveDestination->row == '8' || moveDestination->row == '1') {promotePawn(moveDestination);}
                nextTurn();
                return;
            }
            if (moveDestination->row == moveTarget->row + (2*neg) && moveTarget->occupied.moved == 'N'){ //move successful
                char enPassantRow = start[1] + (1*neg);
                char enPassantColumn = start[0];
                enPassant[0] = enPassantColumn; 
                enPassant[1] = enPassantRow; 
                enableEnPassant = 'Y';
                if (moveDestination->occupied.type == 'K') winner = nextToMove;
                moveTarget->occupied.moved = 'Y';
                moveDestination->occupied = moveTarget->occupied;
                moveTarget->occupied = (struct Piece) {'U','U','N'};
                if (moveDestination->row == '8' || moveDestination->row == '1') {promotePawn(moveDestination);}
                nextTurn();
                return;
            }
        }
        //pawn attack
        if (((moveDestination->column == moveTarget->column + 1 || moveDestination->column == moveTarget->column - 1) && 
            moveDestination->row == moveTarget->row + (1*neg)) && (moveDestination->occupied.color != 'U')){ //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            if (moveDestination->row == '8' || moveDestination->row == '1') {promotePawn(moveDestination);}
            nextTurn();
            return;
        }
        //en passant
        if (((moveDestination->column == moveTarget->column + 1 || moveDestination->column == moveTarget->column - 1) && moveDestination->row == moveTarget->row + (1*neg)) && (end[0] == enPassant[0] && end[1] == enPassant[1])){//en passant successful
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            GameBoard[endIndex+(8*neg)].occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return;
        }
        break;

    case 'R':
        if (moveDestination->column == moveTarget->column){
            for (int i = smallerIndex+8; i<largerIndex; i+=8) {
                if (GameBoard[i].occupied.type != 'U'){
                    printf("PIECE IN THE WAY!\n");
                    return;
                }
            } //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return; 
        }
        if (moveDestination->row == moveTarget->row){
            for (int i = smallerIndex+1; i<largerIndex; i++) {
                if (GameBoard[i].occupied.type != 'U'){
                    printf("PIECE IN THE WAY!\n");
                    return;
                }
            } //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return; 
        }
        break;

    case 'H':
        if ((moveDestination->column == moveTarget->column + 1 && moveDestination->row == moveTarget->row + 2) || 
            (moveDestination->column == moveTarget->column - 1 && moveDestination->row == moveTarget->row + 2) || 
            (moveDestination->column == moveTarget->column + 1 && moveDestination->row == moveTarget->row - 2) || 
            (moveDestination->column == moveTarget->column - 1 && moveDestination->row == moveTarget->row - 2) || 
            (moveDestination->column == moveTarget->column + 2 && moveDestination->row == moveTarget->row + 1) || 
            (moveDestination->column == moveTarget->column - 2 && moveDestination->row == moveTarget->row + 1) || 
            (moveDestination->column == moveTarget->column + 2 && moveDestination->row == moveTarget->row - 1) || 
            (moveDestination->column == moveTarget->column - 2 && moveDestination->row == moveTarget->row - 1) ){ //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return;
        }
        break;

    case 'B':
        if ((largerIndex-smallerIndex)%9 == 0){
            for (int i = smallerIndex+9; i<largerIndex; i+=9) {
                if (GameBoard[i].occupied.type != 'U'){
                    printf("PIECE IN THE WAY!\n");
                    return;
                }
            } //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return; 
        }
        if ((largerIndex-smallerIndex)%7 == 0){
            for (int i = smallerIndex+7; i<largerIndex; i+=7) {
                if (GameBoard[i].occupied.type != 'U'){
                    printf("PIECE IN THE WAY!\n");
                    return;
                }
            } //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return; 
        }
        break;
    
    case 'Q'://B code
        if ((largerIndex-smallerIndex)%9 == 0){
            for (int i = smallerIndex+9; i<largerIndex; i+=9) {
                if (GameBoard[i].occupied.type != 'U'){
                    printf("PIECE IN THE WAY!\n");
                    return;
                }
            } //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return; 
        }
        if ((largerIndex-smallerIndex)%7 == 0){
            for (int i = smallerIndex+7; i<largerIndex; i+=7) {
                if (GameBoard[i].occupied.type != 'U'){
                    printf("PIECE IN THE WAY!\n");
                    return;
                }
            } //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return; 
        }//R code
        if (moveDestination->column == moveTarget->column){
            for (int i = smallerIndex+8; i<largerIndex; i+=8) {
                if (GameBoard[i].occupied.type != 'U'){
                    printf("PIECE IN THE WAY!\n");
                    return;
                }
            } //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return; 
        }
        if (moveDestination->row == moveTarget->row){
            for (int i = smallerIndex+1; i<largerIndex; i++) {
                if (GameBoard[i].occupied.type != 'U'){
                    printf("PIECE IN THE WAY!\n");
                    return;
                }
            } //move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return; 
        }
        break;

    case 'K':
        //add threatening fuction and restrict king movement
        if (largerIndex - smallerIndex == 1 || largerIndex - smallerIndex == 7 || 
            largerIndex - smallerIndex == 8 || largerIndex - smallerIndex == 9){//move successful
            if (moveDestination->occupied.type == 'K') winner = nextToMove;
            moveTarget->occupied.moved = 'Y';
            moveDestination->occupied = moveTarget->occupied;
            moveTarget->occupied = (struct Piece) {'U','U','N'};
            nextTurn();
            return; 
        }
        //add castling here
        break;
    default: printf("INVALID MOVE!\n");
        break;
    }
    printf("THAT PIECE CAN'T MOVE THERE!\n");
}

void Turn(struct Tile* GameBoard){
    char start[8];
    char end[8];
    printf("Enter the coordinates of the piece to move:");
    fgets(start, sizeof(start), stdin);
    if (start[0] == 'r' && start[1] == 'e' && start[2] == 's') {
        winner = (nextToMove == 'W') ? 'B' : 'W'; 
        return;
    }
    printf("Enter the coordinates of the destination tile:");
    fgets(end, sizeof(end), stdin);
    Move(GameBoard, start, end);
    PrintBoard(GameBoard);
}


int main(){

    struct Tile* GameBoard = malloc(64* sizeof *GameBoard);
    BuildBoard(GameBoard);
    PrintBoard(GameBoard);

    while (winner == 'N') {
    Turn(GameBoard);
    }

    switch (winner)
    {
    case 'W':
        printf("White Wins!");
        break;
    case 'B':
        printf("Black Wins!");
        break;
    
    default:
        printf("Its Tied!");
        break;
    }

    free(GameBoard);
    printf(BLACKTILE "\e[0;37m");
    return 0;
}