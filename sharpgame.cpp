#include "sharpgame.h"

SharpGame::SharpGame()
{
    memset ( data , 0 , sizeof ( data ) );
}

bool SharpGame::InBoard(int r, int c){
    return (r >= 0 && c >= 0 && r < 3 && c < 3);
}

bool SharpGame::CanPut(int r, int c){
    return !data[r][c];
}

int SharpGame::Get ( int r , int c ){
    return data[r][c] ;
}

void SharpGame::Set ( int r , int c , int player )
{
    if (InBoard(r,c) && CanPut(r,c))
        data[r][c] = player;

}

int SharpGame::InJudge(int br, int bc, int ar, int ac)
{
    int u = data[br][bc];
    if (u == 0)return 0;
    for (int i = 0;i < 2; ++i){
        br += ar;
        bc += ac;
        int k = data[br][bc];
        if (k != u)return 0;
    }
    return u;
}

int SharpGame::GetWinner (){
    for (int r = 0;r < 3; ++r){
        int w = InJudge(r, 0 , 0, 1);
        if (w)return w;
    }
    for (int c = 0;c < 3; ++c){
        int w = InJudge(0, c , 1, 0);
        if (w)return w;
    }
    int w = InJudge(0, 0, 1, 1);
    if(w)return w;
    w = InJudge(0, 2, 1, -1);
    if(w)return w;
}

bool SharpGame::IsFull(){
    for (int r = 0;r < 3;++r){
        for (int c = 0;c < 3; ++c){
            if (!data[r][c])return false;
        }
    }
    return true;
}
