#ifndef _SHARPGAME_
#define _SHARPGAME_

#include <iostream>
#include <vector>
#include <utility>
using namespace std;

#ifndef INT_MAX
#define INT_MAX (~(1<<31))
#endif

const int INT_INF = INT_MAX;

// 0 为空， 1为玩家1号， 2为玩家二号

const int MAX_DEPTH = 100;
typedef pair<int, int> POS;

class ChessBoard{
public:
    int data[3][3];
    ChessBoard(){
        for (int i = 0;i < 3;++i){
            for (int j = 0;j < 3;++j){
                data[i][j] = 0;
            }
        }
    }
    ChessBoard(const ChessBoard &b){
        for (int i = 0;i < 3;++i){
            for (int j = 0;j < 3;++j){
                data[i][j] = b.data[i][j];
            }
        }
    }
    ChessBoard& operator=(const ChessBoard &b){
        for (int i = 0;i < 3;++i){
            for (int j = 0;j < 3;++j){
                data[i][j] = b.data[i][j];
            }
        }
        return *this;
    }
    void Set(int r,int c,int flag){
        data[r][c] = flag;
    }
    int Get(int r,int c){
        return data[r][c];
    }

    void restart(){
        for (int r = 0;r < 3;++r){
            for (int c = 0;c < 3;++c){
                data[r][c] = 0;
            }
        }
    }

    bool IsFull(){
        for (int r = 0;r < 3;++r){
            for (int c = 0;c < 3;++c){
                if (!data[r][c])return false;
            }
        }
        return true;
    }

    bool InBoard(int r,int c){
        return (r >= 0 && r < 3 && c >= 0 && c < 3);
    }
    bool CanPut(int r,int c){
        return !data[r][c];
    }
    int get_winner() const{
        for (int i = 0;i < 3;++i){
            int w = get_winner_in(i, 0, 0, 1);
            if (w)return w;
        }
        for (int i = 0;i < 3;++i){
            int w = get_winner_in(0, i, 1, 0);
            if (w)return w;
        }
        int w = get_winner_in(0,0,1,1);
        if (w)return w;
        return get_winner_in(2,0,-1,1);
    }
    int get_triple_num() const{
        int u = 0;
        for (int i = 0;i < 3;++i){
            int w = get_winner_in(i, 0, 0, 1);
            if (w)++u;
        }
        for (int i = 0;i < 3;++i){
            int w = get_winner_in(0, i, 1, 0);
            if (w)++u;
        }
        if(get_winner_in(0,0,1,1))++u;
        if(get_winner_in(2,0,-1,1))++u;
        return u;
    }
    void put(int r,int c, int turn){
        data[r][c] = turn;
    }
    int get(int r, int c) const{
        return data[r][c];
    }
private:
    int get_winner_in(int r,int c,int ar,int ac) const{
        int u = data[r][c];
        if (!u)return 0;
        for (int i = 0;i < 2;++i){
            r += ar;
            c += ac;
            if (data[r][c] != u)return 0;
        }
        return u;
    }
};

//玩家2号为AI
int get_score(const ChessBoard &b){
    int w = b.get_winner();
    if (w == 1)return -INT_INF;
    if (w == 2)return INT_INF;
    // w == 0
    ChessBoard b1 = b;
    for (int i = 0;i < 3;++i){
        for (int j = 0;j < 3;++j){
            if(!b1.data[i][j])b1.data[i][j] = 1;
        }
    }

    ChessBoard b2 = b;
    for (int i = 0;i < 3;++i){
        for (int j = 0;j < 3;++j){
            if(!b2.data[i][j])b2.data[i][j] = 2;
        }
    }

    return b2.get_triple_num() - b1.get_triple_num();
}

struct Node{
    int ID;
    string type;
    int ab;
    int score;
    int cut;
    ChessBoard board;
};

int abcut_count = 0;
vector<Node> abcut_nodes;

int abcut(ChessBoard &b,int depth, int alpha,int beta, int turn, POS &p, bool cut){
    abcut_count += 1;
    //alpha: 极大层的下界 >= alpha
    //beta : 极小层的上界 <= beta
    // p 为最终选择的位置
    if (depth == 0)return get_score(b);
    // over case, 这里的判断不是最简的:-(
    // 有一方胜利或者下满平局
    int w = b.get_winner();
    if (w == 1)return -INT_INF;
    if (w == 2)return INT_INF;
    //turn = 2, 极大值层
    bool full = true; // 下满
    for (int r = 0;r < 3;++r){
        for (int c = 0;c < 3;++c){
            if (b.data[r][c] == 0){
                full = false;
                break;
            }
        }
    }

    //下满平局(这个非常重要,否则返回的bestVal 不等于 get_score(b))
    if (full)return get_score(b);

    bool first = true;
    for (int r = 0;r < 3;++r){
        for (int c = 0;c < 3;++c){
            if (b.data[r][c] == 0){
                //下棋
                b.data[r][c] = turn;

                POS nextP;
                int val = 0;

                if (turn == 2)
                    val	= abcut(b, depth - 1, alpha, INT_INF, 3 - turn, nextP, cut);
                else
                    val	= abcut(b, depth - 1, -INT_INF, beta, 3 - turn, nextP, cut);

                //放回
                b.data[r][c] = 0;

                //注意, 以下不判断val == alpha, beta的情况,避免选择剪枝了的节点
                if (turn == 2){
                    //当前节点在极大值层, val 为极小值层的值
                    if (val > alpha || first){
                        // 找到更大的值
                        p = make_pair(r,c);
                        alpha = val; // 更新极大值层的节点的下界alpha
                        first = false;
                    }
                }else{
                    //当前节点在极小值层
                    if (val < beta || first){
                        p = make_pair(r,c);
                        beta = val; // 更新极小值层的节点的上界beta
                        first = false;
                    }
                }

                if (alpha >=  beta && cut){
                    // 剪枝, 放在这里看起来更清晰, 但效率没那么好
                    // 当alpha == beta的时候, 节点的值也能被确定
                    // 需要注意alpha == beta时的返回值
                    // 若该节点为极小值点, 当alpha >= beta时, 它的子节点有可能比beta更小
                    // 若返回beta的话,会导致该节点的父节点误以为该节点的最小值为beta, 导致发生错误
                    Node nd;
                    nd.ID = abcut_count;
                    nd.score = val;
                    nd.cut = 9 - r * 3 - (c + 1);
                    nd.board = b;
                    if (turn == 2){
                        nd.type = "MAX";
                        nd.ab = alpha;
                    }else{
                        nd.type = "MIN";
                        nd.ab = beta;
                    }
                    abcut_nodes.push_back(nd);
                    if (turn == 2){
                        //极大值层
                        return alpha;
                    }else{
                        return beta;
                    }
                }
            }
        }
    }
    if (turn == 2)return alpha;
    return beta;
}

POS AI(const ChessBoard &b, int depth, bool cut){
    POS p;
    ChessBoard bcopy = b;
    int j = abcut(bcopy, depth, -INT_INF, INT_INF, 2, p, cut);
    cout << "Best Val: " << j << endl;
    return p;
}

#endif
