
#include "GoBang_ChessBoard.h"

#include <algorithm>
#include <functional>
#include <vector>
using namespace std;

const char  Chess_Empty = 'e'; 
const char  Chess_Black = 'b';
const char  Chess_White = 'w';
const char  Chess_Edge  = '/';

const int  Board_NOVALUE  = 0;
const int  Board_FIVE   = 1;
const int  Board_FOUR   = 2;
const int  Board_SFOUR  = 3;
const int  Board_THREE  = 4;
const int  Board_STHREE = 5;
const int  Board_TWO    = 6;
const int  Board_STWO   = 7;
const int  Board_ONE    = 8;
const int  Board_SONE   = 9;
const int  Board_LONG   = 10; 
const int  Board_DONE    = -1;

const int64_t  MM_MAX  = 2000000000;
const int64_t  MM_MIN  =-2000000000;

 //                   5        open4    s4     open3       s3    open2   s2   open1  s1 
int ScoreList[] = {0, 
    1000000,       //five
     100000,       //open4
     10000,        //s4
     10000- 5000 , //open3
      1000,    //s3
      1000-500,//open2
      100,     //s2
      100-50,  //open1
      10 ,     //s1
      2000000  //long
};

ChessBoard::ChessBoard(int size):m_size(size)
{
    for(int i=0; i<=size+2; i++)   {
        for(int j=0; j<=size+2;  j++) {
            m_board[i][j] = Chess_Empty;
            if( i==0 || j==0 || i>size || j>size )
                m_board[i][j] = Chess_Edge;
        }
    }
    m_stepnum=0; 
}


int ChessBoard:: PutChess(int x, int y , ChessSide side)
{
    
    m_board[x][y] = side;
    StPos pos(x, y, side);
    m_chessList.push_back(pos);
    m_stepnum ++;
    return 0;
}


ChessSide ChessBoard::UnPutChess(int x, int y)
{
    char  c = m_board[x][y];
    m_board[x][y] = Chess_Empty;
    m_chessList.pop_back(); 
    m_stepnum--;
    return c;
}

void ChessBoard::ShowChessBoard ()
{
    char  szTmp[1024];
    int   len =0;
    for(int i=1; i<=m_size ; i++ )
    {
        szTmp[len++]= ('0'+i);
        for(int j=1; j <=m_size; j++ ) 
        {
            char c;
            if( m_board[i][j] == Chess_White)
                c = 'O' ;
            else if( m_board[i][j] == Chess_Black)
                c = '#';
            else if(m_board[i][j]  == Chess_Empty)
                c = '_';
            else 
                c= '/';
            szTmp[len++] =    c;                 
            szTmp[len++] = ' '; 
        } 
        szTmp[len++] = '\n';
    }
    for(int i=0; i<m_size; i++) 
        szTmp[len++] = '0'+i;
    szTmp[len++] = '\n';
    szTmp[len++] = 0;
    printf("%s\n" , szTmp);
}

bool  ChessBoard::IsLong(char oneLine[],int len,    int pos, int &resL , int &resR)
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if (ri-li +1 >=6 )
    {
        resL = li;
        resR = ri;
        return true;
    }
    else 
        return false;
}

bool  ChessBoard::Is5(char oneLine[], int len, int pos, int &resL , int &resR)
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if( ri - li +1 == 5)
    {
        resL = li;
        resR = ri;
        return  true;
    }
    else
        return false;
}

bool ChessBoard::IsOpen4(char oneLine[], int len , int pos, int &resL, int &resR)
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if( ri - li +1 != 4 )
        return false;
    if( ll < li  &&  rr > ri)
    {
        resL = ri;
        resR = ri;
        return true;
    }
    return false;
}

bool ChessBoard::IsSide4(char oneLine[], int len , int pos, int &resL, int &resR)
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if(rr - ll +1 < 5)
        return false;
    
    int cnt = ri - li +1; 
    if(cnt ==4 &&  ll == li && rr > ri)
    {
        resL= li;
        resR= ri;   
        return true;
    }
    if(cnt == 4 &&  rr == ri && ll < li  )
    {
        resL = li;
        resR = ri;
        return true;
    }
    cnt = 0; 
    //向左尝试放4个
    int kk=0, ii=0, rL = 0, rR = 0;
    for( kk = ri ; kk>=ll && ii<5 ; kk--, ii++)
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = Is5(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    //向右4个
    ii=0;
    for( kk = li ; kk<=rr  && ii<5 ; kk++ ,ii++)
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = Is5(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    if(cnt == 1)
    {
        resL = li;
        resR = ri;
        return true;
    }
    return false;
}

bool ChessBoard::IsOpen3(char oneLine[], int len, int pos, int &resL ,int &resR)
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if(rr - ll +1 < 5)
        return false;

    int cnt = 0;
    //向左尝试放3个
    int kk=0, ii=0, rL = 0, rR = 0;
    for( kk = ri ; kk>=ll && ii<4 ; kk--, ii++)
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsOpen4(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    //向右3个
    ii=0;
    for( kk = li ; kk<=rr  && ii<4 ; kk++, ii++ )
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsOpen4(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    if(cnt > 0 )
    {
        resL = li;
        resR = ri;
        return true;
    }

    return false;
}

bool ChessBoard::IsSide3(char oneLine[], int len, int pos, int &resL , int &resR )
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if(rr - ll +1 < 5)
        return false;

    int cnt = 0;
    //向左尝试放3个
    int kk=0, ii=0, rL = 0, rR = 0;
    for( kk = ri ; kk>=ll && ii<4 ; kk--, ii++)
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsSide4(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    //向右3个
    ii=0;
    for( kk = li ; kk<=rr  && ii<4 ; kk++, ii++ )
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsSide4(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    if(cnt > 0 )
    {
        resL = li;
        resR = ri;
        return true;
    }

    return false;
}

bool ChessBoard::IsOpen2(char oneLine[], int len , int pos , int &resL , int &resR) 
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if(rr - ll +1 < 5)
        return false;

    int cnt = 0;
    //向左尝试放3个
    int kk=0, ii=0, rL = 0, rR = 0;
    for( kk = ri ; kk>=ll && ii<3 ; kk--, ii++)
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsOpen3(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    //向右3个
    ii=0;
    for( kk = li ; kk<=rr  && ii<3 ; kk++, ii++ )
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsOpen3(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    if(cnt > 0 )
    {
        resL = li;
        resR = ri;
        return true;
    }

    return false;

}

bool ChessBoard::IsSide2(char oneLine[], int len , int pos, int &resL, int &resR)
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if(rr - ll +1 < 5)
        return false;

    int cnt = 0;
    //向左尝试放3个
    int kk=0, ii=0, rL = 0, rR = 0;
    for( kk = ri ; kk>=ll && ii<3 ; kk--, ii++)
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsSide3(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    //向右3个
    ii=0;
    for( kk = li ; kk<=rr  && ii<3 ; kk++, ii++ )
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsSide3(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    if(cnt > 0 )
    {
        resL = li;
        resR = ri;
        return true;
    }

    return false;
}

bool ChessBoard::IsOpen1(char oneLine[], int len , int pos, int &resL, int &resR)
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if(rr - ll +1 < 5)
        return false;

    int cnt = 0;
    //向左尝试放1个
    int kk=0, ii=0, rL = 0, rR = 0;
    for( kk = ri ; kk>=ll && ii<2 ; kk--, ii++)
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsOpen2(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    //向右1个
    ii=0;
    for( kk = li ; kk<=rr  && ii<2 ; kk++, ii++ )
    {
        if (oneLine[kk] != Chess_Empty)
            continue;
        oneLine[kk] = side;

        bool res = IsOpen2(oneLine,len ,pos, rL, rR );
        if(res )
            cnt++;
        oneLine[kk] = Chess_Empty;
    }
    if(cnt > 0 )
    {
        resL = li;
        resR = ri;
        return true;
    }

    return false;
}


bool ChessBoard::IsSide1(char oneLine[], int len , int pos, int &resL, int &resR)
{
    char side = oneLine[pos];
    if( Chess_Empty == side  )
        return  false;
    if( pos > len || pos<0 )
        return false;
    if( side != oneLine[pos])
        return false;

    int ri =pos, li=pos;
    int rr =pos, ll=pos;
    while( oneLine[li-1] == side )  li--;
    while( oneLine[ri+1] == side )  ri++;
    rr = ri; ll = li; 
    while( oneLine[ll-1] == Chess_Empty || oneLine[ll-1] == side ) ll--;
    while( oneLine[rr+1] == Chess_Empty || oneLine[rr+1] == side ) rr++;

    if(rr - ll +1 < 5)
        return false;
    if( rr==ri && ll < li ) 
        return true;
    if( ll == li && rr > ri ) 
        return true;

    return false;
}


BoardType  ChessBoard::CalculateLine(char  oneLine[],  int len , int pos, int &resL , int &resR)
{
    if(IsLong(oneLine, len , pos, resL , resR ) )
        return Board_LONG;
    if(Is5(oneLine, len , pos , resL , resR))
        return Board_FIVE;
    if(IsOpen4(oneLine, len , pos ,resL , resR))
        return Board_FOUR;
    if(IsSide4(oneLine, len , pos, resL , resR)   )
        return Board_SFOUR;
    if(IsOpen3(oneLine , len , pos, resL, resR) )
        return Board_THREE;
    if(IsSide3(oneLine, len , pos , resL , resR) )
        return Board_STHREE;
    if(IsOpen2(oneLine, len , pos, resL , resR ))
        return Board_TWO;
    if(IsSide2(oneLine, len , pos, resL , resR) ) 
        return Board_STWO;
    if(IsOpen1(oneLine, len, pos, resL, resR) )
        return Board_ONE;
    if(IsSide1(oneLine, len , pos, resL, resR))
        return Board_SONE;
    
    return Board_NOVALUE;
}


/*
 * function:  判断这点从某个方向上，形成的棋子形状, oneline 两端包含边界的Chess_Edge
 */
BoardType ChessBoard:: CalculatePosHeng(int x, int y, ChessSide side)
{
    if( x==0 || y== 0 || x > m_size || y> m_size )  
        return 0;
    char * line = m_board[x]; 
    int pos =  y; 
    int rl , rr;
    return CalculateLine( line, m_size+2, pos,rl, rr );
}

BoardType ChessBoard:: CalculatePosShu(int x, int y, ChessSide side)
{
   if( x==0 || y== 0 || x > m_size || y> m_size )  
        return 0;
    char  line[30]; 
    int pos =0;
    for(int i=0;i<= m_size+1; i++ )
        line[i] = m_board[i][y];
    pos = x;

    int rl , rr;
    return CalculateLine( line, m_size+2, pos,rl, rr );
}

BoardType  ChessBoard:: CalculatePosLeft(int x, int y, ChessSide side)
{
    if( x==0 || y== 0 || x > m_size || y> m_size )  
        return 0;

    char line[30];
    int  kk =0;
    int  pos = 0;
    if( x>=y ) // (x-y,0), (x-y+1,1) (x-y+2,2) ...... 
    {
       int  xx = x-y;
       int  yy = 0;
       for(kk=0 ; xx <=m_size +2; xx ++, kk++, yy++ )
       {
            line[kk] = m_board[xx][yy];
            if( xx == x )
                pos = kk;
       }
    }
    else //(0, y-x) , (1, y-x +1), (2, y-x+2)
    {
        int xx =  0; 
        int yy = y-x; 
        for( kk=0; yy<=m_size+2; xx++, yy++, kk++ )
        {
            line[kk] = m_board[xx][yy];
            if(xx == x )
                pos = kk;
        }
    }
    int rr, rl;
    return CalculateLine( line, m_size+2, pos,rl, rr );

}

BoardType  ChessBoard:: CalculatePosRight(int x, int y, ChessSide side)
{
   if( x==0 || y== 0 || x > m_size || y> m_size )  
        return 0;
   char line[30];
   int kk = 0;
   int pos = 0;
   if( x + y <= m_size+2 )  // (x+y,0), (x+y-1, 1) .....
   {
        int xx = x+y;
        int yy = 0;
        for( kk=0; xx >= 0 ; kk++,yy++ , xx--  )
        {
            line[kk] = m_board[xx][yy];
            if( xx == x )
                pos = kk;
        }
   }
   else  //(m_size+2 , y-(m_size+2-x) ), ....( y+x - (m-size-2)   ,  m_size+2)
   {
       int xx = m_size+2;
       int yy = y+x - m_size-2; 
       for(kk=0; yy<=m_size+2 && xx>=0;  xx--, yy++ , kk++ )
       {
            line[kk] = m_board[xx][yy]; 
            if( xx ==  x)
                pos = kk;
       }
   }
   int rr, rl;
   return CalculateLine( line, m_size+2, pos,rl, rr );
}

int ChessBoard::GetPossbilePos(ChessSide side, std::vector<StPos >  &list)
{
    for(int i =1; i<=m_size; i++ )
        for(int j=1; j<=m_size; j++ )
        {
            if( m_board[i][j] !=  Chess_Empty )    
                continue;
            int dist  = m_size;
            for(int k=0; k< m_chessList.size(); k++ )
            {
                int xx = i - m_chessList[k].x;
                int yy = j - m_chessList[k].y;
                if(xx<0 ) xx = -xx;
                if(yy<0 ) yy = -yy;
                int tt = (xx>yy)?xx:yy;
                dist = (tt<dist )?tt:dist;
                if(dist <=2)
                    break;
            }
            if( dist<=2 )
            {
                StPos pos(i,j,side);
                list.push_back(pos);
            }
        }
    return  list.size();
}

int64_t ChessBoard::CalculateOnePos(int x, int y , ChessSide side, int flag )
{
    BoardType  res[4]={0};
    res[0] = CalculatePosHeng(x, y, side);
    res[1] = CalculatePosShu(x,y, side);
    res[2] = CalculatePosLeft(x, y, side);
    res[3] = CalculatePosRight(x, y, side);
    //printf("CaluatOne pos(%u %u)  %u \n", x, y , res[0]);
    //printf("CaluatOne pos(%u %u)  %u \n", x, y , res[1]);
    //printf("CaluatOne pos(%u %u)  %u \n", x, y , res[2]);
    //printf("CaluatOne pos(%u %u)  %u \n", x, y , res[3]);
    //memset(m_bType, 0 , sizeof(m_bType));
    //memset(m_wType, 0 , sizeof(m_wType));


    int xx = x - (1+m_size)/2;
    int yy = y - (1+m_size)/2; 
    if(xx<0 ) xx = -xx;
    if(yy<0 ) yy = -yy;
    int dist = (xx>yy )? xx:yy;


    int64_t score =   ScoreList[res[0]] + ScoreList[res[1]] + ScoreList[res[2]] 
        + ScoreList[res[3]];
    if(flag != 0 )
    {
        if(side == Chess_Black )
        {
            m_bType[res[0]] ++;
            m_bType[res[1]] ++;
            m_bType[res[2]] ++;
            m_bType[res[3]] ++;

        }
        if(side == Chess_White )
        {
            m_wType[res[0]] ++;
            m_wType[res[1]] ++;
            m_wType[res[2]] ++;
            m_wType[res[3]] ++;

        }
    }

    return score + (1+m_size)/2 - dist;
}

int64_t ChessBoard::CalculateBoard( ChessSide side , int &win, bool isMax)
{
    int  blackWin=0, whiteWin=0, blackScore=0, whiteScore=0; 
    memset(m_bType, 0 , sizeof(m_bType));
    memset(m_wType, 0 , sizeof(m_wType));

    for(int i=0; i<m_chessList.size(); i++ ) 
    {
        int t = CalculateOnePos(m_chessList[i].x, m_chessList[i].y,
                m_chessList[i].side, 1);                    

        if( Chess_Black ==  m_chessList[i].side )
        {
            blackScore += t;
        }
        else
        {
            whiteScore += t;
        }
    }     

    int64_t  score;
    if( side == Chess_Black )
        score = blackScore - whiteScore;
    else
        score = whiteScore - blackScore;

    win = 0;
    if( side == Chess_Black  && isMax) //这里是AI下子后
    {
        if(m_bType[Board_FIVE] > 0  ) 
            win = 1; 
        else if(m_bType[Board_FOUR] > 0   &&
                m_wType[Board_FOUR]==0  && m_wType[Board_SFOUR] == 0 ) 
            win = 1;
        if(win == 1)
            score =  MM_MAX;
    }
    else if( side == Chess_Black &&  !isMax ) //对方下完后
    {
        if(m_wType[Board_FIVE] >0     ) 
            win =1;
        else if(m_wType[Board_FOUR] >0  &&
                m_bType[Board_FOUR] ==0 && m_bType[Board_SFOUR] ==0 ) 
            win = 1;
        if(win == 1)
            score = MM_MIN;
    }
    else if(side == Chess_White  && isMax) //AI白，AI下子后
    {
        if(m_wType[Board_FIVE ]> 0 ) 
            win = 1;
        if(m_wType[Board_FOUR] > 0   &&
                m_bType[Board_FOUR]==0  && m_bType[Board_SFOUR] == 0 ) 
            win = 1;
        if(win==1  )
            score == MM_MAX;
    }
    else if(side == Chess_White && !isMax)
    {
         if(m_bType[Board_FIVE] > 0  ) 
            win = 1; 
        else if(m_bType[Board_FOUR] > 0   &&
                m_wType[Board_FOUR]==0  && m_wType[Board_SFOUR] == 0 ) 
            win = 1;
        if(win == 1)
            score =  MM_MIN;
    }
    return score;
}


int64_t ChessBoard::GetNextPosMinMax(int deep, StPos &pos, int64_t alpha , int64_t beta )
{
    int64_t bestVa = MM_MIN;
    std::vector<StPos>   nextList;
    std::vector<StPos>   bestList;

    int n = GetPossbilePos( m_aiside, nextList);
    for( int i=0; i<nextList.size(); i++ )
    {
        PutChess(nextList[i].x, nextList[i].y , m_aiside);
        nextList[i].score = CalculateOnePos(nextList[i].x, nextList[i].y,
                m_aiside,0);
        UnPutChess(nextList[i].x, nextList[i].y );
        //printf("Pos(%u %u), socre:%u \n", nextList[i].x, nextList[i].y ,nextList[i].score );
    }
    sort(nextList.begin(), nextList.end(), greater<StPos>()   );    

    for( int i=0; i< nextList.size(); i++ )
    {
        StPos pos = nextList[i];
        PutChess(pos.x, pos.y,m_aiside  ); 
        int win = 0; 
        int64_t va = GetMinValue(deep-1, alpha ,beta) ;
        UnPutChess(pos.x, pos.y);

        if(bestVa <  va)
        {
            bestVa = va;
            bestList.clear();
            bestList.push_back(pos);
        }
        if( va == bestVa)
        {
            bestList.push_back(pos);
        }
        if(va > alpha )
            alpha = va;
        if( alpha >= beta) 
            break;
    }

    for(int i=0 ; i< bestList.size(); i++ )
    {
        PutChess( bestList[i].x, bestList[i].y, m_aiside);
        bestList[i].score = CalculateOnePos( bestList[i].x , bestList[i].y , m_aiside, 0 ); 
        UnPutChess(bestList[i].x, bestList[i].y );
        printf("select pos(%u %u) score:%d , mapVar:%d\n",bestList[i].x ,bestList[i].y, bestList[i].score, bestVa );
    }
    sort(bestList.begin(),   bestList.end(), greater<StPos>()  );
    if(bestList.size() > 0  )
    {
        pos.x = bestList[0].x;
        pos.y = bestList[0].y;
        return 0;    
    }
    return -1;
}


int64_t   ChessBoard::  GetMinValue(int deep, int64_t alpha, int64_t  beta)
{
    int64_t bestVa = MM_MAX;
    std::vector<StPos>   nextList;
    int64_t va = MM_MAX;
    int win = 0;
    va = CalculateBoard(m_aiside, win , true);
    if( deep <= 0  || win )
    {
        return va;
    }
    
    int n = GetPossbilePos( m_otherside, nextList);
    for( int i=0; i<nextList.size(); i++ )
    {
        PutChess(nextList[i].x, nextList[i].y , m_otherside);
        nextList[i].score = CalculateOnePos(nextList[i].x, nextList[i].y,
                m_otherside, 0 );
        UnPutChess(nextList[i].x, nextList[i].y);
    }
    sort(nextList.begin(), nextList.end(), greater<StPos>()   );    

    for( int i=0; i< nextList.size(); i++ )
    {
        StPos pos = nextList[i];
        PutChess(pos.x, pos.y, m_otherside  ); 
        win = 0; 
        va =  GetMaxValue(deep-1, alpha, beta);
        UnPutChess(pos.x, pos.y);
        
        if(va  <  bestVa)  
        {
            bestVa = va;
        }
        if(va <   beta )
            beta = va;
        if( alpha >= beta) 
            break;
    }
    return bestVa;
}

int64_t   ChessBoard::  GetMaxValue(int deep, int64_t alpha, int64_t beta)
{
    int64_t bestVa = MM_MIN;
    std::vector<StPos>   nextList;
    int64_t va = MM_MIN;
    int win = 0;
    va = CalculateBoard(m_aiside, win, false );
    if( deep <= 0  || win )
    {
        return va;
    }
    
    int n = GetPossbilePos( m_aiside, nextList);
    for( int i=0; i<nextList.size(); i++ )
    {
        PutChess(nextList[i].x, nextList[i].y, m_aiside);
        nextList[i].score = CalculateOnePos(nextList[i].x, nextList[i].y,
                m_aiside, 0);
        UnPutChess(nextList[i].x, nextList[i].y);
    }
    sort(nextList.begin(), nextList.end(), greater<StPos>()   );    

    for( int i=0; i< nextList.size(); i++ )
    {
        StPos pos = nextList[i];
        PutChess(pos.x, pos.y, m_aiside  ); 
        win = 0; 
        va =  GetMinValue(deep-1, alpha , beta);
        UnPutChess(pos.x, pos.y);
        
        if(va  >  bestVa)  
        {
            bestVa = va;
        }
        if(va >   alpha )
            alpha = va;
        if( alpha >= beta) 
            break;
    }
    return bestVa;
}


int ChessBoard:: PutHumenChessGetAI(StPos &humPos, StPos &aiPos)
{
    if( m_aiside == Chess_Black   && m_stepnum == 0 )
    {
        aiPos.x = (1+m_size)/2;
        aiPos.y = (1+m_size)/2;
        return 0;
    }
    else if( m_aiside == Chess_White && m_stepnum == 0 )
    {
        PutChess(humPos.x, humPos.y, Chess_Black);

        int x ,y;
        x = y = (1+m_size)/2;
        int k = random()%8; 
        if( k==0 ){
            aiPos.x = x-1;   aiPos.y = y-1;
        }
        if(k==1){
            aiPos.x = x-1;   aiPos.y = y ;
        }
        if(k==2){
            aiPos.x = x-1;   aiPos.y = y+1;
        }
        if(k==3){
            aiPos.x = x ;  aiPos.y = y-1;
        }
        if(k==4){
            aiPos.x = x ; aiPos.y = y+1 ;
        }
        if(k==5){
            aiPos.x = x+1; aiPos.y = y-1;
        }
        if(k==6){
            aiPos.x = x+1; aiPos.y = y;
        }
        if(k==7){
            aiPos.x = x+1; aiPos.y =  y+1;
        }
        return 0;
    }

    PutChess(humPos.x, humPos.y, m_otherside);
    
    int ret = GetNextPosMinMax(2, aiPos,MM_MIN, MM_MAX );
    
    return 0;
}



