#include "GoBang_ChessBoard.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int x, y ;
    char side;
    ChessBoard board;
    printf("please select the AI's side(w/b)\n");
    scanf("%c", &side);
    printf("AI's side is %c \n", side);

    board.SetAISide(side);
    StPos humPos(0,0,'w');
    StPos aiPos(0,0,'b');

    if( side == 'w')
    {
        while(1)
        {
            printf("human please  input your black chess:\n");
            scanf("%u %u",  &x, &y);
            printf("Human Put white  Chess %u %u\n",x , y);
            humPos.x = x; humPos.y = y;
            board.PutHumenChessGetAI( humPos , aiPos);
            board.ShowChessBoard();
            printf("AI put black chess: %u %u\n", aiPos.x, aiPos.y );
            board.PutChess(aiPos.x, aiPos.y, Chess_White);
            board.ShowChessBoard();
        }
    }

    if(side == 'b')
    {
        while(1)
        {
            board.PutHumenChessGetAI(humPos, aiPos);
            board.ShowChessBoard();
            printf("Ai Put black chess  %u %u \n",  aiPos.x, aiPos.y);        
            board.PutChess(aiPos.x, aiPos.y, Chess_Black);
            board.ShowChessBoard();
            printf("human please input your white chess\n"); 
            scanf("%u %u",  &x, &y);
            printf("Human Put white  Chess %u %u\n",x , y);
            humPos.x = x; humPos.y = y; 
        }
    }

    return 0;

}
