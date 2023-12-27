#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <vector>
#include <unordered_set>
#include "rush_utility.hpp"

namespace RUSH{

class Solver {
private:
    BitBoard initBoard;
    std::vector<Node> BFS_Q;
    std::unordered_set<idPair,myHash> visitedNodes;

public:
    Solver(){
        Grid grid;
        grid.getInput();
        Grid2Board(grid, initBoard);
    }

    void Print(BitBoard &board){
        Grid grid;
        Board2Grid(board,grid);
        grid.print();
    }

    void Print_initBoard(){
        Print(initBoard);
    }

    std::pair<std::pair<int,int>,std::pair<int,int>> getHorizontalMove(BitBoard &board,int i,int &j1,int &len1,int &j2,int &len2){
        std::pair<std::pair<int,int>,std::pair<int,int>> ret;
        switch (board.rowCnt[i]){
            case 2:
            case 3:{
                int j=0;
                while (!(board[0][i]&(1<<j))) ++j;
                int l=j-1,r=j+board.rowCnt[i];
                while (l>=0&&!(board[1][l]&(1<<i))) --l;
                while (r<6&&!(board[1][r]&(1<<i))) ++r;
                ret.first=std::make_pair((j-1)-l,r-(j+board.rowCnt[i]));
                ret.second=std::make_pair(0,0);
                j1=j;
                len1=board.rowCnt[i];
                j2=-1;
                len2=0;
                break;
            }
            case 4:{
                int localCnt=0;
                for (int j=0;j<6;++j){
                    if (board[0][i]&(1<<j)) ++localCnt;
                    if (localCnt==4) break;
                    if (localCnt&1){
                        int l=j-1,r=j+2;
                        while (l>=0&&!(board[1][l]&(1<<i))&&!(board[0][i]&(1<<l))) --l;
                        while (r<6&&!(board[1][r]&(1<<i))&&!(board[0][i]&(1<<r))) ++r;
                        if (localCnt==1){
                            ret.first=std::make_pair((j-1)-l,r-(j+2));
                            j1=j;
                            len1=2;
                        }else{
                            ret.second=std::make_pair((j-1)-l,r-(j+2));
                            j2=j;
                            len2=2;
                        }
                    }
                }                
                break;
            }
            case 5:{
                int j=0;
                len1=(board[0][i]&(1<<7))?3:2;
                len2=5-len1;
                j1=j2=-1;
                while (board[0][i]&(1<<j)||board[1][j]&(1<<i)) ++j;
                if (j==0){
                    ret.first=std::make_pair(1,0);
                    ret.second=std::make_pair(0,0);
                    j1=1;
                    j2=1+len1;
                }else if (j==5){
                    ret.first=std::make_pair(0,0);
                    ret.second=std::make_pair(0,1);
                    j1=0;
                    j2=len1;
                }else{
                    ret.first=std::make_pair(0,1);
                    ret.second=std::make_pair(1,0);
                    j1=0;
                    j2=j+1;
                }
                break;
            }
            default:
                j1=j2=-1;
                break;
        }
        return ret;
    }

    std::pair<std::pair<int,int>,std::pair<int,int>> getVerticalMove(BitBoard &board,int j,int &i1,int &len1,int &i2,int &len2){
        std::pair<std::pair<int,int>,std::pair<int,int>> ret;
        switch (board.colCnt[j]){
            case 2:
            case 3:{
                int i=0;
                while (!(board[1][j]&(1<<i))) ++i;
                int l=i-1,r=i+board.colCnt[j];
                while (l>=0&&!(board[0][l]&(1<<j))) --l;
                while (r<6&&!(board[0][r]&(1<<j))) ++r;
                ret.first=std::make_pair((i-1)-l,r-(i+board.colCnt[j]));
                ret.second=std::make_pair(0,0);
                i1=i;
                len1=board.colCnt[j];
                i2=-1;
                len2=0;
                break;
            }
            case 4:{
                int localCnt=0;
                for (int i=0;i<6;++i){
                    if (board[1][j]&(1<<i)) ++localCnt;
                    if (localCnt==4) break;
                    if (localCnt&1){
                        int l=i-1,r=i+2;
                        while (l>=0&&!(board[0][l]&(1<<j))&&!(board[1][j]&(1<<l))) --l;
                        while (r<6&&!(board[0][r]&(1<<j))&&!(board[1][j]&(1<<r))) ++r;
                        if (localCnt==1){
                            ret.first=std::make_pair((i-1)-l,r-(i+2));
                            i1=i;
                            len1=2;
                        }else{
                            ret.second=std::make_pair((i-1)-l,r-(i+2));
                            i2=i;
                            len2=2;
                        }
                    }
                }                
                break;
            }
            case 5:{
                int i=0;
                len1=(board[1][j]&(1<<7))?3:2;
                len2=5-len1;
                i1=i2=-1;
                while (board[0][i]&(1<<j)||board[1][j]&(1<<i)) ++i;
                if (i==0){
                    ret.first=std::make_pair(1,0);
                    ret.second=std::make_pair(0,0);
                    i1=1;
                    i2=1+len1;
                }else if (i==5){
                    ret.first=std::make_pair(0,0);
                    ret.second=std::make_pair(0,1);
                    i1=0;
                    i2=len1;
                }else{
                    ret.first=std::make_pair(0,1);
                    ret.second=std::make_pair(1,0);
                    i1=0;
                    i2=i+1;
                }
                break;
            }
            default:
                i1=i2=-1;
                break;
        }
        return ret;
    }

    void solve(){
        BFS_Q.push_back(Node{initBoard,0,0});
        visitedNodes.insert(std::make_pair(initBoard.rowID(),initBoard.colID()));
        size_t idx=0;
        while (idx!=BFS_Q.size()){
            if (BFS_Q[idx].board.solved()){
                PrintProcess(BFS_Q[idx]);
                std::cout<<"Puzzle Solved in "<<BFS_Q[idx].dis+1<<" steps!"<<std::endl;
                return;
            }
            for (int i=0;i<6;++i){
                int j1,len1,j2,len2;
                auto movePair=getHorizontalMove(BFS_Q[idx].board,i,j1,len1,j2,len2);
                if (j1>=0){
                    for (int k=1;k<=movePair.first.first;++k){
                        BitBoard newBoard(BFS_Q[idx].board);
                        for (int j=j1;j<j1+len1;++j){
                            newBoard[0][i]&=~(1<<j);
                            newBoard[0][i]|=1<<(j-k);
                        }
                        idPair id=std::make_pair(newBoard.rowID(),newBoard.colID());
                        if (visitedNodes.count(id)) continue;
                        BFS_Q.push_back(Node{newBoard,BFS_Q[idx].dis+1,idx});
                        visitedNodes.insert(id);
                    }
                    for (int k=1;k<=movePair.first.second;++k){
                        BitBoard newBoard(BFS_Q[idx].board);
                        for (int j=j1+len1-1;j>=j1;--j){
                            newBoard[0][i]&=~(1<<j);
                            newBoard[0][i]|=1<<(j+k);
                        }
                        idPair id=std::make_pair(newBoard.rowID(),newBoard.colID());
                        if (visitedNodes.count(id)) continue;
                        BFS_Q.push_back(Node{newBoard,BFS_Q[idx].dis+1,idx});
                        visitedNodes.insert(id);
                    }
                }
                if (j2>=0){
                    for (int k=1;k<=movePair.second.first;++k){
                        BitBoard newBoard(BFS_Q[idx].board);
                        for (int j=j2;j<j2+len2;++j){
                            newBoard[0][i]&=~(1<<j);
                            newBoard[0][i]|=1<<(j-k);
                        }
                        idPair id=std::make_pair(newBoard.rowID(),newBoard.colID());
                        if (visitedNodes.count(id)) continue;
                        BFS_Q.push_back(Node{newBoard,BFS_Q[idx].dis+1,idx});
                        visitedNodes.insert(id);
                    }
                    for (int k=1;k<=movePair.second.second;++k){
                        BitBoard newBoard(BFS_Q[idx].board);
                        for (int j=j2+len2-1;j>=j2;--j){
                            newBoard[0][i]&=~(1<<j);
                            newBoard[0][i]|=1<<(j+k);
                        }
                        idPair id=std::make_pair(newBoard.rowID(),newBoard.colID());
                        if (visitedNodes.count(id)) continue;
                        BFS_Q.push_back(Node{newBoard,BFS_Q[idx].dis+1,idx});
                        visitedNodes.insert(id);
                    }
                }
            }
            for (int j=0;j<6;++j){
                int i1,len1,i2,len2;
                auto movePair=getVerticalMove(BFS_Q[idx].board,j,i1,len1,i2,len2);
                if (i1>=0){
                    for (int k=1;k<=movePair.first.first;++k){
                        BitBoard newBoard(BFS_Q[idx].board);
                        for (int i=i1;i<i1+len1;++i){
                            newBoard[1][j]&=~(1<<i);
                            newBoard[1][j]|=1<<(i-k);
                        }
                        idPair id=std::make_pair(newBoard.rowID(),newBoard.colID());
                        if (visitedNodes.count(id)) continue;
                        BFS_Q.push_back(Node{newBoard,BFS_Q[idx].dis+1,idx});
                        visitedNodes.insert(id);
                    }
                    for (int k=1;k<=movePair.first.second;++k){
                        BitBoard newBoard(BFS_Q[idx].board);
                        for (int i=i1+len1-1;i>=i1;--i){
                            newBoard[1][j]&=~(1<<i);
                            newBoard[1][j]|=1<<(i+k);
                        }
                        idPair id=std::make_pair(newBoard.rowID(),newBoard.colID());
                        if (visitedNodes.count(id)) continue;
                        BFS_Q.push_back(Node{newBoard,BFS_Q[idx].dis+1,idx});
                        visitedNodes.insert(id);
                    }
                }
                if (i2>=0){
                    for (int k=1;k<=movePair.second.first;++k){
                        BitBoard newBoard(BFS_Q[idx].board);
                        for (int i=i2;i<i2+len2;++i){
                            newBoard[1][j]&=~(1<<i);
                            newBoard[1][j]|=1<<(i-k);
                        }
                        idPair id=std::make_pair(newBoard.rowID(),newBoard.colID());
                        if (visitedNodes.count(id)) continue;
                        BFS_Q.push_back(Node{newBoard,BFS_Q[idx].dis+1,idx});
                        visitedNodes.insert(id);
                    }
                    for (int k=1;k<=movePair.second.second;++k){
                        BitBoard newBoard(BFS_Q[idx].board);
                        for (int i=i2+len2-1;i>=i2;--i){
                            newBoard[1][j]&=~(1<<i);
                            newBoard[1][j]|=1<<(i+k);
                        }
                        idPair id=std::make_pair(newBoard.rowID(),newBoard.colID());
                        if (visitedNodes.count(id)) continue;
                        BFS_Q.push_back(Node{newBoard,BFS_Q[idx].dis+1,idx});
                        visitedNodes.insert(id);
                    }
                }
            }
            ++idx;
        }
    }

    void PrintProcess(Node &node){
        if (node.dis==0){
            putchar('\n');
            Print(node.board);
            putchar('\n');
        }else{
            PrintProcess(BFS_Q[node.last]);
            putchar('\n');
            Print(node.board);
            putchar('\n');
        }
    }
};

}

#endif