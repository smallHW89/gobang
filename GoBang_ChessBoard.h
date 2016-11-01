
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string.h>
#include <algorithm>

using namespace std;

typedef char       ChessSide;
extern const char  Chess_Empty ; 
extern const char  Chess_Black ;
extern const char  Chess_White ;
extern const char  Chess_Edge  ;

typedef int       BoardType; 
extern const int  Board_LONG;
extern const int  Board_FIVE;
extern const int  Board_FOUR;
extern const int  Board_SFOUR;
extern const int  Board_THREE;
extern const int  Board_STHREE;
extern const int  Board_TWO;
extern const int  Board_STWO;
extern const int  Board_ONE;
extern const int  Board_SONE;
extern const int  Board_NOVALUE;
extern const int  Board_DONE;

struct StPos
{
    StPos(int _x, int _y, ChessSide _s):x(_x), y(_y),side(_s)
    {
        score = 0;
    }
    int x;
    int y;
    int64_t  score;
    ChessSide side;


    bool operator < (const StPos &rhs) const
    {
        return score < rhs.score;
    }

    bool operator > (const StPos &rhs) const
    {
        return score > rhs.score;
    }

};


/*
 *  gobang chessboard
 */
class ChessBoard
{
public:
    ChessBoard(int size = 15 );
    ~ChessBoard(){};
    void       SetAISide(ChessSide s){
        m_aiside = s;
        if( s == Chess_Black)
            m_otherside = Chess_White;
        else
            m_otherside = Chess_Black;
    }
    void       AddStep(){
        m_stepnum++;
    }
    int        PutChess(int x, int y , ChessSide side);
    ChessSide  UnPutChess(int x , int y);
    void       ShowChessBoard();

    BoardType CalculateLine(char  oneLine[],  int len , int pos, int &resL , int &resR);
    BoardType CalculatePosHeng(int x, int y, ChessSide side);
    BoardType CalculatePosShu(int x, int y, ChessSide side);
    BoardType CalculatePosLeft(int x, int y, ChessSide side);
    BoardType CalculatePosRight(int x, int y, ChessSide side);

    int   GetPossbilePos(ChessSide  side, std::vector<StPos>  & list);

    int64_t   CalculateOnePos(int x , int y, ChessSide , int flag);
    int64_t   CalculateBoard( ChessSide, int &win, bool isMax);

    int64_t   GetNextPosMinMax(int deep, StPos &pos, int64_t alpha , int64_t beta );
    int64_t   GetMaxValue(int deep, int64_t alpha, int64_t beta);
    int64_t   GetMinValue(int deep, int64_t alpha, int64_t beta);

    int   PutHumenChessGetAI(StPos &humPos, StPos &aiPos);


private:
    int  m_stepnum;
    char m_board[20][20] ; //chessboard , higheit 1----15 ,   
    int  m_size; 
    ChessSide  m_aiside;
    ChessSide  m_otherside;
    int  m_bType[20];
    int  m_wType[20];
    
    std::vector<StPos>  m_chessList;

   
    bool  IsLong(char oneLine[],int len,    int pos, int &resL , int &resR);
    bool  Is5(char oneLine[], int len, int pos, int &resL , int &resR);
    bool  IsOpen4(char oneLine[], int len , int pos, int &resL, int &resR);
    bool  IsSide4(char oneLine[], int len , int pos, int &resL, int &resR);
    bool  IsOpen3(char oneLine[], int len, int pos,  int &resL , int &resR);
    bool  IsSide3(char oneLine[], int len, int pos,  int &resL , int &resR);
    bool  IsOpen2(char oneLine[], int len , int pos ,  int &resL , int &resR); 
    bool  IsSide2(char oneLine[], int len , int pos,   int &resL,   int &resR);
    bool  IsOpen1(char oneLine[], int len , int pos,   int &resL,   int &resR);
    bool  IsSide1(char oneLine[], int len , int pos,   int &resL,   int &resR);
};

