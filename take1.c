#include <stdio.h>


struct Position {
    char column;
    char row;
};

struct Piece {
    char captured; //Y,N
    char color; //W, B
    char type; //P, Q, R, B, K, H
    struct Position position;
    
};

struct Position Move(struct Piece selectedPiece){

};

void Setup(struct Piece* White, struct Piece* Black){

    for (int i = 0; i<=8; i++){
        White[i].color = 'W';
        White[i].position.column = 'a' + i;
        White[i].position.row = '1';
        White[i].captured = 'N';
        Black[i].color = 'B';
        Black[i].position.column = 'a' + i;
        Black[i].position.row = '8';
        Black[i].captured = 'N';
    }

    for (int i = 8; i<=15; i++){
        White[i].color = 'W';
        White[i].position.column = 'a' + i-8;
        White[i].position.row = '2';
        White[i].type = 'P';
        White[i].captured = 'N';
        Black[i].color = 'B';
        Black[i].position.column = 'a' + i-8;
        Black[i].position.row = '7';
        Black[i].type = 'P';
        Black[i].captured = 'N';
    }

    White[0].type = 'R'; White[7].type = 'R';
    Black[0].type = 'R'; Black[7].type = 'R';
    White[1].type = 'H'; White[6].type = 'H';
    Black[1].type = 'H'; Black[6].type = 'H';
    White[2].type = 'B'; White[5].type = 'B';
    Black[2].type = 'B'; Black[5].type = 'B';
    White[3].type = 'Q'; White[4].type = 'K';
    Black[3].type = 'Q'; Black[4].type = 'K';

};

char turn = 'W';


int main() 
{
    struct Piece* Player1 = malloc(16 * sizeof *Player1);
    struct Piece* Player2 = malloc(16 * sizeof *Player2);
    Setup(Player1, Player2);

    free(Player1); free(Player2);
    return 0;
}