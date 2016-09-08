#ifndef SHARPGAME_H
#define SHARPGAME_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
#include <ctime>
using namespace std ;

class SharpGame {
public:
    SharpGame () ;
    int Get ( int r , int c ) ;
    void Set ( int r , int c , int player ) ;
    int GetWinner();
    bool InBoard(int r, int c);
    bool CanPut(int r, int c);
    bool IsFull();
private:
    int InJudge (int br, int bc, int ar, int ac) ;
private:
    int data[3][3] ;
};

#endif // SHARPGAME_H
